open Ast;

module Exceptions = {
  exception UnexpectedAtThisPoint(string);
  exception FeatureMissing(string, string);
};

type iPath = Identifier.path;
type runtime = {
  root_modules: array(Node.node(Node.Constraint.exactlyModule)),
  node_count: int,
  fully_qualified_added: list(string),
  warnings: bool,
};
type scope = {
  source_file: option(Ts_nodes.SourceFile.t),
  root_declarations: array(Node.node(Node.Constraint.moduleLevel)),
  current_declaration: option(Ts_nodes.nodeKind),
  current_declared_params:
    array(Identifier.t(Identifier.Constraint.exactlyTypeParameter)),
  path: iPath,
  has_any: bool,
  refs: Hashtbl.t(iPath, array(iPath)),
  context_params:
    list((string, option(Node.node(Node.Constraint.assignable)))),
  context_args: list((string, Node.node(Node.Constraint.assignable))),
};
type mapped_modifier = [ | `none | `to_optional | `to_required];
module Context = {
  let get_param = (key, scope) =>
    scope.context_params
    |> CCList.Assoc.get(~eq=CCString.equal, key)
    |> CCOpt.flatten;
  let has_param = (key, scope) =>
    scope.context_params |> CCList.Assoc.mem(~eq=CCString.equal, key);
  let get_param_key = (i, scope) =>
    scope.context_params |> CCList.get_at_idx(i) |> CCOpt.map(fst);
  let add_param = (key, ty, scope) => {
    ...scope,
    context_params:
      scope.context_params |> CCList.Assoc.set(~eq=CCString.equal, key, ty),
  };
  let clear_params = scope => {...scope, context_params: []};

  let clear_args = scope => {...scope, context_args: []};
  let has_arg = (key, scope) =>
    scope.context_args |> CCList.Assoc.mem(~eq=CCString.equal, key);
  let add_arg = (key, ty, scope) => {
    ...scope,
    context_args:
      scope.context_args |> CCList.Assoc.set(~eq=CCString.equal, key, ty),
  };
  let add_arg_tpl = ((key, ty), scope) => add_arg(key, ty, scope);
  let add_arg_lst = (lst, scope) =>
    lst |> CCList.fold_left((scope, tpl) => add_arg_tpl(tpl, scope), scope);
  let get_arg = (key, scope) =>
    scope.context_args |> CCList.Assoc.get(~eq=CCString.equal, key);
  let args_match_params = scope =>
    CCList.compare_lengths(
      scope.context_args,
      scope.context_params
      |> CCList.filter(((_, default)) => CCOpt.is_none(default)),
    )
    >= 0;
  let get_params_generalized = (scope: scope) =>
    scope.context_params
    |> CCList.map(((name, _)) =>
         (name, Node.GenericReference(Identifier.TypeParameter(name)))
       );
  let retain_default_params = (scope: scope) => {
    let context_params = scope.context_params;
    let context_args = scope.context_args;
    (
      {
        ...scope,
        context_params:
          scope.context_params |> CCList.map(((name, _)) => (name, None)),
        context_args: [],
      },
      newScope => {...newScope, context_params, context_args},
    );
  };
  let get_all_args = (scope: scope) =>
    args_match_params(scope)
      ? scope.context_args
      : scope.context_params
        |> CCList.map(((name, default)) =>
             switch (get_arg(name, scope), default) {
             | (Some(arg), _) => (name, arg)
             | (None, Some(arg)) => (name, arg)
             | (None, None) => (
                 name,
                 Node.GenericReference(Identifier.TypeParameter(name)),
               )
             }
           );
};
module Runtime = {
  let add_root_module =
      (root_module: Node.node(Node.Constraint.exactlyModule), runtime) => {
    ...runtime,
    root_modules: CCArray.append(runtime.root_modules, [|root_module|]),
  };
  let incr_node = runtime => {...runtime, node_count: runtime.node_count + 1};
  let warn = (e: exn, runtime: runtime) =>
    runtime.warnings ? Console.warn(Printexc.to_string(e)) : ();
  let warn_ftr = (a, b, runtime) =>
    runtime |> warn(Exceptions.FeatureMissing(a, b));
};
module Scope = {
  let add_to_path: (Identifier.t(_), scope) => scope =
    (i, scope) => {...scope, path: CCArray.append(scope.path, [|i|])};
  let replace_path: (Identifier.t(_), scope) => scope =
    (i, scope) => {...scope, path: [|i|]};
  let replace_path_arr: (array(Identifier.t(_)), scope) => scope =
    (path, scope) => {...scope, path};
  let add_root_declaration =
      (
        ~before=?,
        root_declaration: Node.node(Node.Constraint.moduleLevel),
        scope,
      ) => {
    {
      ...scope,
      root_declarations:
        switch (before) {
        | None =>
          CCArray.append(scope.root_declarations, [|root_declaration|])
        | Some(ident) =>
          let found_idx =
            scope.root_declarations
            |> CCArray.find_idx(decl =>
                 switch (decl) {
                 | Node.TypeDeclaration({name, _})
                     when name == Obj.magic(ident) =>
                   true
                 | Module({name, _}) when Identifier.Module(name) == ident =>
                   true
                 | _ => false
                 }
               );
          switch (found_idx) {
          | None =>
            CCArray.append([|root_declaration|], scope.root_declarations)
          | Some((insert_before, _))
              when insert_before == 0 || insert_before == 1 =>
            CCArray.append([|root_declaration|], scope.root_declarations)
          | Some((insert_before, _)) =>
            CCArray.concat([
              CCArray.sub(scope.root_declarations, 0, insert_before - 1),
              [|root_declaration|],
              CCArray.sub(
                scope.root_declarations,
                insert_before,
                CCArray.length(scope.root_declarations) - insert_before - 1,
              ),
            ])
          };
        },
    };
  };
  let retain_path = (replace_path, scope) => {
    let base_path = scope.path;
    (
      scope |> replace_path_arr(replace_path),
      scope => scope |> replace_path_arr(base_path),
    );
  };
  let add_ref = (to_: iPath, from: iPath, scope: scope) => {
    Hashtbl.replace(
      scope.refs,
      to_,
      CCArray.append(
        CCHashtbl.get_or(scope.refs, to_, ~default=[||]),
        [|from|],
      ),
    );
    scope;
  };
  let get_ref = (to_: iPath, scope: scope) => {
    Hashtbl.find_opt(scope.refs, to_);
  };
  let decrease_ref = (p: iPath, scope: scope) => {
    switch (get_ref(p, scope)) {
    | None => ()
    | Some(refs) when CCArray.length(refs) <= 1 =>
      Hashtbl.remove(scope.refs, p)
    | Some(refs) =>
      Hashtbl.replace(
        scope.refs,
        p,
        CCArray.sub(refs, 1, CCArray.length(refs) - 1),
      )
    };
  };
};
module Path = {
  type t = Identifier.path;
  let hd = (p: t) => {
    CCArray.get_safe(p, CCArray.length(p) - 1);
  };
  let add = (i, p: t) => CCArray.append(p, [|i|]);
  let append = (i, p: t) => CCArray.append(p, i);
  let hd_unsafe = (p: t) => CCArray.get(p, CCArray.length(p) - 1);
  let is_sub = i =>
    switch (i) {
    | Identifier.SubIdent(_)
    | Identifier.SubName(_) => true
    | _ => false
    };
  let unwrap: type t. Identifier.t(t) => string =
    i =>
      switch (i) {
      | Module(str)
      | TypeName(str)
      | TypeParameter(str)
      | PropertyName(str)
      | VariantIdentifier(str, _)
      | SubName(str) => str
      | SubIdent(1) => "1st"
      | SubIdent(2) => "2nd"
      | SubIdent(3) => "3rd"
      | SubIdent(i) => string_of_int(i) ++ "th"
      };
  let fold_fun:
    type t.
      (Identifier.t(t), (array(string), bool)) => (array(string), bool) =
    (i, (arr, finished)) =>
      finished
        ? (arr, finished)
        : (
          switch (i) {
          | TypeParameter(str)
          | PropertyName(str)
          | SubName(str) => (CCArray.append([|str|], arr), false)
          | SubIdent(_) as si => (
              CCArray.append([|unwrap(si)|], arr),
              false,
            )
          | TypeName(str) => (CCArray.append([|str|], arr), true)
          | Identifier.VariantIdentifier(_)
          | Module(_) => (arr, true)
          }
        );
  let make_sub_type_name = (p: t) => {
    Array.fold_right(fold_fun, p, ([||], false))
    |> fst
    |> CCArray.to_string(~sep="_", a => a);
  };

  let make_current_scope = (p: t) => {
    CCArray.filter(
      ident =>
        switch (ident) {
        | Identifier.Module(_) => true
        | _ => false
        },
      p,
    );
  };

  let strip_root_module = (p: t) =>
    switch (CCArray.get_safe(p, 0)) {
    | Some(Module(_)) => CCArray.sub(p, 1, CCArray.length(p) - 1)
    | Some(_)
    | None => p
    };

  let strip_current_sub_path = (p: t) => {
    CCArray.fold_right(
      (current, prev) => {
        CCArray.length(prev) > 0
          ? CCArray.append([|current|], prev)
          : (
            switch (current) {
            | Identifier.SubName(_)
            | SubIdent(_) => prev
            | other => [|other|]
            }
          )
      },
      p,
      [||],
    );
  };

  let eq = (p1: t, p2: t) => CCArray.equal((a, b) => a == b, p1, p2);
};

let build_path_from_ref_string = (~scope, ref_string: string) => {
  let path_parts = CCString.split_on_char('.', ref_string);
  let parts_count = CCList.length(path_parts);

  let rec parse_parts = parts =>
    switch (parts) {
    | [] => []
    | [one] => [Identifier.TypeName(one)]
    | [first, ...rest] when CCString.contains(first, '/') =>
      let first = CCString.replace(~sub="\"", ~by="", first);
      switch (CCString.Split.right(~by="/", first) |> CCOpt.map(snd)) {
      | None => parse_parts(rest)
      | Some(first) => parse_parts([first, ...rest])
      };
    | [first, ..._] => [Identifier.Module(first)]
    };
  let result = parse_parts(path_parts) |> CCArray.of_list;
  if (parts_count == 1) {
    CCArray.append(Path.make_current_scope(scope.path), result);
  } else {
    result;
  };
};

let parse__AssignAny = (~runtime, ~scope) => {
  (runtime, {...scope, has_any: true}, Node.Basic(Any));
};

let node_contains_type_parameter =
    (param_name: string, node: Ts_nodes.Generic.t) => {
  let result = ref(false);
  node#forEachDescendant(des =>
    result^
      ? ()
      : {
        switch ((des |> Ts_nodes.WithGetType.fromGeneric)#getType()) {
        | Some(t) when t#isTypeParameter() && t#getText() == param_name =>
          result := true
        | Some(_)
        | None => ()
        };
      }
  );
  result^;
};

let resolved_node_replace_type_parameter =
    (~runtime, ~scope, node: Node.node(Node.Constraint.any)) => {
  (
    runtime,
    scope,
    switch (node) {
    | Node.GenericReference(TypeParameter(name)) =>
      switch (scope |> Context.get_arg(name)) {
      | None => node
      | Some(arg) => arg |> Node.Escape.toAny
      }
    | other => other
    },
  );
};

let parse__map = (map_func, (runtime, scope, payload)) => (
  runtime,
  scope,
  map_func(payload),
);

let find_td = (path, current_type_list) =>
  CCArray.find_idx(
    node =>
      switch (node) {
      | Node.TypeDeclaration({path: p, _}) when Path.eq(p, path) => true
      | _ => false
      },
    current_type_list,
  )
  |> CCOpt.flat_map(((i, v)) =>
       switch (v) {
       | Node.TypeDeclaration(inner) => Some((i, inner))
       | _ => None
       }
     );

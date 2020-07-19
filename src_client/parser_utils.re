open Ast;

module Exceptions = {
  exception UnexpectedAtThisPoint(string);
  exception FeatureMissing(string, string);
};

type iPath = Identifier.path;
type parse_config = {exports_only: bool};
type runtime = {
  root_modules: array(Node.node(Node.Constraint.exactlyModule)),
  node_count: int,
  parse_config,
  fully_qualified_added: list(string),
};
type scope = {
  source_file: option(Ts_nodes.SourceFile.t),
  root_declarations: array(Node.node(Node.Constraint.moduleLevel)),
  parent: option(Ts_nodes.nodeKind),
  path: iPath,
  has_any: bool,
  refs: Hashtbl.t(iPath, array(iPath)),
  context_params:
    list((string, option(Node.node(Node.Constraint.assignable)))),
  context_args: list((string, Node.node(Node.Constraint.assignable))),
  context_args_raw: list((string, Ts_nodes.Generic.t)),
};
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
  let get_arg = (key, scope) =>
    scope.context_args |> CCList.Assoc.get(~eq=CCString.equal, key);
};
module Runtime = {
  let add_root_module =
      (root_module: Node.node(Node.Constraint.exactlyModule), runtime) => {
    ...runtime,
    root_modules: CCArray.append(runtime.root_modules, [|root_module|]),
  };
  let incr_node = runtime => {...runtime, node_count: runtime.node_count + 1};
};
module Scope = {
  let add_to_path: (Identifier.t(_), scope) => scope =
    (i, scope) => {...scope, path: CCArray.append(scope.path, [|i|])};
  let replace_path: (Identifier.t(_), scope) => scope =
    (i, scope) => {...scope, path: [|i|]};
  let replace_path_arr: (array(Identifier.t(_)), scope) => scope =
    (path, scope) => {...scope, path};
  let add_root_declaration =
      (root_declaration: Node.node(Node.Constraint.moduleLevel), scope) => {
    {
      ...scope,
      root_declarations:
        CCArray.append(scope.root_declarations, [|root_declaration|]),
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
};
module Path = {
  type t = Identifier.path;
  let hd = (p: t) => {
    CCArray.get_safe(p, CCArray.length(p) - 1);
  };
  let add = (i, p: t) => CCArray.append(p, [|i|]);
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
      | VariantIdentifier(str)
      | SubName(str) => str
      | SubIdent(i) => string_of_int(i)
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
          | SubIdent(num) => (
              CCArray.append([|string_of_int(num)|], arr),
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

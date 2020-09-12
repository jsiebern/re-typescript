module Naming = Ast_generator_utils.Naming;
open Parser_utils;
open Ast;

type unionType =
  | Union(array(Node.node(Node.Constraint.assignable)))
  | StringLiteral(array(string))
  | NumericLiteral(array(float))
  | MixedLiteral(array(Node.node(Node.Constraint.exactlyLiteral)))
  | Discriminating
  | Nullable(array(Node.node(Node.Constraint.assignable)))
  | Optional(array(Node.node(Node.Constraint.assignable)))
  | Single(Node.node(Node.Constraint.assignable));

module Cache = {
  type t = Hashtbl.t(Path.t, unionType);
  let cache: t = Hashtbl.create(0);
  let add = (path, t) => Hashtbl.replace(cache, path, t);
  let get = path => Hashtbl.find_opt(cache, path);
  let has = path => CCOpt.is_some(get(path));
  let clear = () => Hashtbl.clear(cache);
};

let rec make_union_typename:
  type t. (~runtime: runtime, ~scope: scope, Node.node(t)) => string =
  (~runtime, ~scope, node) =>
    switch (node) {
    | Basic(String) => "string"
    | Basic(Number) => "number"
    | Basic(Boolean) => "boolean"
    | Basic(Void) => "void"
    | Basic(Any) => "any"
    | Basic(Null) => "null"
    | Basic(Undefined) => "undefined"
    | Basic(Never) => "never"
    | Basic(This) => "this"
    | Basic(RelevantKeyword(str))
    | Literal(String(str)) => Naming.typeName(str)
    | Literal(Number(num)) => Naming.typeName(Printf.sprintf("%.0f", num))
    | Literal(Boolean(true)) => "true"
    | Literal(Boolean(false)) => "false"
    | Function(_) => "func"
    | Record(_) => "obj"
    | Variant(_) => "var"
    | Tuple(_) => "tpl"
    | Array(t) =>
      Printf.sprintf("arr_of_%s", make_union_typename(~runtime, ~scope, t))
    | Reference({target, _}) =>
      switch (
        scope.root_declarations
        |> find_td(
             CCArray.append(scope.path |> Path.make_current_scope, target),
           )
      ) {
      | Some((_, {annot: Basic(_) as annot, _})) =>
        make_union_typename(~runtime, ~scope, annot)
      | Some(_)
      | None =>
        let config = Re_typescript_config.getConfig();
        switch (config.print_language) {
        | ReasonML =>
          Ast_generator_utils.Naming.to_valid_ident(
            Path.make_sub_type_name(target),
          )
        | ReScript => Path.make_sub_type_name(target)
        };
      }
    | Nullable(t)
    | Optional(t) => make_union_typename(~runtime, ~scope, t)
    | GenericReference(TypeParameter(t)) =>
      let config = Re_typescript_config.getConfig();
      switch (config.print_language) {
      | ReasonML => Ast_generator_utils.Naming.to_valid_ident(t)
      | ReScript => t
      };
    | _ => "other"
    };

let checks = (~scope: scope) => [|
  // --- Undefined
  nodes =>
    nodes
    |> CCArray.find_idx(node =>
         switch (node) {
         | Node.Basic(Undefined) => true
         | _ => false
         }
       )
    |> CCOpt.map(((idx, _)) =>
         Optional(CCArray.except_idx(nodes, idx) |> CCArray.of_list)
       ),
  // --- Nullable
  nodes =>
    nodes
    |> CCArray.find_idx(node =>
         switch (node) {
         | Node.Basic(Null) => true
         | _ => false
         }
       )
    |> CCOpt.map(((idx, _)) =>
         Nullable(CCArray.except_idx(nodes, idx) |> CCArray.of_list)
       ),
  // --- StringLiteral
  nodes =>
    nodes
    |> CCArray.for_all(node => {
         switch (node) {
         | Node.Literal(String(_)) => true
         | Reference({target, _}) =>
           Cache.get(
             Path.make_current_scope(scope.path) |> Path.append(target),
           )
           |> CCOpt.map_or(~default=false, t =>
                switch (t) {
                | StringLiteral(_) => true
                | _ => false
                }
              )
         | _ => false
         }
       })
      ? Some(
          StringLiteral(
            nodes
            |> CCArray.fold_left(
                 (acc, node) =>
                   switch (node) {
                   | Node.Literal(String(str)) =>
                     CCArray.append(acc, [|str|])
                   | Reference({target, _}) =>
                     CCArray.append(
                       acc,
                       switch (
                         Cache.get(
                           Path.make_current_scope(scope.path)
                           |> Path.append(target),
                         )
                         |> CCOpt.get_exn
                       ) {
                       | StringLiteral(v) => v
                       | _ => raise(Invalid_argument("Not StringLiteral"))
                       },
                     )
                   | other => raise(Invalid_argument(Pp.ast_node(other)))
                   },
                 [||],
               ),
          ),
        )
      : None,
  // --- Numeric Literal
  nodes =>
    nodes
    |> CCArray.for_all(node =>
         switch (node) {
         | Node.Literal(Number(_)) => true
         | Reference({target, _}) =>
           Cache.get(
             Path.make_current_scope(scope.path) |> Path.append(target),
           )
           |> CCOpt.map_or(~default=false, t =>
                switch (t) {
                | NumericLiteral(_) => true
                | _ => false
                }
              )
         | _ => false
         }
       )
      ? Some(
          NumericLiteral(
            nodes
            |> CCArray.fold_left(
                 (acc, node) =>
                   switch (node) {
                   | Node.Literal(Number(num)) =>
                     CCArray.append(acc, [|num|])
                   | Reference({target, _}) =>
                     CCArray.append(
                       acc,
                       switch (
                         Cache.get(
                           Path.make_current_scope(scope.path)
                           |> Path.append(target),
                         )
                         |> CCOpt.get_exn
                       ) {
                       | NumericLiteral(v) => v
                       | _ => raise(Invalid_argument("Not NumericLiteral"))
                       },
                     )
                   | other => raise(Invalid_argument(Pp.ast_node(other)))
                   },
                 [||],
               ),
          ),
        )
      : None,
  // --- Mixed Literal
  nodes =>
    nodes
    |> CCArray.for_all(node =>
         switch (node) {
         | Node.Literal(Number(_)) => true
         | Literal(String(_)) => true
         | Literal(Boolean(_)) => true
         | Reference({target, _}) =>
           Cache.get(
             Path.make_current_scope(scope.path) |> Path.append(target),
           )
           |> CCOpt.map_or(~default=false, t =>
                switch (t) {
                | StringLiteral(_) => true
                | NumericLiteral(_) => true
                | MixedLiteral(_) => true
                | _ => false
                }
              )
         | _ => false
         }
       )
      ? Some(
          MixedLiteral(
            nodes
            |> CCArray.fold_left(
                 (acc, node) =>
                   switch (node) {
                   | Node.Literal(_) as n => CCArray.append(acc, [|n|])
                   | Reference({target, _}) =>
                     CCArray.append(
                       acc,
                       switch (
                         Cache.get(
                           Path.make_current_scope(scope.path)
                           |> Path.append(target),
                         )
                         |> CCOpt.get_exn
                       ) {
                       | NumericLiteral(v) =>
                         v |> CCArray.map(f => Node.Literal(Number(f)))
                       | StringLiteral(v) =>
                         v |> CCArray.map(f => Node.Literal(String(f)))
                       | MixedLiteral(v) => v
                       | _ => raise(Invalid_argument("Not Mixed Literal"))
                       },
                     )
                   | other => raise(Invalid_argument(Pp.ast_node(other)))
                   },
                 [||],
               ),
          ),
        )
      : None,
  // --- Todo: Discrimimeting
  // --- Union
  nodes => Some(Union(nodes)),
|];

let determine_union_type =
    (~scope: scope, nodes: array(Node.node(Node.Constraint.assignable))) => {
  // Important: Scope is readonly here as it's not being returned
  switch (nodes) {
  | [|one|]
      when
        switch (one) {
        | Literal(_) => false
        | _ => true
        } =>
    Some(Single(one))
  | nodes =>
    checks(~scope)
    |> CCArray.fold_left(
         (prev, fn) => {
           switch (prev) {
           | None => fn(nodes)
           | Some(_) => prev
           }
         },
         None,
       )
  };
};

let generate_ast_for_union:
  (
    ~runtime: runtime,
    ~scope: scope,
    array(Node.node(Node.Constraint.assignable))
  ) =>
  (runtime, scope, Node.node(Node.Constraint.assignable)) =
  (~runtime, ~scope, members) => {
    let name = Path.make_sub_type_name(scope.path);
    let wrapper_module_name = Ast_generator_utils.Naming.moduleName(name);
    let scope = ref(scope);
    let base_as_target = Path.strip_root_module(scope^.path);

    let name_cache = ref([||]);
    let members =
      members
      |> CCArray.map(node => {
           let name = make_union_typename(~runtime, ~scope=scope^, node);
           let suffix = ref("");
           let i = ref(1);
           while (CCArray.find_idx(n => n == name ++ suffix^, name_cache^)
                  |> CCOpt.is_some) {
             i := i^ + 1;
             suffix := string_of_int(i^);
           };
           let name = name ++ suffix^;
           name_cache := CCArray.append(name_cache^, [|name|]);
           let type_name = Identifier.TypeName(name);
           let path =
             scope^.path
             |> Path.add(Module(wrapper_module_name))
             |> Path.add(type_name);
           // Add a new reference point here if necessary (background: original references while generating would originate from the primary type path)
           // Also parse possible recursive types here and map them to type "t"
           // Again, TODO: Can most certainly be abstracted to somewhere else (maybe some traversal tools module)
           let node =
             switch (node) {
             | Reference({
                 target: [|TypeName("Array")|],
                 params: [(_, Reference({target, _} as r))],
               })
                 when Path.eq(target, base_as_target) =>
               Node.Array(Reference({...r, target: [|TypeName("t")|]}))
             | Array(Reference({target, _} as r))
                 when Path.eq(target, base_as_target) =>
               Array(Reference({...r, target: [|TypeName("t")|]}))
             | Optional(Reference({target, _} as r))
                 when Path.eq(target, base_as_target) =>
               Optional(Reference({...r, target: [|TypeName("t")|]}))
             | Nullable(Reference({target, _} as r))
                 when Path.eq(target, base_as_target) =>
               Nullable(Reference({...r, target: [|TypeName("t")|]}))
             | Reference({target, _} as r)
                 when Path.eq(target, base_as_target) =>
               Reference({...r, target: [|TypeName("t")|]})
             | Reference({target, _}) =>
               switch (
                 scope^.root_declarations
                 |> find_td(
                      CCArray.append(
                        scope^.path |> Path.make_current_scope,
                        target,
                      ),
                    )
               ) {
               | Some((_, {annot: Basic(_), _})) =>
                 // Do not add additional reference if it's a basic type as that get's printed directly and there's no need to keep it around as a declaration
                 ()
               | Some(_)
               | None =>
                 scope :=
                   scope^
                   |> Scope.add_ref(
                        Path.make_current_scope(scope^.path)
                        |> Path.append(target),
                        path,
                      )
               };
               node;
             | _ => node
             };
           Node.TypeDeclaration({
             name: type_name,
             path,
             annot: node,
             params: [||],
           });
         });

    // Try find all recursive references and then pull them into this module
    let (runtime, scope, extracted) =
      find_and_extract(~runtime, ~scope=scope^, base_as_target);

    let target = [|Identifier.Module(wrapper_module_name), TypeName("t")|];
    let path = Path.make_current_scope(scope.path) |> Path.append(target);
    let wrapper_module =
      Node.Module({
        name: wrapper_module_name,
        path: path |> Path.make_sub_type_name,
        types:
          CCArray.append(
            [|
              Node.Fixture(TUnboxed(scope.context_params, extracted), path),
            |],
            members,
          ),
      });
    let scope = scope |> Scope.add_root_declaration(wrapper_module);
    (
      runtime,
      scope,
      Reference({target, params: Context.get_params_generalized(scope)}),
    );
  };

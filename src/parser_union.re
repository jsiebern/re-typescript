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
      | None => Path.make_sub_type_name(target)
      | Some((_, {annot, _})) =>
        make_union_typename(~runtime, ~scope, annot)
      }
    | Nullable(t)
    | Optional(t) => make_union_typename(~runtime, ~scope, t)
    | _ => "other"
    };

let checks = [|
  // --- Undefined
  lazy(
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
         )
  ),
  // --- Nullable
  lazy(
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
         )
  ),
  // --- StringLiteral
  lazy(
    nodes =>
      nodes
      |> CCArray.for_all(node =>
           switch (node) {
           | Node.Literal(String(_)) => true
           | _ => false
           }
         )
        ? Some(
            StringLiteral(
              nodes
              |> CCArray.map(node =>
                   switch (node) {
                   | Node.Literal(String(str)) => str
                   | other => raise(Invalid_argument(Pp.ast_node(other)))
                   }
                 ),
            ),
          )
        : None
  ),
  // --- Numeric Literal
  lazy(
    nodes =>
      nodes
      |> CCArray.for_all(node =>
           switch (node) {
           | Node.Literal(Number(_)) => true
           | _ => false
           }
         )
        ? Some(
            NumericLiteral(
              nodes
              |> CCArray.map(node =>
                   switch (node) {
                   | Node.Literal(Number(num)) => num
                   | other => raise(Invalid_argument(Pp.ast_node(other)))
                   }
                 ),
            ),
          )
        : None
  ),
  // --- Mixed Literal
  lazy(
    nodes =>
      nodes
      |> CCArray.for_all(node =>
           switch (node) {
           | Node.Literal(Number(_)) => true
           | Node.Literal(String(_)) => true
           | Node.Literal(Boolean(_)) => true
           | _ => false
           }
         )
        ? Some(
            MixedLiteral(
              nodes
              |> CCArray.map(node =>
                   switch (node) {
                   | Node.Literal(_) as n => n
                   | other => raise(Invalid_argument(Pp.ast_node(other)))
                   }
                 ),
            ),
          )
        : None
  ),
  // --- Todo: Discrimimeting
  // --- Union
  lazy(nodes => Some(Union(nodes))),
|];

let determine_union_type =
    (nodes: array(Node.node(Node.Constraint.assignable))) => {
  switch (nodes) {
  | [|one|]
      when
        switch (one) {
        | Literal(_) => false
        | _ => true
        } =>
    Some(Single(one))
  | nodes =>
    checks
    |> CCArray.fold_left(
         (prev, lazy_func) => {
           switch (prev) {
           | None => Lazy.force(lazy_func, nodes)
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
    let members =
      members
      |> CCArray.map(node => {
           let name = make_union_typename(~runtime, ~scope, node);
           let type_name = Identifier.TypeName(name);
           Node.TypeDeclaration({
             name: type_name,
             path:
               scope.path
               |> Path.add(Module(wrapper_module_name))
               |> Path.add(type_name),
             annot: node,
             params: [||],
           });
         });

    let wrapper_module =
      Node.Module({
        name: wrapper_module_name,
        path: "",
        types: CCArray.append([|Node.Fixture(TUnboxed)|], members),
      });
    let scope = scope |> Scope.add_root_declaration(wrapper_module);

    (
      runtime,
      scope,
      Reference({
        target: [|Module(wrapper_module_name), TypeName("t")|],
        params: [],
      }),
    );
  };

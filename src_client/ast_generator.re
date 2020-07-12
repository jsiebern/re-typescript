// TODO: Implement the options from before <--- ACTUALLY: I want this AST to be as pure as possible, so all the logic will be moved to the parser.
// TODO: Implement the tests. Probably not a good idea to have JSOO run rely, but who knows
open Migrate_parsetree;
open Ast_406;
open Parsetree;
open Ast_helper;
module Util = Ast_generator_utils;

open Ast;

type scope = {
  path: Identifier.path,
  parent: option(Node.node(Node.Constraint.any)),
};
let defaultScope = {path: [||], parent: None};

// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Entry (Returns the generated OCaml AST)
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
let rec generate =
        (~scope=defaultScope, nodes: array(Node.node(Node.Constraint.any))) => {
  nodes
  |> CCArray.to_list
  |> CCList.fold_left(
       ((scope, struct_carry), node) => {
         let (scope, generated_struct) = generate__Node(~scope, node);
         (scope, struct_carry @ [generated_struct]);
       },
       (scope, []),
     );
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Generate Node
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and generate__Node = (~scope, node: Node.node(Node.Constraint.any)) => {
  switch (node) {
  | SourceFile(_) as source_file =>
    print__Node__SourceFile(~scope, source_file)
  | _ => raise(Failure("Not implemented"))
  };
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- SourceFile
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and print__Node__SourceFile =
    (~scope, SourceFile(node): Node.node(Node.Constraint.exactlySourceFile)) => {
  let module_name = Util.Naming.moduleName(node.name);
  let scope = {
    parent: Some(SourceFile(node)),
    path: [|Module(module_name)|],
  };

  let (scope, generated_structure) =
    node.types
    |> CCArray.to_list
    |> CCList.fold_left(
         ((scope, struct_carry), node) => {
           let (scope, generated_struct) =
             generate__Node__TypeDeclaration(~scope, node);
           (scope, struct_carry @ generated_struct);
         },
         (scope, []),
       );

  (scope, Util.make_module(module_name, generated_structure));
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Type Declarations
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and generate__Node__TypeDeclaration =
    (~scope, node: Node.node(Node.Constraint.moduleLevel)) => {
  switch (node) {
  | Fixture(_) as fixtureNode => generate__Fixture(~scope, fixtureNode)
  | TypeDeclaration(node) =>
    let type_name = node.name;
    let scope = {
      parent: Some(TypeDeclaration(node)),
      path: scope.path |> CCArray.append([|type_name|]),
    };

    let (scope, annotated_type) =
      generate__Node__Assignable_CoreType(~scope, node.annot);

    (
      scope,
      switch (annotated_type) {
      | None => []
      | Some(annotated_type) => [
          Util.make_type_declaration(
            ~aliasName=Util.Naming.fromIdentifier(type_name),
            ~aliasType=annotated_type,
          ),
        ]
      },
    );
  // extracted_nodes:
  // params: array(Node.node(Node.Constraint.exactlyTypeParameter)),
  // Util.make
  };
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Generate Assignable Node (As Core Type)
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and generate__Node__Assignable_CoreType =
    (~scope, node: Node.node(Node.Constraint.assignable)) => {
  let return_basic = (~scope=scope, ~inner=?, t: string) => (
    scope,
    Some(Util.make_type_constraint(~inner?, t)),
  );

  switch (node) {
  | Basic(Never) => (scope, None)
  | Basic(This) => (scope, None)
  | Basic(RelevantKeyword(keyword)) =>
    // TODO: Implement stuff like Set / Map / Object / Function
    return_basic(keyword)
  | Basic(basic) =>
    switch (basic) {
    | String => return_basic("string")
    | Number =>
      // TODO: React to configured number mode
      return_basic("float")
    | Boolean => return_basic("bool")
    | Void => return_basic("unit")
    | Any => return_basic("any")
    | Null =>
      return_basic(~inner=[Util.make_type_constraint("unit")], "Js.null")
    | Undefined =>
      return_basic(~inner=[Util.make_type_constraint("unit")], "option")
    | RelevantKeyword(_)
    | This
    | Never => raise(Failure("This case should not be reached"))
    }
  | _ => raise(Failure("Should this be handled here?"))
  };
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Generate Fixtures
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and generate__Fixture =
    (~scope, Fixture(fixture): Node.node(Node.Constraint.exactlyFixture)) => {
  switch (fixture) {
  | AnyUnboxed => (scope, Util.make_any_helper_unboxed())
  };
};

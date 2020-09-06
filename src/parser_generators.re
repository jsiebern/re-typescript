open Ast;
open Node;
open Parser_utils;

let generate_string_literal_list = (~runtime, ~scope, strings: array(string)) => {
  (
    runtime,
    scope,
    Variant(
      strings
      |> CCArray.map(name =>
           {
             VariantConstructor.name: Identifier.VariantIdentifier(name),
             arguments: [||],
           }
         ),
    ),
  );
};

let generate_number_literal_list = (~runtime, ~scope, floats: array(float)) => {
  (
    runtime,
    scope,
    Variant(
      floats
      |> CCArray.map(num =>
           {
             VariantConstructor.name:
               Identifier.VariantIdentifier(Printf.sprintf("%.0f", num)),
             arguments: [||],
           }
         ),
    ),
  );
};

let generate_mixed_literal_list =
    (
      ~runtime,
      ~scope,
      literals: array(Node.node(Node.Constraint.exactlyLiteral)),
    ) => {
  let name = Path.make_sub_type_name(scope.path);
  let wrapper_module_name = Ast_generator_utils.Naming.moduleName(name);

  let members =
    literals
    |> CCArray.map(
         (Node.Literal(node): Node.node(Node.Constraint.exactlyLiteral)) => {
         let name =
           switch (node) {
           | String(str) => str
           | Number(num) => Printf.sprintf("%.0f", num)
           | Boolean(true) => "true"
           | Boolean(false) => "false"
           };
         let config = Re_typescript_config.getConfig();
         let name =
           switch (config.print_language) {
           | ReasonMl => name |> Ast_generator_utils.Naming.to_valid_ident
           | ReScript => name
           };
         let type_name = Identifier.TypeName(name);
         Node.TypeDeclaration({
           name: type_name,
           path:
             scope.path
             |> Path.add(Module(wrapper_module_name))
             |> Path.add(type_name),
           annot: Literal(node),
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

let generate_intersection_body =
    (types: array(Node.node(Node.Constraint.assignable))) =>
  Tuple(types);

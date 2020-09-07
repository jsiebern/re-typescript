open Ast;
open Node;
open Parser_utils;

let generate_string_literal_list = (~runtime, ~scope, strings: array(string)) => {
  let config = Re_typescript_config.getConfig();
  let has_exotic_identifiers = config.print_language == ReScript;
  let makeVariant =
    lazy(
      v => (
        runtime,
        scope,
        Variant(
          strings
          |> CCArray.map(name =>
               {
                 VariantConstructor.name:
                   Identifier.VariantIdentifier(name, has_exotic_identifiers),
                 arguments: [||],
               }
             ),
          v,
        ),
      )
    );
  let makeUnboxed =
    lazy(
      () => {
        let name = Path.make_sub_type_name(scope.path);
        let wrapper_module_name = Ast_generator_utils.Naming.moduleName(name);
        let config = Re_typescript_config.getConfig();

        let members =
          strings
          |> CCArray.map(str => {
               let name =
                 switch (config.print_language) {
                 | ReasonMl => str |> Ast_generator_utils.Naming.to_valid_ident
                 | ReScript => str
                 };
               let type_name = Identifier.TypeName(name);
               Node.TypeDeclaration({
                 name: type_name,
                 path:
                   scope.path
                   |> Path.add(Module(wrapper_module_name))
                   |> Path.add(type_name),
                 annot: Literal(String(str)),
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
      }
    );
  switch (config.unions.string_literal) {
  | Unboxed => Lazy.force(makeUnboxed, ())
  | Variant => Lazy.force(makeVariant, `variant)
  | PolymorphicVariant => Lazy.force(makeVariant, `poly)
  };
};

let generate_number_literal_list = (~runtime, ~scope, floats: array(float)) => {
  let config = Re_typescript_config.getConfig();
  let has_exotic_identifiers = config.print_language == ReScript;
  let makeVariant =
    lazy(
      v => (
        runtime,
        scope,
        Variant(
          floats
          |> CCArray.map(num =>
               {
                 VariantConstructor.name:
                   Identifier.VariantIdentifier(
                     Printf.sprintf("%.0f", num),
                     has_exotic_identifiers,
                   ),
                 arguments: [||],
               }
             ),
          v,
        ),
      )
    );
  let makeUnboxed =
    lazy(
      () => {
        let name = Path.make_sub_type_name(scope.path);
        let wrapper_module_name = Ast_generator_utils.Naming.moduleName(name);
        let config = Re_typescript_config.getConfig();

        let members =
          floats
          |> CCArray.map(num => {
               let name = Printf.sprintf("%.0f", num);
               let name =
                 switch (config.print_language) {
                 | ReasonMl =>
                   name |> Ast_generator_utils.Naming.to_valid_ident
                 | ReScript => name
                 };
               let type_name = Identifier.TypeName(name);
               Node.TypeDeclaration({
                 name: type_name,
                 path:
                   scope.path
                   |> Path.add(Module(wrapper_module_name))
                   |> Path.add(type_name),
                 annot: Literal(Number(num)),
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
      }
    );
  switch (config.unions.number_literal) {
  | Unboxed => Lazy.force(makeUnboxed, ())
  | Variant => Lazy.force(makeVariant, `variant)
  | PolymorphicVariant => Lazy.force(makeVariant, `poly)
  };
};

let generate_mixed_literal_list =
    (
      ~runtime,
      ~scope,
      literals: array(Node.node(Node.Constraint.exactlyLiteral)),
    ) => {
  let name = Path.make_sub_type_name(scope.path);
  let wrapper_module_name = Ast_generator_utils.Naming.moduleName(name);
  let config = Re_typescript_config.getConfig();

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

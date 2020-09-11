// TODO: Implement the options from before <--- ACTUALLY: I want this AST to be as pure as possible, so all the logic will be moved to the parser.
// TODO: Implement the tests. Probably not a good idea to have JSOO run rely, but who knows
open Migrate_parsetree;
open Ast_406;
open Parsetree;
open Ast_helper;
module Util = Ast_generator_utils;

open Ast;

exception AstGeneratorException(string);

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
        (
          ~print_language,
          ~scope=defaultScope,
          nodes: array(Node.node(Node.Constraint.exactlyModule)),
        ) => {
  nodes
  |> CCArray.to_list
  |> CCList.fold_left(
       ((scope, struct_carry), node) => {
         let (scope, generated_struct) =
           generate__Node__Module(~scope, node);
         (scope, struct_carry @ generated_struct);
       },
       (scope, []),
     );
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Module
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and generate__Node__Module =
    (~scope, Module(node): Node.node(Node.Constraint.exactlyModule)) => {
  let module_name = Util.Naming.moduleName(node.name);
  let scope = {parent: Some(Module(node)), path: [|Module(module_name)|]};

  let (scope, generated_structure) =
    node.types
    |> CCArray.fold_left(
         (
           (scope, struct_carry),
           node: Node.node(Node.Constraint.moduleLevel),
         ) => {
           switch (node) {
           | Node.Module({types, _}) as module_node
               when
                 switch (CCArray.get_safe(types, 0)) {
                 | Some(Fixture(TUnboxed(_), _)) => true
                 | _ => false
                 } =>
             let (scope, item) =
               generate__Node__ModuleUnboxed(~scope, module_node);
             (scope, struct_carry @ [item]);
           | Module(_) as module_node =>
             let (scope, items) =
               generate__Node__Module(~scope, module_node);
             (scope, struct_carry @ items);
           | Fixture(_) as fixtureNode =>
             let (scope, items) = generate__Fixture(~scope, fixtureNode);
             (scope, struct_carry @ items);
           | TypeDeclaration(node) =>
             let (scope, type_declarations) =
               generate__Node__TypeDeclaration(~scope, node);
             (
               scope,
               struct_carry @ Util.wrap_type_declarations(type_declarations),
             );
           }
         },
         (scope, []),
       );

  (
    scope,
    CCString.trim(module_name) == ""
      ? generated_structure
      : [Util.make_module(module_name, generated_structure)],
  );
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Module Unboxed
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and generate__Node__ModuleUnboxed =
    (~scope, Module(node): Node.node(Node.Constraint.exactlyModule)) => {
  let module_name = Util.Naming.moduleName(node.name);
  let scope = {parent: Some(Module(node)), path: [|Module(module_name)|]};
  let params =
    switch (CCArray.get_safe(node.types, 0)) {
    | Some(Ast.Node.Fixture(TUnboxed(params, _), _)) =>
      CCList.map(fst, params)
    | Some(_)
    | None => []
    };

  let (scope, generated_stri, generated_sigi) =
    node.types
    |> CCArray.fold_left(
         ((scope, arr_stri, arr_sigi), node) => {
           let res =
             switch (node) {
             | Ast.Node.Fixture(TUnboxed(_, header_types), _) =>
               let (scope, header_types) =
                 header_types
                 |> CCArray.fold_left(
                      ((scope, nodes), node) =>
                        switch (node) {
                        | Node.TypeDeclaration(td) =>
                          let (scope, td) =
                            generate__Node__TypeDeclaration(~scope, td);
                          (scope, nodes @ td);
                        | _ =>
                          raise(
                            Invalid_argument("Should be a TypeDeclaration"),
                          )
                        },
                      (scope, []),
                    );

               let (stri, sigi) =
                 Util.Unboxed.make_unboxed_helper(~params, ~header_types, ());
               Some((scope, stri, sigi));
             | Ast.Node.TypeDeclaration({
                 annot: Literal(literal),
                 name: TypeName(name),
                 _,
               }) =>
               let value =
                 switch (literal) {
                 | String(str) =>
                   Ast_helper.(Exp.constant(Const.string(str)))
                 | Number(num) =>
                   Ast_helper.(Exp.constant(Const.int(int_of_float(num))))
                 | Boolean(b) =>
                   Ast_helper.Exp.construct(
                     Location.mknoloc(Longident.parse(b ? "true" : "false")),
                     None,
                   )
                 };

               let (stri, sigi) = Util.Unboxed.unboxed_literal(name, value);

               Some((scope, stri, sigi));
             | Ast.Node.TypeDeclaration({annot, name: TypeName(name), _}) =>
               let (scope, t) =
                 generate__Node__Assignable_CoreType(~scope, annot);

               switch (t) {
               | None => None
               | Some(t) =>
                 let (stri, sigi) =
                   Util.Unboxed.unboxed_func(~params, name, t);

                 Some((scope, stri, sigi));
               };
             | _ => None
             };

           switch (res) {
           | None => (scope, arr_stri, arr_sigi)
           | Some((scope, stri, sigi)) => (
               scope,
               CCArray.append(arr_stri, [|stri|]),
               CCArray.append(arr_sigi, [|sigi|]),
             )
           };
         },
         (scope, [||], [||]),
       );

  (
    scope,
    Str.module_(
      Mb.mk(
        Location.mknoloc(module_name),
        Mod.constraint_(
          Mod.mk(Pmod_structure(CCArray.to_list(generated_stri))),
          Mty.signature(CCArray.to_list(generated_sigi)),
        ),
      ),
    ),
  );
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Type Declarations
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and generate__Node__TypeDeclaration =
    (~scope, node: TypeDeclaration.t): (scope, list(type_declaration)) => {
  let type_name =
    Identifier.TypeName(
      switch (node.name) {
      | TypeName(str) => str
      },
    );
  let scope = {
    parent: Some(TypeDeclaration(node)),
    path: CCArray.append(scope.path, [||]),
  };

  switch (node.annot) {
  | Variant(variant_members, mode) =>
    // TODO: Use normal variant type for the time being, this need to change in several levels of complexity
    // Each member could have been assiged another literal value (or not), there could be computated values
    // Also TODO: There will be more types like this that need to be parsed here as some can't return a core_type
    switch (mode) {
    | `variant =>
      let type_kind =
        Util.make_variant_kind(
          variant_members
          |> CCArray.map(member =>
               Util.Naming.fromIdentifier(
                 switch (member.VariantConstructor.name) {
                 | VariantIdentifier(str, has_exotic_identifiers) =>
                   Identifier.VariantIdentifier(str, has_exotic_identifiers)
                 },
               )
             )
          |> CCArray.to_list,
        );
      (
        scope,
        Util.make_type_declaration_of_kind(
          ~params=
            node.params
            |> CCArray.map(Util.Naming.fromIdentifier)
            |> CCArray.to_list,
          ~aliasName=Util.Naming.fromIdentifier(type_name),
          ~kind=type_kind,
        ),
      );
    | `poly => (
        scope,
        Util.make_type_declaration(
          ~params=
            node.params
            |> CCArray.map(Util.Naming.fromIdentifier)
            |> CCArray.to_list,
          ~aliasName=Util.Naming.fromIdentifier(type_name),
          ~aliasType=
            Util.make_polymorphic(variant_members |> CCArray.to_list),
        ),
      )
    }
  | Record(parameters) =>
    let (scope, fields) =
      CCArray.fold_left(
        (
          (scope, params),
          Node.Parameter({name, is_optional, type_, named}):
            Node.node(Node.Constraint.exactlyParameter),
        ) => {
          let type_ = is_optional ? Node.Optional(type_) : type_;
          let (scope, t) =
            generate__Node__Assignable_CoreType(~scope, type_);
          let name_original = Util.Naming.unwrap(name);
          let name_reason = Util.Naming.fromIdentifier(name);
          let param = (
            name_reason,
            t |> CCOpt.value(~default=Util.make_type_constraint("any")),
            name_reason != name_original
              ? [Util.make_bs_as_attribute(name_original)] : [],
          );
          (scope, CCArray.append(params, [|param|]));
        },
        (scope, [||]),
        parameters,
      );
    let record_kind = Util.make_record_kind(fields);
    (
      scope,
      Util.make_type_declaration_of_kind(
        ~params=
          node.params
          |> CCArray.map(Util.Naming.fromIdentifier)
          |> CCArray.to_list,
        ~aliasName=Util.Naming.fromIdentifier(type_name),
        ~kind=record_kind,
      ),
    );

  | annot =>
    let (scope, annotated_type) =
      generate__Node__Assignable_CoreType(~scope, annot);

    (
      scope,
      switch (annotated_type) {
      | None => []
      | Some(annotated_type) =>
        Util.make_type_declaration(
          ~params=
            node.params
            |> CCArray.map(Util.Naming.fromIdentifier)
            |> CCArray.to_list,
          ~aliasName=Util.Naming.fromIdentifier(type_name),
          ~aliasType=annotated_type,
        )
      },
    );
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
  | Nullable(inner) =>
    let (scope, inner) = generate__Node__Assignable_CoreType(~scope, inner);
    (scope, inner |> CCOpt.map(Util.make_nullable_of));
  | Optional(inner) =>
    let (scope, inner) = generate__Node__Assignable_CoreType(~scope, inner);
    (scope, inner |> CCOpt.map(Util.make_optional_of));
  | Array(inner) =>
    let (scope, inner) = generate__Node__Assignable_CoreType(~scope, inner);
    (scope, inner |> CCOpt.map(Util.make_array_of));
  | Tuple(inner) =>
    let (scope, types) =
      inner
      |> CCArray.fold_left(
           ((scope, types), t) => {
             let (scope, t) = generate__Node__Assignable_CoreType(~scope, t);
             (
               scope,
               switch (t) {
               | Some(t) => CCArray.append(types, [|t|])
               | None => types
               },
             );
           },
           (scope, [||]),
         );
    (
      scope,
      CCArray.length(types) > 0 ? Some(Util.make_tuple_of(types)) : None,
    );
  | Function({return_type, parameters}) =>
    let (scope, return_type) =
      generate__Node__Assignable_CoreType(~scope, return_type);
    let (scope, parameters) =
      CCArray.fold_left(
        (
          (scope, params),
          Node.Parameter({name, is_optional, type_, named}):
            Node.node(Node.Constraint.exactlyParameter),
        ) => {
          let (scope, t) =
            generate__Node__Assignable_CoreType(~scope, type_);
          let param = (
            switch (is_optional, named) {
            | (false, true)
            | (true, _) => Some(Util.Naming.fromIdentifier(name))
            | (false, false) => None
            },
            is_optional,
            t |> CCOpt.value(~default=Util.make_type_constraint("any")),
          );
          (scope, CCArray.append([|param|], params));
        },
        (scope, [||]),
        parameters,
      );
    let parameters = parameters |> CCArray.rev;

    let parameter_count = CCArray.length(parameters);
    let parameters =
      parameter_count > 0
        ? switch (CCArray.get(parameters, parameter_count - 1)) {
          | (_, true, _) =>
            CCArray.append(
              parameters,
              [|(None, false, Util.make_type_constraint("unit"))|],
            )
          | _ => parameters
          }
        : parameters;

    (
      scope,
      Some(
        Util.make_function_type(
          parameter_count > 0
            ? parameters
            : [|(None, false, Util.make_type_constraint("unit"))|],
          return_type
          |> CCOpt.value(~default=Util.make_type_constraint("any")),
        ),
      ),
    );
  | Record(_) =>
    raise(
      Failure(
        "Error: Records can not be defined inline, they have to be extracted into their own TypeDeclaration",
      ),
    )
  | Reference({target, params}) =>
    let generated_params =
      params
      |> CCList.map(((_, param)) =>
           generate__Node__Assignable_CoreType(~scope, param)
         )
      |> CCList.map(snd)
      |> CCList.keep_some;
    (
      scope,
      Some(
        Util.make_type_constraint(
          ~inner=generated_params,
          Util.Naming.full_identifier_of_path(target),
        ),
      ),
    );
  | GenericReference(Identifier.TypeParameter(param)) => (
      scope,
      Some(Typ.var(param)),
    )
  | SafeDict(t) =>
    let (scope, t) = generate__Node__Assignable_CoreType(~scope, t);
    (
      scope,
      t
      |> CCOpt.map(inner =>
           Util.make_type_constraint(~inner=[inner], "Js.Dict.t")
         ),
    );
  | Literal(literal) =>
    // Literals cannot really be displayed in reason, so we just use the primitive here
    // TODO: Actually think about an inline type here!
    switch (literal) {
    | String(_) => generate__Node__Assignable_CoreType(~scope, Basic(String))
    | Number(_) => generate__Node__Assignable_CoreType(~scope, Basic(Number))
    | Boolean(_) =>
      generate__Node__Assignable_CoreType(~scope, Basic(Boolean))
    }
  | Variant(variant_members, `poly) => (
      scope,
      Some(Util.make_polymorphic(variant_members |> CCArray.to_list)),
    )
  | other =>
    raise(
      AstGeneratorException(
        Printf.sprintf(
          "> Error: Type '%s' should not be handled as an Assignable_CoreType",
          Pp.ast_node(other),
        ),
      ),
    )
  };
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Generate Fixtures
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and generate__Fixture =
    (~scope, Fixture(fixture, _): Node.node(Node.Constraint.exactlyFixture)) => {
  switch (fixture) {
  | AnyUnboxed => (scope, Util.make_any_helper_unboxed())
  | TUnboxed(params, header_types) =>
    let (scope, header_types) =
      header_types
      |> CCArray.fold_left(
           ((scope, nodes), node) =>
             switch (node) {
             | Node.TypeDeclaration(td) =>
               let (scope, td) = generate__Node__TypeDeclaration(~scope, td);
               (scope, nodes @ td);
             | _ => raise(Invalid_argument("Should be a TypeDeclaration"))
             },
           (scope, []),
         );
    (
      scope,
      [
        fst(
          Util.Unboxed.make_unboxed_helper(
            ~header_types,
            ~params=CCList.map(fst, params),
            (),
          ),
        ),
      ],
    );
  };
};

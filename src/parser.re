// TODO: Eventually define an interface so that multiple backends can be built
// ALso: Plugin system should be solved here as well, before the AST is built
// TODO: Add more ways in which elements can obtain references (like type extraction)

// Could use a similar system to graphql-ppx (see https://github.com/reasonml-community/graphql-ppx/blob/master/src/base/validations.re for reference)
open Ast;
open Node;

// ---------------------------------------------------------------- ´--------------------------
// ------------------------------------------------------------------------------------------
// --- Utility types
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
open Parser_utils;
module PlannedExceptions = {
  exception LoneFunctionSignature(Ts_nodes.FunctionDeclaration.t);
};

// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Entry / Preparation
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
let rec parse__Entry =
        (~warnings=true, ~source_files: array(Ts_morph.SourceFile.t)) => {
  let runtime = {
    root_modules: [||],
    node_count: 0,
    fully_qualified_added: [],
    warnings,
  };
  let runtime =
    source_files
    |> CCArray.fold_left(
         (runtime, source_file) => {
           let scope = {
             root_declarations: [||],
             source_file: None,
             current_declaration: None,
             current_declared_params: [||],
             path: [||],
             has_any: false,
             refs: Hashtbl.create(10),
             context_params: [],
             context_args: [],
             context_args_retained: [],
           };

           let source_file =
             source_file
             |> Ts_morph.SourceFile.castToNode
             |> Ts_nodes.Generic.fromMorphNode
             |> Ts_nodes.SourceFile.fromGeneric;
           let (runtime, scope, t) =
             parse__Node__SourceFile(~runtime, ~scope, source_file);
           let (runtime, scope, t) =
             Parser_rules_post.run(~runtime, ~scope, t);
           runtime |> Runtime.add_root_module(t);
         },
         runtime,
       );
  runtime.root_modules;
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Generic Node ("Hub")
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__Generic =
    (~runtime, ~scope, node: Ts_nodes.Generic.t)
    : (runtime, scope, Node.node(Node.Constraint.any)) => {
  let is_sub =
    Path.hd(scope.path) |> CCOpt.map_or(~default=false, Path.is_sub);
  let identifiedNode = Ts_nodes_util.identifyGenericNode(node);

  switch (identifiedNode) {
  | StringKeyword(_)
  | NumberKeyword(_)
  | NeverKeyword(_)
  | NullKeyword(_)
  | ObjectKeyword(_)
  | UndefinedKeyword(_)
  | BooleanKeyword(_)
  | VoidKeyword(_)
  | AnyKeyword(_) => parse__Node__Basic(~runtime, ~scope, identifiedNode)
  | ArrayType(_) => parse__Node__Array(~runtime, ~scope, identifiedNode)
  | TupleType(_) => parse__Node__Tuple(~runtime, ~scope, identifiedNode)
  | IntersectionType(_) =>
    parse__Node__Intersection(~runtime, ~scope, identifiedNode)
  | TypeReference(_) =>
    parse__Node_TypeReference(~runtime, ~scope, identifiedNode)
  | TypeLiteral(_)
  | UnionType(_) when is_sub =>
    parse__Node__Generic__WrapSubNode(~runtime, ~scope, node)
  | TypeLiteral(typeLiteral) =>
    parse__Node__TypeLiteral(~runtime, ~scope, typeLiteral)
  | UnionType(union) => parse__Node__UnionType(~runtime, ~scope, union)
  | FunctionType(func_type) =>
    parse__Node__FunctionType(~runtime, ~scope, func_type)
  | IndexedAccessType(ind_acc_type) =>
    parse__Node__IndexedAccessType(~runtime, ~scope, ind_acc_type)
  | MethodSignature(_) =>
    let (runtime, scope, (_, _, t)) =
      parse__Node__SignatureLike(~runtime, ~scope, identifiedNode);
    (runtime, scope, t |> Node.Escape.toAny);
  | LiteralType(_) =>
    parse__Node__LiteralType(~runtime, ~scope, identifiedNode)
  | MappedType(_) => parse__Node__MappedType(~runtime, ~scope, identifiedNode)
  | TypeOperator(_) =>
    parse__Node__TypeOperator(~runtime, ~scope, identifiedNode)
  | ParenthesizedType(pNode) =>
    parse__Node__Generic(~runtime, ~scope, pNode#getTypeNode())
  | _ =>
    Console.log(
      Ast_generator_utils.Naming.full_identifier_of_path(scope.path),
    );
    raise(
      Exceptions.UnexpectedAtThisPoint(
        Printf.sprintf(
          "> Cannot process generic node '%s'",
          node#getKindName(),
        ),
      ),
    );
  };
}
and parse__Node__Generic_assignable =
    (~runtime, ~scope, node: Ts_nodes.Generic.t)
    : (runtime, scope, Node.node(Node.Constraint.assignable)) => {
  let (runtime, scope, node) = parse__Node__Generic(~runtime, ~scope, node);
  (runtime, scope, node |> Node.Escape.toAssignable);
}
and parse__Node__Resolved__WrapSubNode =
    (
      ~before=?,
      ~runtime,
      ~scope,
      node: Node.node(Node.Constraint.assignable),
    ) => {
  let name = Path.make_sub_type_name(scope.path);
  let base_path = scope.path;
  let type_name = Identifier.TypeName(name);
  let scope = scope |> Scope.add_to_path(type_name);
  let qualified_path_to_type =
    Path.make_current_scope(scope.path) |> Path.add(type_name);
  let scoped_path = Pp.path(qualified_path_to_type);

  let (runtime, scope) =
    !
      CCList.mem(
        ~eq=CCString.equal,
        scoped_path,
        runtime.fully_qualified_added,
      )
      ? {
        let wrapped_type_declaration =
          TypeDeclaration({
            path: qualified_path_to_type,
            name: type_name,
            annot: node,
            params: scope.current_declared_params,
          });
        let runtime = {
          ...runtime,
          fully_qualified_added: [
            scoped_path,
            ...runtime.fully_qualified_added,
          ],
        };
        (
          runtime,
          scope
          |> Scope.add_root_declaration(~before?, wrapped_type_declaration),
        );
      }
      : (runtime, scope);

  let scope =
    scope
    |> Scope.add_ref(
         qualified_path_to_type,
         Path.strip_current_sub_path(base_path),
       );
  (
    runtime,
    scope,
    Reference({
      target: [|type_name|],
      params:
        scope.current_declared_params
        |> CCArray.map(
             fun
             | Identifier.TypeParameter(n) as i => (n, GenericReference(i))
             | other =>
               raise(
                 Exceptions.UnexpectedAtThisPoint(
                   Printf.sprintf(
                     "Not a valid type param ident: %s",
                     Pp.identifier(other),
                   ),
                 ),
               ),
           )
        |> CCArray.to_list,
    }),
  );
}
and parse__Node__Generic__WrapSubNode =
    (~runtime, ~scope, node: Ts_nodes.Generic.t) => {
  let name = Path.make_sub_type_name(scope.path);
  let base_path = scope.path;
  let type_name = Identifier.TypeName(name);
  let qualified_path_to_type =
    Path.make_current_scope(scope.path) |> Path.add(type_name);
  let scope = scope |> Scope.add_to_path(type_name);

  // TODO:
  // Even though we're looking for a normalized scoped path here to
  // avoid duplicates, this isn't ideal. Typescript declarations could
  // be all over the place, we need to pre-render dependencies in the tree if possible
  // When that is done, there needs to be a check like this for main types as well
  let scoped_path =
    Path.make_current_scope(scope.path) |> Path.add(type_name);
  let scoped_path_str = Pp.path(scoped_path);

  let (runtime, scope, wrapped_type) =
    parse__Node__Generic_assignable(~runtime, ~scope, node);
  let wrapped_params = finalize_generic_reference(wrapped_type);
  switch (wrapped_type) {
  | Nullable(Basic(_))
  | Optional(Basic(_))
  | Array(Basic(_))
  | Basic(_) => (runtime, scope, wrapped_type |> Node.Escape.toAny)
  | _ =>
    let (runtime, scope) =
      !
        CCList.mem(
          ~eq=CCString.equal,
          scoped_path_str,
          runtime.fully_qualified_added,
        )
        ? {
          let wrapped_type_declaration =
            TypeDeclaration({
              path: qualified_path_to_type,
              name: type_name,
              annot: wrapped_type,
              params: wrapped_params,
            });
          let runtime = {
            ...runtime,
            fully_qualified_added: [
              scoped_path_str,
              ...runtime.fully_qualified_added,
            ],
          };
          (
            runtime,
            scope |> Scope.add_root_declaration(wrapped_type_declaration),
          );
        }
        : (runtime, scope);

    let scope =
      scope
      |> Scope.add_ref(
           qualified_path_to_type,
           Path.strip_current_sub_path(base_path),
         );
    (
      runtime,
      scope,
      Reference({
        target: [|type_name|],
        params:
          wrapped_params
          |> CCArray.map(
               fun
               | Identifier.TypeParameter(n) as i => (n, GenericReference(i))
               | other =>
                 raise(
                   Exceptions.UnexpectedAtThisPoint(
                     Printf.sprintf(
                       "Not a valid type param ident: %s",
                       Pp.identifier(other),
                     ),
                   ),
                 ),
             )
          |> CCArray.to_list,
      }),
    );
  };
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Source File
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__SourceFile =
    (~runtime, ~scope, node: Ts_nodes.SourceFile.t)
    : (runtime, scope, Node.node(Node.Constraint.atLeastModule('poly))) => {
  let config = Re_typescript_config.getConfig();
  let source_file_name = node#getBaseNameWithoutExtension();
  let scope = {...scope, source_file: Some(node)};
  let children_to_traverse: array(Ts_nodes.Generic.t) =
    config.types == ExportsOnly
      ? node#getExportedDeclarations() : node#getStatements();

  let (runtime: runtime, scope: scope) =
    CCArray.fold_left(
      ((runtime, scope), node) => {
        let scope = scope |> Scope.replace_path(Module(source_file_name));
        let (runtime, scope, res) =
          parse__Node__Declaration(~runtime, ~scope, node);
        let scope = scope |> Scope.add_root_declaration(res);
        (runtime, scope);
      },
      (runtime, scope),
      children_to_traverse,
    );

  let (runtime, scope, prependFixtures) =
    parse__Fixtures__ForSourceFile(~runtime, ~scope);
  (
    runtime,
    scope,
    Module({
      name: config.top_level_module ? source_file_name : "",
      path: node#getFilePath(),
      types: CCArray.append(prependFixtures, scope.root_declarations),
    }),
  );
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- TypeDeclaration
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and create__ContextFromNode =
    (
      ~clear=true,
      ~runtime,
      ~type_arguments=[||],
      ~node: Ts_nodes.Generic.t,
      scope: scope,
    ) => {
  let scope =
    clear ? scope |> Context.clear_params |> Context.clear_args : scope;
  let base_path = scope.path;
  let (runtime, scope, params) =
    Ts_morph_util.typeGuards##isParameteredNode(
      Ts_nodes.Generic.t_to_js(node),
    )
      ? {
        let parameters =
          Ts_nodes.TypeParametered.fromGeneric(node)#getTypeParameters();
        let (runtime, scope, params) =
          parameters
          |> CCArray.fold_left(
               ((runtime, scope, params), param) => {
                 let scope = scope |> Scope.replace_path_arr(base_path);
                 let name = param#getName();
                 // IMPORTANT: Maybe revert this change later, but for now disabling the render of the default params to prevent superfluous type definitions like union modules that only add noise.
                 //  let (runtime, scope, default) =
                 //    param#getDefault()
                 //    |> CCOpt.map(paramNode => {
                 //         let (runtime, scope, res) =
                 //           parse__Node__Generic_assignable(
                 //             ~runtime,
                 //             ~scope,
                 //             paramNode,
                 //           );
                 //         (runtime, scope, Some(res));
                 //       })
                 //    |> CCOpt.value(~default=(runtime, scope, None));
                 let scope = scope |> Context.add_param(name, None);

                 (
                   runtime,
                   scope,
                   CCArray.append(params, [|(name, None)|]),
                 );
               },
               (runtime, scope, [||]),
             );
        (runtime, scope, params);
      }
      : (runtime, scope, [||]);
  let scope = scope |> Scope.replace_path_arr(base_path);

  let (runtime, scope) =
    params
    |> CCArray.foldi(
         ((runtime, scope), i, (key, default)) => {
           let scope = scope |> Scope.replace_path_arr(base_path);
           let (runtime, scope, arg) =
             CCArray.get_safe(type_arguments, i)
             |> CCOpt.map(arg => {
                  let scope =
                    scope |> Scope.add_to_path(Identifier.SubName(key));
                  let (runtime, scope, res) =
                    parse__Node__Generic_assignable(~runtime, ~scope, arg);
                  (runtime, scope, Some(res));
                })
             |> CCOpt.value(~default=(runtime, scope, None));
           let scope =
             switch (arg, default) {
             | (Some(arg), _) => scope |> Context.add_arg(key, arg)
             | (None, Some(_)) => scope
             // TYPE PARAMS: Just omit all defaults from directly being applied. Should be the safest bet
             //  | (None, Some(Record(_))) =>
             // We can filter out records here for them to not be inlined. This is far from ideal. In the end it might make the most sense to completely omit default values in the original type
             //  scope
             //  | (None, Some(default)) =>
             //    scope |> Context.add_arg(key, default)
             | (None, None) => scope
             };

           (runtime, scope);
         },
         (runtime, scope),
       );
  (runtime, scope);
}
and parse__Node__Declaration:
  (~runtime: runtime, ~scope: scope, Ts_nodes.Generic.t) =>
  (runtime, scope, Node.node(Node.Constraint.moduleLevel)) =
  (~runtime, ~scope, node: Ts_nodes.Generic.t) => {
    // Populate scope with parameters
    // TODO: Possibly remove
    // let curry_context = create__ContextFromNode(~runtime, ~node);
    // let set_context = lazy(scope => curry_context(scope));
    let scope = scope |> Context.clear_args |> Context.clear_params;

    let identified_node = Ts_nodes_util.identifyNode(node);
    switch (identified_node) {
    // | ClassDeclaration
    | InterfaceDeclaration(if_declaration) =>
      try(parse__Node__InterfaceDeclaration(~runtime, ~scope, if_declaration)) {
      // ------------
      // TODO
      // Defeinitely move this from here, also not sure if exception is the way to go
      // ------------
      | PlannedExceptions.LoneFunctionSignature(fn_declaration) =>
        let name =
          if_declaration#getName()
          |> CCOpt.value(~default=if_declaration#getNameNode()#getText());
        let type_name = Identifier.TypeName(name);
        let (scope, retain) =
          scope |> Scope.retain_path(scope.path |> Path.add(type_name));
        let (runtime, scope, wrapped_type) =
          parse__Node__FunctionType(~runtime, ~scope, fn_declaration);

        let params =
          fn_declaration#getTypeParameters()
          |> CCArray.map(n => Identifier.TypeParameter(n#getName()));
        let wrapped_type_declaration =
          TypeDeclaration({
            path: scope.path,
            name: type_name,
            annot: wrapped_type |> Node.Escape.toAssignable,
            params,
          });
        let scope = scope |> retain;
        (runtime, scope, wrapped_type_declaration);
      | other => raise(other)
      }
    | EnumDeclaration(enum) =>
      parse__Node__EnumDeclaration(~runtime, ~scope, enum)
    | FunctionDeclaration(fn_declaration) =>
      parse__Node__FunctionDeclaration(~runtime, ~scope, fn_declaration)
    // | VariableDeclaration
    | TypeAliasDeclaration(typeAlias) =>
      parse__Node__TypeAliasDeclaration(~runtime, ~scope, typeAlias)
    | ModuleDeclaration(nsDeclaration)
    | NamespaceDeclaration(nsDeclaration) =>
      parse__Node__NamespaceDeclaration(~runtime, ~scope, nsDeclaration)
    // | Expression
    | _ =>
      Console.error("> " ++ node#getKindName());
      raise(Failure("Only declarations allowed here"));
    };
  }
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- NamespaceDeclaration
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__NamespaceDeclaration =
    (~runtime, ~scope, node: Ts_nodes.NamespaceDeclaration.t) => {
  let config = Re_typescript_config.getConfig();
  let children_to_traverse: array(Ts_nodes.Generic.t) =
    config.types == ExportsOnly
      ? node#getExportedDeclarations() : node#getStatements();

  let scope = scope |> Scope.add_to_path(Identifier.Module(node#getName()));

  let base_path = scope.path;
  let (runtime, scope, types) =
    children_to_traverse
    |> CCArray.fold_left(
         ((runtime, scope, types), node) => {
           let scope = scope |> Scope.replace_path_arr(base_path);
           let (runtime, scope, t) =
             parse__Node__Declaration(~runtime, ~scope, node);
           (runtime, scope, CCArray.append(types, [|t|]));
         },
         (runtime, scope, [||]),
       );
  let scope = scope |> Scope.replace_path_arr(base_path);

  (
    runtime,
    scope,
    Module({
      name: node#getName(),
      path: Ast_generator_utils.Naming.full_identifier_of_path(scope.path),
      types,
    }),
  );
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- TypeOperator
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__TypeOperator = (~runtime, ~scope, node: Ts_nodes.nodeKind) => {
  switch (node) {
  | TypeOperator(type_operator) =>
    switch (
      type_operator#getType()
      |> CCOpt.flat_map(
           Parser_resolvers.try_to_resolve_type(~runtime, ~scope),
         )
    ) {
    | Some(res) => res |> parse__map(Node.Escape.toAny)
    | None =>
      // Resolving the type failed, let's try with the child node as a last resort
      // First, determine the operator
      let kind =
        (type_operator#compilerNode |> Ts_raw.TypeOperatorNode.fromNode)#
          operator
        |> Ojs.int_of_js;
      switch (Ts_morph_util.getSyntaxKindName(kind)) {
      | "KeyOfKeyword" =>
        switch (type_operator#getLastChild()) {
        | Some(child) =>
          parse__Node__Generic_assignable(~runtime, ~scope, child)
          |> parse__map(Node.Escape.toAny)
        | None =>
          raise(
            Exceptions.UnexpectedAtThisPoint(
              "Type operator node resolution expects the TypeOperatorNode to have at least one child",
            ),
          )
        }
      | "UniqueKeyword" =>
        raise(
          Exceptions.FeatureMissing(
            "UniqueKeyword",
            "Don't know what to do with this type operator yet",
          ),
        )
      | "ReadonlyKeyword" =>
        raise(
          Exceptions.FeatureMissing(
            "ReadonlyKeyword",
            "Don't know what to do with this type operator yet",
          ),
        )
      | other =>
        raise(
          Exceptions.UnexpectedAtThisPoint(
            Printf.sprintf("Unexpected type operator: %s", other),
          ),
        )
      };
    }
  | _ => raise(Exceptions.UnexpectedAtThisPoint("Expected TypeOperator"))
  };
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- LiteralType
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__LiteralType = (~runtime, ~scope, node: Ts_nodes.nodeKind) => {
  switch (node) {
  | LiteralType(literal) =>
    let inner = literal#getLiteral();
    (
      runtime,
      scope,
      switch (inner |> Ts_nodes_util.identifyGenericNode) {
      | BooleanLiteral(b) => Literal(Boolean(b#getLiteralValue()))
      | TrueKeyword(_) => Literal(Boolean(true))
      | FalseKeyword(_) => Literal(Boolean(false))
      | StringLiteral(s) => Literal(String(s#getLiteralValue()))
      | NumericLiteral(n) => Literal(Number(n#getLiteralValue()))
      | NullLiteral(_) => Basic(Null)
      | _ =>
        raise(
          Exceptions.FeatureMissing(
            "LiteralType(" ++ inner#getKindName() ++ ")",
            inner#getText(),
          ),
        )
      },
    );
  | _ => raise(Exceptions.UnexpectedAtThisPoint("Expected LiteralType"))
  };
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- MappedType
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__MappedType__FromResolved =
    (
      ~modifier: mapped_modifier,
      ~parse__Value:
         (
           ~runtime: runtime,
           ~scope: scope,
           ~subname: string=?,
           Node.node(Node.Constraint.assignable)
         ) =>
         (runtime, scope, Node.node(Node.Constraint.assignable)),
      ~runtime,
      ~scope,
      resolved_node: Node.node(Node.Constraint.assignable),
    ) => {
  let resolved_node = resolved_node |> Node.Escape.toAny;
  let (runtime, scope, resolved_node) =
    Parser_resolvers.follow_references_from_resolved(
      ~runtime,
      ~scope,
      resolved_node,
    )
    |> CCOpt.value(~default=(runtime, scope, resolved_node));
  let optional = modifier == `to_optional;
  let keys =
    switch (resolved_node) {
    | Variant(idents, _) =>
      idents
      |> CCArray.map(i => i.VariantConstructor.name)
      |> CCArray.map(Ast_generator_utils.Naming.unwrap)
      |> CCArray.map(i => (i, optional))
    | Literal(String(str)) => [|(str, optional)|]
    | Literal(Number(n)) => [|(Printf.sprintf("%.0f", n), optional)|]
    | Record(fields) =>
      fields
      |> CCArray.mapi((i, field) =>
           switch (field) {
           | Parameter({name: Identifier.PropertyName(name), is_optional, _}) => (
               name,
               switch (modifier) {
               | `none => is_optional
               | _ => optional
               },
             )
           | _ => (Printf.sprintf("%i", i), optional)
           }
         )
    | other =>
      raise(
        Exceptions.FeatureMissing(
          Pp.ast_node(other),
          "Could not resolve mapped type",
        ),
      )
    };

  let (runtime, scope, fields) =
    keys
    |> CCArray.fold_left(
         ((runtime, scope, fields), (key, is_optional)) => {
           let (runtime, scope, value_resolved) =
             parse__Value(
               ~runtime,
               ~scope,
               ~subname=key,
               Literal(String(key)),
             );

           (
             runtime,
             scope,
             CCArray.append(
               fields,
               [|
                 Parameter({
                   name: Identifier.PropertyName(key),
                   is_optional,
                   type_: value_resolved,
                   named: true,
                 }),
               |],
             ),
           );
         },
         (runtime, scope, [||]),
       );

  parse__Node__Resolved__WrapSubNode(~runtime, ~scope, Record(fields));
}
and parse__Node__MappedType = (~runtime, ~scope, node: Ts_nodes.nodeKind) => {
  switch (node) {
  // Have this additional check to exclude a mapped type when not all of it's parameters are filled
  | MappedType(_) when !Context.args_match_params(scope) => (
      runtime,
      scope,
      Basic(Never),
    )
  | MappedType(mapped_type) =>
    // TODO: This function part needs to be cleaned up a lot
    // Massively repeating sections for the subkeys, should be abstracted into it's own file maybe even

    // We need to structurally analyse this type as ts-morph does not have a proper wrapper for the MappedType
    let children = mapped_type#getChildren();
    let type_parameter =
      children
      |> CCArray.find_map(node =>
           node#getKindName() == "TypeParameter"
             ? switch (Ts_nodes_util.identifyGenericNode(node)) {
               | TypeParameter(tp) => Some(tp)
               | _ => None
               }
             : None
         )
      |> CCOpt.get_exn;
    let parameter_name = type_parameter#getName();

    let raw_mapped_type_node =
      mapped_type#compilerNode |> Ts_raw.MappedTypeNode.fromNode;
    let modifier: mapped_modifier =
      switch (raw_mapped_type_node#questionToken) {
      | None => `none
      | Some(raw_node) =>
        switch (
          raw_node#kind |> Ojs.int_of_js |> Ts_morph_util.getSyntaxKindName
        ) {
        | "QuestionToken"
        | "PlusToken" => `to_optional
        | "MinusToken" => `to_required
        | _ => `none
        }
      };

    let value_node =
      children
      |> CCArray.find_map_i((i, node) =>
           node#getKindName() == "ColonToken"
             ? CCArray.get_safe(children, i + 1) : None
         )
      |> CCOpt.get_exn;
    // We analyze the value node for occurences of our type_parameter reference
    // If it doesn't exist, we don't need to assign separate sub_names
    let is_value_clean =
      !Parser_utils.node_contains_type_parameter(parameter_name, value_node);

    let type_constraint = type_parameter#getConstraint() |> CCOpt.get_exn;
    let key_list_type =
      (
        type_constraint
        |> Parser_resolvers.raw_remove_references
        |> CCOpt.get_exn
        |> Ts_nodes.WithGetType.fromGeneric
      )#
        getType()
      |> CCOpt.get_exn;

    let parse__Value =
        (~value_node, ~is_value_clean, ~runtime, ~scope, ~subname=?, key_type) => {
      let (scope, restore) =
        Scope.retain_path(
          scope.path
          |> Path.add(
               Identifier.SubName(
                 is_value_clean ? "t" : CCOpt.value(~default="t", subname),
               ),
             ),
          scope,
        );
      let scope = scope |> Context.add_param(parameter_name, None);
      let (scope, restore_retained_args) =
        scope |> Context.inject_retained_arg_tpl((parameter_name, key_type));

      let (runtime, scope, value_resolved) =
        parse__Node__Generic_assignable(~runtime, ~scope, value_node);

      let scope = restore_retained_args(scope);
      let scope = restore(scope);
      // If the resolved value represents a type parameter, we should try to further resolve that with an argument
      Parser_utils.resolved_node_replace_type_parameter(
        ~runtime,
        ~scope,
        value_resolved |> Node.Escape.toAny,
      )
      |> parse__map(Node.Escape.toAssignable);
    };
    let parse__Value = parse__Value(~value_node, ~is_value_clean);

    // Extract Properties from union
    if (key_list_type#isTypeParameter()) {
      let paramName = key_list_type#getText();
      let arg = scope |> Context.get_arg(paramName);
      switch (
        arg
        |> CCOpt.flat_map(arg =>
             Parser_resolvers.follow_references_from_resolved(
               ~runtime,
               ~scope,
               arg |> Node.Escape.toAny,
             )
           )
      ) {
      | None =>
        Console.warn(
          Printf.sprintf(
            "Could not resolve type parameter %s in resolved type MappedType",
            paramName,
          ),
        );
        (runtime, scope, Basic(Never));
      | Some((runtime, scope, final_arg)) =>
        parse__Node__MappedType__FromResolved(
          ~modifier,
          ~parse__Value,
          ~runtime,
          ~scope,
          final_arg |> Node.Escape.toAssignable,
        )
      };
    } else if (key_list_type#isUnion()) {
      let keys =
        key_list_type#getUnionTypes()
        |> CCArray.filter(t => t#isLiteral())
        |> CCArray.map(t =>
             t#getText() |> CCString.replace(~sub="\"", ~by="")
           );

      let (runtime, scope, fields) =
        keys
        |> CCArray.fold_left(
             ((runtime, scope, fields), key) => {
               let (runtime, scope, value_resolved) =
                 parse__Value(
                   ~runtime,
                   ~scope,
                   ~subname=key,
                   Literal(String(key)),
                 );

               (
                 runtime,
                 scope,
                 CCArray.append(
                   fields,
                   [|
                     Parameter({
                       name: Identifier.PropertyName(key),
                       is_optional: modifier == `to_optional,
                       type_: value_resolved,
                       named: true,
                     }),
                   |],
                 ),
               );
             },
             (runtime, scope, [||]),
           );

      parse__Node__Resolved__WrapSubNode(~runtime, ~scope, Record(fields));
    } else if (key_list_type#isStringLiteral()) {
      let keys = [|
        key_list_type#getText() |> CCString.replace(~sub="\"", ~by=""),
      |];

      let (runtime, scope, fields) =
        keys
        |> CCArray.fold_left(
             ((runtime, scope, fields), key) => {
               let (runtime, scope, value_resolved) =
                 parse__Value(
                   ~runtime,
                   ~scope,
                   ~subname=key,
                   Literal(String(key)),
                 );

               (
                 runtime,
                 scope,
                 CCArray.append(
                   fields,
                   [|
                     Parameter({
                       name: Identifier.PropertyName(key),
                       is_optional: modifier == `to_optional,
                       type_: value_resolved,
                       named: true,
                     }),
                   |],
                 ),
               );
             },
             (runtime, scope, [||]),
           );

      parse__Node__Resolved__WrapSubNode(~runtime, ~scope, Record(fields));
    } else if (key_list_type#isNumberLiteral()) {
      let keys = [|
        key_list_type#getText() |> CCString.replace(~sub="\"", ~by=""),
      |];

      let (runtime, scope, fields) =
        keys
        |> CCArray.fold_left(
             ((runtime, scope, fields), key) => {
               let (runtime, scope, value_resolved) =
                 parse__Value(
                   ~runtime,
                   ~scope,
                   ~subname=key,
                   Literal(String(key)),
                 );

               (
                 runtime,
                 scope,
                 CCArray.append(
                   fields,
                   [|
                     Parameter({
                       name: Identifier.PropertyName(key),
                       is_optional: modifier == `to_optional,
                       type_: value_resolved,
                       named: true,
                     }),
                   |],
                 ),
               );
             },
             (runtime, scope, [||]),
           );

      parse__Node__Resolved__WrapSubNode(~runtime, ~scope, Record(fields));
    } else if (key_list_type#isString()) {
      let (runtime, scope, resolved_value) =
        parse__Value(~runtime, ~scope, ~subname="t", Basic(String));
      (runtime, scope, SafeDict(resolved_value));
    } else if (key_list_type#isNumber()) {
      let (runtime, scope, resolved_value) =
        parse__Value(~runtime, ~scope, ~subname="t", Basic(String));
      (runtime, scope, Array(resolved_value));
    } else if (key_list_type#isEnum()) {
      let symbol = key_list_type#getSymbol() |> CCOpt.get_exn;
      let value_declaration = symbol#getValueDeclaration() |> CCOpt.get_exn;
      let members =
        Ts_nodes.EnumDeclaration.fromGeneric(value_declaration)#getMembers();
      let keys = members |> CCArray.map(member => member#getName());

      let (runtime, scope, fields) =
        keys
        |> CCArray.fold_left(
             ((runtime, scope, fields), key) => {
               let (runtime, scope, value_resolved) =
                 parse__Value(
                   ~runtime,
                   ~scope,
                   ~subname=key,
                   Literal(String(key)),
                 );

               (
                 runtime,
                 scope,
                 CCArray.append(
                   fields,
                   [|
                     Parameter({
                       name: Identifier.PropertyName(key),
                       is_optional: modifier == `to_optional,
                       type_: value_resolved,
                       named: true,
                     }),
                   |],
                 ),
               );
             },
             (runtime, scope, [||]),
           );

      parse__Node__Resolved__WrapSubNode(~runtime, ~scope, Record(fields));
    } else {
      // Analyzing the type has failed, let's try to resolve the type constraint directly via the node
      let (runtime, scope, maybe_useful_t) =
        parse__Node__Generic_assignable(~runtime, ~scope, type_constraint);
      parse__Node__MappedType__FromResolved(
        ~modifier,
        ~parse__Value,
        ~runtime,
        ~scope,
        maybe_useful_t,
      );
    };
  | _ => raise(Exceptions.UnexpectedAtThisPoint("Expected mapped type"))
  };
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- IntersectionType
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__Intersection = (~runtime, ~scope, node: Ts_nodes.nodeKind) => {
  switch (node) {
  | IntersectionType(is_type) =>
    let (runtime, scope, sub_nodes) =
      is_type#getTypeNodes() |> parse__ArrayOfGenerics(~runtime, ~scope);
    (
      runtime,
      scope,
      Tuple(sub_nodes |> CCArray.map(Node.Escape.toAssignable)),
    );
  | _ =>
    raise(Exceptions.UnexpectedAtThisPoint("Expected intersection type"))
  };
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- UnionType
//
// As the AST should stay as pure as possible, we need to construct the different versions of a union here directly
// Important for the union type:
// Whenever there is a Fixture(TUnboxed) present as the first child of a module, it will be printed as an unboxed module type
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__UnionType__Nodes:
  (
    ~runtime: runtime,
    ~scope: scope,
    array(Node.node(Node.Constraint.assignable))
  ) =>
  (runtime, scope, Node.node(Node.Constraint.assignable)) =
  (~runtime, ~scope, nodes) => {
    let union_type = Parser_union.determine_union_type(nodes);
    switch (union_type) {
    | Some(Optional(rest)) =>
      let (runtime, scope, t) =
        parse__Node__UnionType__Nodes(~runtime, ~scope, rest);
      (runtime, scope, Optional(t));
    | Some(Nullable(rest)) =>
      let (runtime, scope, t) =
        parse__Node__UnionType__Nodes(~runtime, ~scope, rest);
      (runtime, scope, Nullable(t));

    | Some(Single(one)) => (runtime, scope, one)
    | Some(Union(members)) =>
      let (runtime, scope, reference) =
        Parser_union.generate_ast_for_union(~runtime, ~scope, members);

      (runtime, scope, reference);
    | Some(NumericLiteral(literals)) =>
      Parser_generators.generate_number_literal_list(
        ~runtime,
        ~scope,
        literals,
      )
    | Some(MixedLiteral(literals)) =>
      Parser_generators.generate_mixed_literal_list(
        ~runtime,
        ~scope,
        literals,
      )
    | Some(StringLiteral(literals)) =>
      Parser_generators.generate_string_literal_list(
        ~runtime,
        ~scope,
        literals,
      )
    | None =>
      raise(Exceptions.UnexpectedAtThisPoint("Could not detect union type"))
    | _ => raise(Failure("Union nodes result not recognized"))
    };
  }
and parse__Node__UnionType = (~runtime, ~scope, node: Ts_nodes.UnionType.t) => {
  let type_nodes = node#getTypeNodes();
  let base_path = scope.path;
  let (scope, restore) = scope |> Scope.retain_path(base_path);
  // Note on unions:
  // As unions are usually extracted into their own modules for generation (unboxed for example)
  // we cannot apply any default args (that would generate the wrong types in the module)
  let (scope, restoreParams) = Context.retain_default_params(scope);
  let (runtime, scope, parsed_nodes) =
    type_nodes
    |> CCArray.foldi(
         ((runtime, scope, nodes), i, node) => {
           // TODO: Maybe a quick check to replace "1st" with a "t" when this will become an optional / a nullable
           let current_path =
             base_path |> Path.add(Identifier.SubIdent(i + 1));
           let scope = scope |> Scope.replace_path_arr(current_path);
           let (runtime, scope, t) =
             parse__Node__Generic_assignable(~runtime, ~scope, node);
           (runtime, scope, CCArray.append(nodes, [|t|]));
         },
         (runtime, scope, [||]),
       );
  let scope = restore(scope);
  let scope = restoreParams(scope);

  let (runtime, scope, t) =
    parse__Node__UnionType__Nodes(~runtime, ~scope, parsed_nodes);
  (runtime, scope, t |> Node.Escape.toAny);
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- IndexedAccessType
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__IndexedAccessType =
    (~runtime, ~scope, node: Ts_nodes.IndexedAccessType.t) => {
  let index_of_access_node = node => {
    let index = node#getIndexTypeNode();
    let index_stringified =
      switch (Ts_nodes_util.identifyGenericNode(index)) {
      | LiteralType(lt) =>
        switch (Ts_nodes_util.identifyGenericNode(lt#getLiteral())) {
        | StringLiteral(sl) => [|sl#getLiteralValue()|]
        | NumericLiteral(nl) => [|
            Printf.sprintf("%.0f", nl#getLiteralValue()),
          |]
        | l =>
          Runtime.warn_ftr(
            "Index of access node invalid: ",
            Pp.node_kind(l),
            runtime,
          );
          [|"t"|];
        }
      | TypeReference(_)
          when
            switch ((index |> Ts_nodes.WithGetType.fromGeneric)#getType()) {
            | None => false
            | Some(t) => t#isTypeParameter()
            } =>
        let param_name = index#getText();
        switch (scope |> Context.get_arg(param_name)) {
        | Some(Literal(String(str))) => [|str|]
        | Some(Literal(Number(n))) => [|Printf.sprintf("%.0f", n)|]
        | Some(l) =>
          Runtime.warn_ftr(
            "Index of access node invalid: ",
            Pp.ast_node(l),
            runtime,
          );
          [|"t"|];
        | None =>
          Runtime.warn_ftr(
            "Index of access node invalid: ",
            param_name,
            runtime,
          );
          [|"t"|];
        };
      | UnionType(union) =>
        // TODO: Abstract this function, currently all over the place with a lot of repition
        union#getTypeNodes()
        |> CCArray.filter_map(member =>
             switch (Ts_nodes_util.identifyGenericNode(member)) {
             | LiteralType(lt) =>
               switch (Ts_nodes_util.identifyGenericNode(lt#getLiteral())) {
               | StringLiteral(sl) => Some(sl#getLiteralValue())
               | NumericLiteral(nl) =>
                 Some(Printf.sprintf("%.0f", nl#getLiteralValue()))
               | l =>
                 Runtime.warn_ftr(
                   "Index of access node invalid (in union): ",
                   Pp.node_kind(l),
                   runtime,
                 );
                 None;
               }
             | _ => None
             }
           )
      | other =>
        Runtime.warn_ftr(
          "Index of access node invalid: ",
          Pp.node_kind(other),
          runtime,
        );
        [|"t"|];
      };
    (index, index_stringified);
  };

  // It's easiest to analyse this structurally, if the parent is the declaration
  switch (node#getParent()) {
  | Some(parent)
      when
        Ts_morph_util.isTypeAliasDeclaration(
          Ts_nodes.Generic.t_to_js(parent),
        ) =>
    let children = parent#getDescendants();
    let indexedAccessTypes =
      children
      |> CCArray.filter_map(child => {
           child#getKindName() == "IndexedAccessType"
             ? Some(Ts_nodes.IndexedAccessType.fromGeneric(child)) : None
         })
      |> CCArray.rev;

    let first_obj_type =
      CCArray.get(indexedAccessTypes, 0)#getObjectTypeNode();
    switch (Ts_nodes_util.identifyGenericNode(first_obj_type)) {
    | TypeReference(obj_reference) =>
      let ref_to =
        obj_reference#getSymbol()
        |> CCOpt.map_or(~default=obj_reference#getTypeName()#getText(), s =>
             s#getFullyQualifiedName()
           );
      let ref_path = build_path_from_ref_string(~scope, ref_to);

      // Make this easy, if we only have one access & it is listed in refs, just return a reference
      let (_, index_stringified) = index_of_access_node(node);
      if (CCArray.length(indexedAccessTypes) == 1
          && scope
          |> Scope.get_ref(
               ref_path
               |> Path.add(
                    Identifier.SubName(
                      CCArray.get_safe(index_stringified, 0)
                      |> CCOpt.value(~default=""),
                    ),
                  ),
             )
          |> CCOpt.is_some) {
        (
          runtime,
          scope,
          // TODO: This ref concept will definitely break down cross module - We need to resolve scopes here
          // But do we? Need to experiment with import declarations to say for sure
          Reference({
            target: [|
              Identifier.TypeName(
                Path.make_sub_type_name(
                  ref_path
                  |> Path.add(
                       Identifier.SubName(
                         CCArray.unsafe_get(index_stringified, 0),
                       ),
                     ),
                ),
              ),
            |],
            params: [],
          }),
        );
      } else {
        switch (
          Parser_resolvers.extract_from_resolved(
            ~wrap_sub_node=parse__Node__Resolved__WrapSubNode,
            ~runtime,
            ~scope,
            ref_path,
            indexedAccessTypes
            |> CCArray.map(n => snd(index_of_access_node(n))),
          )
        ) {
        | Some((runtime, scope, t)) => (
            runtime,
            scope,
            t |> Node.Escape.toAny,
          )
        | None =>
          // TODO: Should not be any
          (runtime, scope, Basic(Any))
        };
      };
    | _ =>
      raise(
        Exceptions.UnexpectedAtThisPoint(
          Printf.sprintf(
            "Expected a type reference, got %s instead",
            first_obj_type#getKindName(),
          ),
        ),
      )
    };
  /*| Some(parent)
    when
      switch ((parent |> Ts_nodes.WithGetType.fromGeneric)#getType()) {
      | None => false
      | Some(t) => Ts_nodes_util.Type.has_object_flag(t, Mapped)
      } =>*/
  | Some(_) =>
    let (_, index_stringified) = index_of_access_node(node);
    let index_stringified =
      CCArray.get_safe(index_stringified, 0) |> CCOpt.value(~default="");

    let maybe_resolve_from_record = maybe_record_with_runtime_and_scope =>
      switch (maybe_record_with_runtime_and_scope) {
      | Some((runtime, scope, Record(fields))) =>
        // TODO: This needs to be potentially extracted into a sub type
        switch (
          fields
          |> CCArray.find_idx(field =>
               switch (field) {
               | Parameter({name: Identifier.PropertyName(name), _}) =>
                 name == index_stringified
               | _ => false
               }
             )
        ) {
        | Some((_, Parameter({type_: Reference({target, _}) as type_, _}))) =>
          // Special case: If this is a reference, then the scope needs to get an additional reference count from the originating path
          // Maybe breaking at this point, idea for future:
          // Add scope field to reference so that refs can be re-built from anywhere that obj is used
          let scope =
            scope
            |> Scope.add_ref(
                 Path.make_current_scope(scope.path) |> Path.append(target),
                 scope.path,
               );
          (runtime, scope, type_ |> Node.Escape.toAny);
        | Some((_, Parameter({type_, _}))) => (
            runtime,
            scope,
            type_ |> Node.Escape.toAny,
          )
        | None =>
          raise(
            Exceptions.UnexpectedAtThisPoint(
              Printf.sprintf(
                "Could not extract type '%s' from record",
                index_stringified,
              ),
            ),
          )
        }
      | Some((_, _, other)) =>
        raise(
          Exceptions.FeatureMissing(
            Pp.ast_node(other),
            "Cannot type extract from this",
          ),
        )
      | None =>
        // Trying to resolve without all args present
        (runtime, scope, Basic(Never))
      };

    // TODO: This might be too tightly tailored to a use case
    // Should abstract the whole mapped type at some point, many redundancies
    let obj_type = node#getObjectTypeNode();
    switch (Ts_nodes_util.identifyGenericNode(obj_type)) {
    | TypeReference(obj_reference) =>
      switch (obj_reference#getType()) {
      | Some(t) when t#isTypeParameter() =>
        let param_name = t#getText();
        maybe_resolve_from_record(
          scope
          |> Context.get_arg(param_name)
          |> CCOpt.flat_map(arg =>
               Parser_resolvers.follow_references_from_resolved(
                 ~runtime,
                 ~scope,
                 arg |> Node.Escape.toAny,
               )
             ),
        );
      | Some(t) =>
        let maybe_path =
          scope.path
          |> Path.make_current_scope
          |> Path.add(
               Identifier.TypeName(obj_reference#getTypeName()#getText()),
             );
        let maybe = scope.root_declarations |> find_td(maybe_path);
        switch (maybe) {
        | Some((_, {annot: Record(_) as r, _})) =>
          maybe_resolve_from_record(Some((runtime, scope, r)))
        | Some(_)
        | None =>
          let res = Parser_resolvers.try_to_resolve_type(~runtime, ~scope, t);
          maybe_resolve_from_record(res);
        };
      | None =>
        raise(
          Exceptions.FeatureMissing(
            obj_type#getKindName(),
            "Not implemented if it doesn't resolve to a sub type",
          ),
        )
      }
    | _ =>
      raise(
        Exceptions.FeatureMissing(
          obj_type#getKindName(),
          "Not implemented as indexed type child of a mapped type",
        ),
      )
    };
  | _ =>
    raise(
      Exceptions.UnexpectedAtThisPoint(
        Printf.sprintf(
          "Could not resolve type for IndexedAccess (%s - %s)",
          node#getKindName(),
          node#getText(),
        ),
      ),
    )
  };
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- InterfaceDeclaration
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__InterfaceDeclaration__CollectExtension:
  (~runtime: runtime, ~scope: scope, Ts_nodes.InterfaceDeclaration.t) =>
  (
    runtime,
    scope,
    array((option(string), bool, Node.node(Node.Constraint.assignable))),
  ) =
  (~runtime, ~scope, node: Ts_nodes.InterfaceDeclaration.t) => {
    let base_path = scope.path;
    node#getExtends()
    |> CCArray.map(en => (en#getType(), en#getTypeArguments()))
    |> CCArray.filter_map(((t, args)) =>
         t#getSymbol() |> CCOpt.map(s => (s, args))
       )
    |> CCArray.fold_left(
         (declarations, (s, args)) =>
           CCArray.append(
             declarations,
             [|
               (CCArray.get(s#getDeclarations(), 0), args, s#getMembers()),
             |],
           ),
         [||],
       )
    |> CCArray.fold_left(
         ((runtime, scope, rendered), (declaration, args, members)) => {
           let (runtime, scope, additional) =
             switch (declaration |> Ts_nodes_util.identifyGenericNode) {
             | InterfaceDeclaration(i_node) =>
               // First: Create Context
               let (runtime, scope, res) =
                 parse__Node__InterfaceDeclaration__CollectExtension(
                   ~runtime,
                   ~scope,
                   i_node,
                 );
               let scope =
                 switch (i_node#getName()) {
                 | Some(name) =>
                   scope
                   |> Scope.replace_path_arr(
                        base_path |> Path.add(Identifier.TypeName(name)),
                      )
                 | None => scope
                 };
               (runtime, scope, res);

             | _ => (runtime, scope, [||])
             };
           let scoped_path = scope.path;
           let scope = scope |> Scope.replace_path_arr(base_path);
           let (runtime, scope) =
             create__ContextFromNode(
               ~runtime,
               ~type_arguments=args,
               ~node=declaration,
               scope,
             );
           let scope = scope |> Scope.replace_path_arr(scoped_path);
           // Second: Render Members

           let (runtime, scope, members): (
             runtime,
             scope,
             array(
               (
                 option(string),
                 bool,
                 Node.node(Node.Constraint.assignable),
               ),
             ),
           ) =
             members
             |> CCArray.filter_map(s => s#getValueDeclaration())
             |> CCArray.map(n => Ts_nodes_util.identifyGenericNode(n))
             |> CCArray.fold_left(
                  ((runtime, scope, nodes), node) => {
                    let (runtime, scope, rn) =
                      parse__Node__SignatureLike(~runtime, ~scope, node);
                    (runtime, scope, CCArray.append(nodes, [|rn|]));
                  },
                  (runtime, scope, [||]),
                );

           (
             runtime,
             scope,
             CCArray.append(rendered, CCArray.append(additional, members)),
           );
         },
         (runtime, scope, [||]),
       );
  }
and parse__Node__InterfaceDeclaration =
    (~runtime, ~scope, node: Ts_nodes.InterfaceDeclaration.t) => {
  let name =
    switch (node#getName()) {
    | Some(name) => name
    | None when node#isDefaultExport() => "default"
    | None => raise(Failure("Expected a name for this function"))
    };
  let type_name: Identifier.t(Identifier.Constraint.exactlyTypeName) =
    Identifier.TypeName(name);
  let scope = scope |> Scope.add_to_path(Identifier.TypeName(name));
  let base_path = scope.path;

  // Try to follow the extension nodes
  // TODO: Think about if inline types should be referenced rather than re-created

  let (runtime, scope, extended_nodes) =
    parse__Node__InterfaceDeclaration__CollectExtension(
      ~runtime,
      ~scope,
      node,
    );

  let nodes_to_parse =
    node#getMembers() |> CCArray.map(Ts_nodes_util.identifyGenericNode);
  let (runtime, scope, signatures) =
    nodes_to_parse
    |> CCArray.fold_left(
         ((runtime, scope, nodes), node) => {
           let scope = scope |> Scope.replace_path_arr(base_path);
           let (runtime, scope, signature) =
             parse__Node__SignatureLike(~runtime, ~scope, node);
           (runtime, scope, CCArray.append(nodes, [|signature|]));
         },
         (runtime, scope, [||]),
       );
  let signatures =
    CCArray.append(extended_nodes, signatures)
    |> CCArray.map(signature => {
         switch (signature) {
         | (None, _, _) =>
           raise(
             Failure("Type literal property should probably have a name"),
           )
         | (Some(name), is_optional, t) =>
           Parameter({
             name: Identifier.PropertyName(name),
             is_optional,
             type_: t,
             named: true,
           })
         }
       });
  let scope = scope |> Scope.replace_path_arr(base_path);

  let params =
    node#getTypeParameters()
    |> CCArray.map(n => Identifier.TypeParameter(n#getName()));
  // TODO: While it is important to collect params in an interface (because signatures can have their own), this next step is not exactly performant. Should replace eventually (maybe have signatures report their required params to the scope and read it here)
  let detect_params = finalize_generic_reference(Record(signatures));
  let params =
    CCArray.length(detect_params) == CCArray.length(params)
      ? params
      : CCArray.append(params, detect_params)
        |> CCArray.to_list
        |> CCList.uniq(~eq=(==))
        |> CCArray.of_list;

  (
    runtime,
    scope,
    TypeDeclaration({
      path: scope.path,
      name: type_name,
      annot: Record(signatures),
      params,
    }),
  );
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- FunctionType
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__FunctionType =
    (~runtime, ~scope, node: Ts_nodes.FunctionType.t) => {
  let (runtime, scope, t) =
    parse__Node__FunctionLikeNode(
      ~runtime,
      ~scope,
      node#getReturnTypeNode(),
      node#getParameters(),
    );
  (runtime, scope, t |> Node.Escape.toAny);
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- FunctionDeclaration
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__FunctionDeclaration =
    (~runtime, ~scope, node: Ts_nodes.FunctionDeclaration.t) => {
  let name =
    switch (node#getName()) {
    | Some(name) => name
    | None when node#isDefaultExport() => "default"
    | None => raise(Failure("Expected a name for this function"))
    };
  let type_name: Identifier.t(Identifier.Constraint.exactlyTypeName) =
    Identifier.TypeName(name);
  let scope = scope |> Scope.add_to_path(Identifier.TypeName(name));

  let params =
    node#getTypeParameters()
    |> CCArray.map(n => Identifier.TypeParameter(n#getName()));

  let scope = {
    ...scope,
    current_declaration: Some(FunctionDeclaration(node)),
    current_declared_params: params,
  };

  let (runtime, scope, annot) =
    parse__Node__FunctionLikeNode(
      ~runtime,
      ~scope,
      node#getReturnTypeNode(),
      node#getParameters(),
    );

  (
    runtime,
    scope,
    TypeDeclaration({path: scope.path, name: type_name, annot, params}),
  );
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- FunctionLikeNode
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__FunctionLikeNode:
  (
    ~runtime: runtime,
    ~scope: scope,
    option(Ts_nodes.Generic.t),
    array(Ts_nodes.Parameter.t)
  ) =>
  (runtime, scope, Node.node(Node.Constraint.assignable)) =
  (~runtime, ~scope, return, parameters) => {
    let base_path = scope.path;

    let (runtime, scope, return_type) =
      switch (return) {
      | Some(return_node) =>
        let scope =
          scope
          |> Scope.replace_path_arr(
               base_path |> Path.add(Identifier.SubName("return")),
             );
        parse__Node__Generic_assignable(~runtime, ~scope, return_node);
      | None => parse__AssignAny(~runtime, ~scope)
      };
    let scope = scope |> Scope.replace_path_arr(base_path);

    let (runtime, scope, parameters) =
      parameters
      |> CCArray.foldi(
           ((runtime, scope, params), i, param) => {
             let name = Identifier.PropertyName(param#getName());
             let is_optional = param#isOptional();
             let current_path =
               base_path |> Path.add(Identifier.SubName(Path.unwrap(name)));
             let scope = scope |> Scope.replace_path_arr(current_path);
             // TODO: isRestParameter
             let (runtime, scope, type_) =
               switch (param#getTypeNode()) {
               | None => parse__AssignAny(~runtime, ~scope)
               | Some(t) =>
                 parse__Node__Generic_assignable(~runtime, ~scope, t)
               };
             (
               runtime,
               scope,
               CCArray.append(
                 params,
                 [|
                   Parameter({name, is_optional, type_, named: is_optional}),
                 |],
               ),
             );
           },
           (runtime, scope, [||]),
         );
    let scope = scope |> Scope.replace_path_arr(base_path);

    (runtime, scope, Function({return_type, parameters}));
  }
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Type Literal
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__TypeLiteral =
    (~runtime, ~scope, node: Ts_nodes.TypeLiteral.t) => {
  let nodes_to_parse =
    node#getMembers() |> CCArray.map(Ts_nodes_util.identifyGenericNode);
  let (scope, restore) = scope |> Scope.retain_path(scope.path);
  let (runtime, scope, t) =
    try({
      let (runtime, scope, signatures) =
        nodes_to_parse
        |> CCArray.fold_left(
             ((runtime, scope, nodes), node) => {
               let scope = restore(scope);
               let (runtime, scope, signature) =
                 parse__Node__SignatureLike(~runtime, ~scope, node);
               (
                 runtime,
                 scope,
                 switch (signature) {
                 | (None, _, _) =>
                   raise(
                     Failure(
                       "Type literal property should probably have a name",
                     ),
                   )
                 | (Some(name), is_optional, t) =>
                   CCArray.append(
                     nodes,
                     [|
                       Parameter({
                         name: Identifier.PropertyName(name),
                         is_optional,
                         type_: t,
                         named: true,
                       }),
                     |],
                   )
                 },
               );
             },
             (runtime, scope, [||]),
           );
      (runtime, scope, Record(signatures));
    }) {
    // ------------
    // TODO
    // Defeinitely move this from here, also not sure if exception is the way to go
    // ------------
    | PlannedExceptions.LoneFunctionSignature(fn_declaration) =>
      let scope = restore(scope);

      // TODO:
      // Somehow report these type parameters to the parent
      // let params =
      //       fn_declaration#getTypeParameters()
      //       |> CCArray.map(n => Identifier.TypeParameter(n#getName()));

      parse__Node__FunctionType(~runtime, ~scope, fn_declaration);

    | other => raise(other)
    };
  let scope = restore(scope);

  (runtime, scope, t);
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Signature Hub
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__SignatureLike:
  (~runtime: runtime, ~scope: scope, Ts_nodes.nodeKind) =>
  (
    runtime,
    scope,
    (option(string), bool, Node.node(Node.Constraint.assignable)),
  ) =
  (~runtime, ~scope, node: Ts_nodes.nodeKind) => {
    switch (node) {
    | PropertySignature(node) =>
      let name = Some(node#getName());
      let type_node = node#getTypeNode();
      let base_path = scope.path;
      let (runtime, scope, t) =
        switch (type_node) {
        | None => parse__AssignAny(~runtime, ~scope)
        | Some(type_node) =>
          let current_path =
            base_path |> Path.add(Identifier.SubName(node#getName()));
          let scope = scope |> Scope.replace_path_arr(current_path);
          parse__Node__Generic_assignable(~runtime, ~scope, type_node);
        };
      let scope = scope |> Scope.replace_path_arr(base_path);
      let is_optional = node#getQuestionTokenNode() |> CCOpt.is_some;
      (runtime, scope, (name, is_optional, t));
    | MethodSignature(node) =>
      let name = node#getName();
      let return_type = node#getReturnTypeNode();
      let parameters = node#getParameters();
      let is_optional = node#getQuestionTokenNode() |> CCOpt.is_some;

      let base_path = scope.path;
      let scope =
        scope
        |> Scope.replace_path_arr(
             base_path |> Path.add(Identifier.SubName(name |> CCOpt.get_exn)),
           );

      let (runtime, scope, fn) =
        parse__Node__FunctionLikeNode(
          ~runtime,
          ~scope,
          return_type,
          parameters,
        );

      let scope = scope |> Scope.replace_path_arr(base_path);

      (runtime, scope, (name, is_optional, fn));
    | CallSignature(node)
        when Ts_morph_util.isNamedNode(node |> Ts_nodes.CallSignature.t_to_js) =>
      let name = node#getName();
      let return_type = node#getReturnTypeNode();
      let parameters = node#getParameters();
      // let type_parameters = node#getTypeParameters();

      let base_path = scope.path;
      let scope =
        scope
        |> Scope.replace_path_arr(
             base_path |> Path.add(Identifier.SubName(name |> CCOpt.get_exn)),
           );

      let (runtime, scope, fn) =
        parse__Node__FunctionLikeNode(
          ~runtime,
          ~scope,
          return_type,
          parameters,
        );

      let scope = scope |> Scope.replace_path_arr(base_path);

      (runtime, scope, (name, false, fn));
    | CallSignature(node)
        when
          !Ts_morph_util.isNamedNode(node |> Ts_nodes.CallSignature.t_to_js)
          && (
            switch (
              node#getParent() |> CCOpt.map(Ts_nodes_util.identifyGenericNode)
            ) {
            | Some(InterfaceDeclaration(_)) => true
            | Some(TypeLiteral(_)) => true
            | _ => false
            }
          ) =>
      raise(PlannedExceptions.LoneFunctionSignature(node))
    | node =>
      Debug.nodeKind_to_json(node);
      Console.log(Pp.path(scope.path));
      let node = Ts_nodes_util.unwrap_identified(node);
      raise(
        Exceptions.FeatureMissing(
          "Signature: " ++ node#getKindName(),
          node#getText(),
        ),
      );
    };
  }
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- EnumDeclaration
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__EnumDeclaration =
    (~runtime, ~scope, node: Ts_nodes.EnumDeclaration.t) => {
  // TODO: Solving this simply for now, we need to look at the propertyName eventually‚ though!!
  let members = node#getMembers();
  let type_name: Identifier.t(Identifier.Constraint.exactlyTypeName) =
    Identifier.TypeName(node#getName());
  let scope = {
    ...scope,
    current_declaration: Some(EnumDeclaration(node)),
    current_declared_params: [||],
    path:
      CCArray.append(scope.path, [|Identifier.TypeName(node#getName())|]),
  };
  let variant_constructors =
    members
    |> CCArray.map(member =>
         {
           VariantConstructor.name:
             Identifier.VariantIdentifier(member#getName(), false),
           arguments: [||],
         }
       );
  let annotation = Variant(variant_constructors, `variant);

  (
    runtime,
    scope,
    TypeDeclaration({
      path: scope.path,
      name: type_name,
      annot: annotation,
      params: [||],
    }),
  );
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- TypeAliasDeclaration
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__TypeAliasDeclaration:
  (~runtime: runtime, ~scope: scope, Ts_nodes.TypeAliasDeclaration.t) =>
  (runtime, scope, Node.node(Node.Constraint.moduleLevel)) =
  (~runtime, ~scope, node: Ts_nodes.TypeAliasDeclaration.t) => {
    let type_name: Identifier.t(Identifier.Constraint.exactlyTypeName) =
      Identifier.TypeName(node#getName());

    let params =
      node#getTypeParameters()
      |> CCArray.map(n => Identifier.TypeParameter(n#getName()));

    let scope = {
      ...scope,
      current_declaration: Some(TypeAliasDeclaration(node)),
      current_declared_params: params,
      path:
        CCArray.append(scope.path, [|Identifier.TypeName(node#getName())|]),
    };
    let (runtime, scope) =
      create__ContextFromNode(
        ~runtime,
        ~type_arguments=[||],
        ~node=node |> Ts_nodes.TypeAliasDeclaration.toGeneric,
        scope,
      );

    let (runtime, scope, annotation) =
      parse__Node__Generic_assignable(~runtime, ~scope, node#getTypeNode());

    (
      runtime,
      scope,
      TypeDeclaration({
        path: scope.path,
        name: type_name,
        annot: annotation,
        params,
      }),
    );
  }
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Basic Nodes
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__Basic = (~runtime, ~scope, node: Ts_nodes.nodeKind) => {
  switch (node) {
  | StringKeyword(_) => (runtime, scope, Basic(String))
  | NumberKeyword(_) => (runtime, scope, Basic(Number))
  | NeverKeyword(_) => (runtime, scope, Basic(Never))
  | ObjectKeyword(_) => (runtime, scope, Basic(RelevantKeyword("Object")))
  | UndefinedKeyword(_) => (runtime, scope, Basic(Undefined))
  | VoidKeyword(_) => (runtime, scope, Basic(Void))
  | NullKeyword(_) => (runtime, scope, Basic(Null))
  | BooleanKeyword(_) => (runtime, scope, Basic(Boolean))
  | AnyKeyword(_) => (runtime, {...scope, has_any: true}, Basic(Any))
  | _ => raise(Exceptions.UnexpectedAtThisPoint("Not a basic type"))
  };
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Tuples
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__Tuple = (~runtime, ~scope, node: Ts_nodes.nodeKind) => {
  switch (node) {
  | TupleType(node) =>
    let children_to_traverse = node#getElementTypeNodes();
    let base_path = scope.path;
    let (
      runtime: runtime,
      scope: scope,
      inner: array(Node.node(Node.Constraint.assignable)),
    ) =
      CCArray.foldi(
        ((runtime, scope, nodes), i, node) => {
          let current_path =
            base_path |> Path.add(Identifier.SubIdent(i + 1));
          let scope = scope |> Scope.replace_path_arr(current_path);
          let (runtime, scope, res) =
            parse__Node__Generic_assignable(~runtime, ~scope, node);
          (runtime, scope, CCArray.append(nodes, [|res|]));
        },
        (runtime, scope, [||]),
        children_to_traverse,
      );
    let scope = scope |> Scope.replace_path_arr(base_path);

    (runtime, scope, Tuple(inner));
  | _ => raise(Exceptions.UnexpectedAtThisPoint("Not a tuple"))
  };
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- TypeReference
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node_TypeReference = (~runtime, ~scope, node: Ts_nodes.nodeKind) => {
  switch (node) {
  // Is a reference to a generic type
  | TypeReference(node)
      when
        node#getType()
        |> CCOpt.map_or(~default=false, t => t#isTypeParameter()) =>
    let tpName = node#getTypeName()#getText();
    (
      runtime,
      scope,
      switch (Context.get_arg(tpName, scope)) {
      | None => GenericReference(Identifier.TypeParameter(tpName))
      | Some(argType) => Node.Escape.toAny(argType)
      },
    );
  // Fixed keyword reference
  | TypeReference(node) when node#getText() == "Error" => (
      runtime,
      scope,
      Basic(RelevantKeyword("Js.Exn.t")),
    )
  // "Normal" reference
  | TypeReference(node) =>
    let type_name = node#getTypeName();

    // We can get the referenced type naively by symbol.declaration for now
    // This might need to change in a more complex setting later
    let type_arguments = node#getTypeArguments();
    let symbol = type_name#getSymbol();
    let declaration =
      symbol
      |> CCOpt.map(symbol => symbol#getDeclarations())
      |> CCOpt.flat_map(CCArray.get_safe(_, 0))
      |> CCOpt.map(Ts_nodes.Generic.fromMorphNode);
    let (runtime, scope, arguments) =
      switch (declaration) {
      | None =>
        // If a declaration cannot be resolved, we'll just use the applied arguments and hope for the best
        parse__ArrayOfGenerics(~runtime, ~scope, type_arguments)
        |> parse__map(arr =>
             arr
             |> CCArray.mapi((i, arg) => (string_of_int(i), arg))
             |> CCArray.to_list
           )
      | Some(declaration_node) =>
        // Force cast this into a TypeParametered - this should be fine as we only need the `getTypeParameters` method, which should be present on any declaration type
        let parameters =
          Ts_nodes.TypeParametered.fromGeneric(declaration_node)#
            getTypeParameters();
        let args =
          parameters
          |> CCArray.foldi(
               (args, i, param) => {
                 let name = param#getName();
                 let arg_type =
                   switch (CCArray.get_safe(type_arguments, i)) {
                   | Some(arg) => arg
                   | None =>
                     switch (param#getDefault()) {
                     | Some(arg) => arg
                     | None =>
                       raise(
                         Exceptions.UnexpectedAtThisPoint(
                           "Expects this type arg to at least have a default value",
                         ),
                       )
                     }
                   };
                 args @ [(name, arg_type)];
               },
               [],
             );
        let (names, nodes) = CCList.split(args);
        parse__ArrayOfGenerics(
          ~scope_additions=CCArray.of_list(names),
          ~runtime,
          ~scope,
          CCArray.of_list(nodes),
        )
        |> parse__map(arr => CCList.combine(names, CCArray.to_list(arr)));
      };

    // Parse the reference information to make a note that it was referenced
    let ref_path =
      scope.path
      |> Path.make_current_scope
      |> Path.add(Identifier.TypeName(type_name#getText()));
    let scope = scope |> Scope.add_ref(ref_path, scope.path);

    let default_return = (
      runtime,
      scope,
      Reference({
        target: [|Identifier.TypeName(type_name#getText())|],
        params: arguments,
      }),
    );
    switch (
      (
        Ts_nodes.Identifier.toGeneric(node#getTypeName())
        |> Ts_nodes.WithGetType.fromGeneric
      )#
        getType()
      |> CCOpt.flat_map(t => t#getSymbol() |> CCOpt.map(s => (t, s)))
    ) {
    | None => default_return
    | Some((ty, symbol)) =>
      switch (
        CCArray.get_safe(symbol#getDeclarations(), 0)
        |> CCOpt.map(Ts_nodes_util.identifyGenericNode)
      ) {
      // --- SPECIAL CASE: Mapped Type as target
      | Some(MappedType(mapped_type))
          when mapped_type#getParent() |> CCOpt.is_some =>
        // This path applies (hopefully) all necessary arguments and tries to resolve a mapped type
        // All of this needs to be further abstracted for sure
        let parent = mapped_type#getParent() |> CCOpt.get_exn;
        let parent = parent |> Ts_nodes.TypeParametered.fromGeneric;
        let args = arguments;
        let parameters = parent#getTypeParameters();

        let (scope, reset) =
          scope
          |> Scope.retain_path(
               scope.path |> Path.add(Identifier.SubName("t")),
             );
        let scope =
          parameters
          |> CCArray.foldi(
               (scope, i, param) => {
                 scope |> Context.add_arg_tpl(CCList.get_at_idx_exn(i, args))
               },
               scope,
             );
        let (runtime, scope, t) =
          parse__Node__Generic_assignable(~runtime, ~scope, mapped_type);
        let scope = reset(scope);
        (runtime, scope, t |> Node.Escape.toAny);
      | Some(_)
      | None => default_return
      }
    };
  | _ => raise(Exceptions.UnexpectedAtThisPoint("Not a type reference"))
  };
}
and parse__ArrayOfGenerics =
    (
      ~scope_additions=[||],
      ~runtime,
      ~scope,
      nodes: array(Ts_nodes.Generic.t),
    ) => {
  let (scope, restore) = scope |> Scope.retain_path(scope.path);
  let (runtime, scope, resolved) =
    nodes
    |> CCArray.foldi(
         ((runtime, scope, resolved), i, node) => {
           let scope = restore(scope);
           let scope =
             scope
             |> Scope.add_to_path(
                  CCArray.get_safe(scope_additions, i)
                  |> CCOpt.map_or(~default=Identifier.SubIdent(i + 1), v =>
                       Identifier.SubName(v)
                     ),
                );

           let (runtime, scope, t) =
             parse__Node__Generic_assignable(~runtime, ~scope, node);
           (runtime, scope, CCArray.append(resolved, [|t|]));
         },
         (runtime, scope, [||]),
       );
  let scope = restore(scope);
  (runtime, scope, resolved);
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Arrays
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__Array = (~runtime, ~scope, node: Ts_nodes.nodeKind) => {
  switch (node) {
  | ArrayType(node) =>
    let scope = scope |> Scope.add_to_path(Identifier.SubName("t"));
    let (runtime, scope, inner) =
      parse__Node__Generic__WrapSubNode(
        ~runtime,
        ~scope,
        node#getElementTypeNode(),
      );
    (runtime, scope, Array(inner |> Node.Escape.toAssignable));

  | _ => raise(Exceptions.UnexpectedAtThisPoint("Not an array"))
  };
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Adding Fixtures
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Fixtures__ForSourceFile = (~runtime, ~scope) => {
  (
    runtime,
    scope,
    if (scope.has_any) {
      [|Fixture(AnyUnboxed, [||])|];
    } else {
      [||];
    },
  );
};

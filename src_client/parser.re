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
let rec parse__Entry = (~source_files: array(Ts_morph.SourceFile.t)) => {
  let runtime = {
    root_modules: [||],
    node_count: 0,
    parse_config: {
      exports_only: false,
    },
    fully_qualified_added: [],
  };
  let runtime =
    source_files
    |> CCArray.fold_left(
         (runtime, source_file) => {
           let scope = {
             root_declarations: [||],
             source_file: None,
             parent: None,
             path: [||],
             has_any: false,
             refs: Hashtbl.create(10),
             context_params: [],
             context_args: [],
             context_args_raw: [],
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
  | MappedType(mapped_type) =>
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
    let value_node =
      children
      |> CCArray.find_map_i((i, node) =>
           node#getKindName() == "ColonToken"
             ? CCArray.get_safe(children, i + 1) : None
         )
      |> CCOpt.get_exn;

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

    let parse__Value = (~runtime, ~scope, ~subname=?, key_type) => {
      let (scope, restore) =
        Scope.retain_path(
          scope.path
          |> Path.add(
               Identifier.SubName(CCOpt.value(~default="t", subname)),
             ),
          scope,
        );
      let parameter_name = type_parameter#getName();
      let scope =
        scope
        |> Context.add_param(parameter_name, None)
        |> Context.add_arg(parameter_name, key_type);
      let (runtime, scope, value_resolved) =
        parse__Node__Generic_assignable(~runtime, ~scope, value_node);
      let scope = restore(scope);
      (runtime, scope, value_resolved);
    };

    // Extract Properties from union
    if (key_list_type#isUnion()) {
      let keys =
        key_list_type#getUnionTypes()
        |> CCArray.filter(t => t#isLiteral())
        |> CCArray.map(t => t#getText());

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
                       is_optional: false,
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
                       is_optional: false,
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
      (runtime, scope, Basic(Never));
    };
  | TypeOperator(_) =>
    parse__Node__TypeOperator(~runtime, ~scope, identifiedNode)
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
  let type_name = Identifier.TypeName(name);
  let scope = scope |> Scope.add_to_path(type_name);
  let scoped_path =
    Pp.path(Path.make_current_scope(scope.path) |> Path.add(type_name));

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
            path: scope.path,
            name: type_name,
            annot: node,
            params: [||],
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

  (runtime, scope, Reference({target: [|type_name|], params: [||]}));
}
and parse__Node__Generic__WrapSubNode =
    (~runtime, ~scope, node: Ts_nodes.Generic.t) => {
  let name = Path.make_sub_type_name(scope.path);
  let sub_path = scope.path;
  let type_name = Identifier.TypeName(name);
  let scope = scope |> Scope.add_to_path(type_name);

  // TODO:
  // Even though we're looking for a normalized scoped path here to
  // avoid duplicates, this isn't ideal. Typescript declarations could
  // be all over the place, we need to pre-render dependencies in the tree if possible
  // When that is done, there needs to be a check like this for main types as well
  let scoped_path =
    Path.make_current_scope(scope.path) |> Path.add(type_name);
  let scoped_path_str = Pp.path(scoped_path);

  let (runtime, scope) =
    !
      CCList.mem(
        ~eq=CCString.equal,
        scoped_path_str,
        runtime.fully_qualified_added,
      )
      ? {
        let (runtime, scope, wrapped_type) =
          parse__Node__Generic_assignable(~runtime, ~scope, node);
        let wrapped_type_declaration =
          TypeDeclaration({
            path: scope.path,
            name: type_name,
            annot: wrapped_type,
            params: [||],
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

  let scope = scope |> Scope.add_ref(sub_path, [||]);

  (runtime, scope, Reference({target: [|type_name|], params: [||]}));
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Source File
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__SourceFile =
    (~runtime, ~scope, node: Ts_nodes.SourceFile.t)
    : (runtime, scope, Node.node(Node.Constraint.atLeastModule('poly))) => {
  let source_file_name = node#getBaseNameWithoutExtension();
  let scope = {...scope, source_file: Some(node)};
  let children_to_traverse: array(Ts_nodes.Generic.t) =
    runtime.parse_config.exports_only
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
      name: source_file_name,
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
                 let (runtime, scope, default) =
                   param#getDefault()
                   |> CCOpt.map(paramNode => {
                        let (runtime, scope, res) =
                          parse__Node__Generic_assignable(
                            ~runtime,
                            ~scope,
                            paramNode,
                          );
                        (runtime, scope, Some(res));
                      })
                   |> CCOpt.value(~default=(runtime, scope, None));
                 let scope = scope |> Context.add_param(name, default);

                 (
                   runtime,
                   scope,
                   CCArray.append(params, [|(name, default)|]),
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
             | (None, Some(default)) =>
               scope |> Context.add_arg(key, default)
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
  let children_to_traverse: array(Ts_nodes.Generic.t) =
    runtime.parse_config.exports_only
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
    | Some((runtime, scope, res)) => (
        runtime,
        scope,
        res |> Node.Escape.toAny,
      )
    | None =>
      raise(
        Exceptions.UnexpectedAtThisPoint(
          "Could not resolve type of TypeOperator",
        ),
      )
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
      | StringLiteral(s) => Literal(String(s#getLiteralValue()))
      | NumericLiteral(n) => Literal(Number(n#getLiteralValue()))
      | NullLiteral(_) => Basic(Null)
      | _ =>
        raise(
          Exceptions.FeatureMissing(inner#getKindName(), inner#getText()),
        )
      },
    );
  | _ => raise(Exceptions.UnexpectedAtThisPoint("Expected LiteralType"))
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
    // TODO: !!!!! Numeric / Mixed Literal
    | Some(StringLiteral(literals)) =>
      // TODO: React to different literal output options (like inline / variant, etc.)
      (
        runtime,
        scope,
        Parser_generators.generate_string_literal_list(literals),
      )
    | None =>
      raise(Exceptions.UnexpectedAtThisPoint("Could not detect union type"))
    | _ => raise(Failure("Union nodes result not recognized"))
    };
  }
and parse__Node__UnionType = (~runtime, ~scope, node: Ts_nodes.UnionType.t) => {
  let type_nodes = node#getTypeNodes();
  let base_path = scope.path;
  let (runtime, scope, parsed_nodes) =
    type_nodes
    |> CCArray.foldi(
         ((runtime, scope, nodes), i, node) => {
           let current_path =
             base_path |> Path.add(Identifier.SubIdent(i + 1));
           let scope = scope |> Scope.replace_path_arr(current_path);
           let (runtime, scope, t) =
             parse__Node__Generic_assignable(~runtime, ~scope, node);
           (runtime, scope, CCArray.append(nodes, [|t|]));
         },
         (runtime, scope, [||]),
       );
  let scope = scope |> Scope.replace_path_arr(base_path);

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
        | StringLiteral(sl) => sl#getLiteralValue()
        | NumericLiteral(nl) => Printf.sprintf("%.0f", nl#getLiteralValue())
        | _ => "t"
        }
      | _ => "t"
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
               ref_path |> Path.add(Identifier.SubName(index_stringified)),
             )
          |> CCOpt.is_some) {
        (
          runtime,
          scope,
          // TODO: This ref concept will definitely break down cross module
          // We need to resolve scopes here
          Reference({
            target: [|
              Identifier.TypeName(
                Path.make_sub_type_name(
                  ref_path |> Path.add(Identifier.SubName(index_stringified)),
                ),
              ),
            |],
            params: [||],
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

  | _ =>
    let (index, index_stringified) = index_of_access_node(node);
    // TODO: Make reosolving this type more robust
    // Also TODO: Create something that can request a type reference and moves referenced types into their own type declaration
    let base_path = scope.path;
    let (runtime, scope, result) =
      switch (
        get__TypeNodeByTypeChecker(
          node |> Ts_nodes.IndexedAccessType.toGeneric,
        )
      ) {
      | None =>
        switch (
          node#getType()
          |> CCOpt.flat_map(get__DerivedTypeFromTypeObj(~runtime, ~scope))
        ) {
        | None =>
          raise(
            Exceptions.UnexpectedAtThisPoint(
              "Could not resolve type for IndexedAccess",
            ),
          )
        | Some(r) => r
        }
      | Some(resolved_type) =>
        let current_path =
          base_path |> Path.add(Identifier.SubName(index_stringified));
        let scope = scope |> Scope.replace_path_arr(current_path);
        let (runtime, scope, result) =
          parse__Node__Generic_assignable(~runtime, ~scope, resolved_type);
        (runtime, scope, result);
      };
    let scope = scope |> Scope.replace_path_arr(base_path);

    (runtime, scope, result |> Node.Escape.toAny);
  };
}
// TODO: This is only an escape hatch.
// A complex type result might still fail
// in deep access fields like ['one']['two']
and get__DerivedTypeFromTypeObj:
  (~runtime: runtime, ~scope: scope, Ts_nodes.Type.t) =>
  option((runtime, scope, Node.node(Node.Constraint.assignable))) =
  (~runtime, ~scope, t) =>
    Parser_resolvers.try_to_resolve_type(~runtime, ~scope, t)
and get__TypeNodeByTypeChecker:
  Ts_nodes.Generic.t => option(Ts_nodes.Generic.t) =
  node => {
    (node |> Ts_nodes.WithGetType.fromGeneric)#getType()
    |> CCOpt.flat_map(t => {
         //  Debug.type_to_json(t);
         t#getSymbol()
       })
    |> CCOpt.map(s => s#getDeclarations())
    |> CCOpt.flat_map(CCArray.get_safe(_, 0));
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
  let scope = {...scope, parent: Some(FunctionDeclaration(node))};

  let (runtime, scope, annot) =
    parse__Node__FunctionLikeNode(
      ~runtime,
      ~scope,
      node#getReturnTypeNode(),
      node#getParameters(),
    );

  let params =
    node#getTypeParameters()
    |> CCArray.map(n => Identifier.TypeParameter(n#getName()));

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
      raise(Exceptions.FeatureMissing(node#getKindName(), node#getText()));
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
    parent: Some(EnumDeclaration(node)),
    path:
      CCArray.append(scope.path, [|Identifier.TypeName(node#getName())|]),
  };
  let variant_constructors =
    members
    |> CCArray.map(member =>
         {
           VariantConstructor.name:
             Identifier.VariantIdentifier(member#getName()),
           arguments: [||],
         }
       );
  let annotation = Variant(variant_constructors);

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
    let scope = {
      ...scope,
      parent: Some(TypeAliasDeclaration(node)),
      path:
        CCArray.append(scope.path, [|Identifier.TypeName(node#getName())|]),
    };
    let params =
      node#getTypeParameters()
      |> CCArray.map(n => Identifier.TypeParameter(n#getName()));

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
          let current_path = base_path |> Path.add(Identifier.SubIdent(i));
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
      | Some(declaration_node) =>
        // Force cast this into a TypeParametered - this should be fine as we only need the `getTypeParameters` method, which should be present on any declaration type
        let parameters =
          Ts_nodes.TypeParametered.fromGeneric(declaration_node)#
            getTypeParameters();
        let (names, nodes) =
          parameters
          |> CCArray.foldi(
               ((names, args), i, param) => {
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
                 (
                   CCArray.append(names, [|name|]),
                   CCArray.append(args, [|arg_type|]),
                 );
               },
               ([||], [||]),
             );
        parse__ArrayOfGenerics(
          ~scope_additions=names,
          ~runtime,
          ~scope,
          nodes,
        );
      };

    // Parse the reference information to make a note
    // that it was referenced
    let ref_to =
      symbol
      |> CCOpt.map_or(~default=type_name#getText(), s =>
           s#getFullyQualifiedName()
         );
    let ref_path = build_path_from_ref_string(~scope, ref_to);
    let scope = scope |> Scope.add_ref(ref_path, scope.path);

    switch (
      (
        Ts_nodes.Identifier.toGeneric(node#getTypeName())
        |> Ts_nodes.WithGetType.fromGeneric
      )#
        getType()
      |> CCOpt.flat_map(t => t#getSymbol())
    ) {
    | None => ()
    | Some(symbol) =>
      switch (CCArray.get_safe(symbol#getDeclarations(), 0)) {
      | None => ()
      | Some(dec) =>
        // TODO: Try inserting arguments into context here and resolve the mapped type
        // This will get more complex because it could not be finally resolved through many references
        // and even then there might be args missing
        Console.log(dec#getKindName())
      }
    };

    (
      runtime,
      scope,
      Reference({
        target: [|Identifier.TypeName(type_name#getText())|],
        params: arguments,
      }),
    );
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
                  |> CCOpt.map_or(~default=Identifier.SubIdent(i), v =>
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
    let (runtime, scope, inner) =
      // TODO: Handle potentially extracted type here
      // Whenever "inner" types are parsed, we should use a separate "parse__Node" function
      // Maybe
      parse__Node__Generic_assignable(
        ~runtime,
        ~scope,
        node#getElementTypeNode(),
      );
    (runtime, scope, Array(inner));

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
      [|Fixture(AnyUnboxed)|];
    } else {
      [||];
    },
  );
};

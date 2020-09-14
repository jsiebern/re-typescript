open Ast;
open Node;
open Parser_utils;

let should_exclude_for_ref = n =>
  n#getKindName() != "ModuleDeclaration"
  && n#getKindName() != "VariableDeclaration";
let type_of_reference =
    (~parse, ~runtime, ~scope, node: Ts_nodes.TypeReference.t) => {
  let checker = node#getProject()#getTypeChecker();
  let type_symbol =
    checker#getSymbolAtLocation(
      node#getTypeName()
      |> Ts_nodes.Identifier.toGeneric
      |> Ts_nodes.Generic.toMorphNode,
    )
    |> CCOpt.get_exn
    |> Ts_nodes.Symbol.fromMorphSymbol;
  if (type_symbol#isAlias()) {
    let aliased_symbol =
      checker#getAliasedSymbol(type_symbol |> Ts_nodes.Symbol.toMorphSymbol)
      |> CCOpt.get_exn
      |> Ts_nodes.Symbol.fromMorphSymbol;

    switch (
      aliased_symbol#getDeclarations()
      |> CCArray.filter(should_exclude_for_ref)
      |> CCArray.get_safe(_, 0)
    ) {
    | Some(node) =>
      let (runtime, scope, t) = parse(~runtime, ~scope, node);
      (runtime, scope, Some(t));
    | None => (runtime, scope, None)
    };
  } else if (type_symbol#hasFlags(
               Ts_morph_util.getSymbolFlag(Ts_nodes.Symbol.TypeParameter),
             )) {
    let name = type_symbol#getName();
    (runtime, scope, scope |> Context.get_arg(name));
  } else if (type_symbol#getName() == "Array"
             || type_symbol#getName() == "ReadonlyArray") {
    raise(Failure("Not sure what to do with this array"));
  } else {
    switch (
      type_symbol#getDeclarations()
      |> CCArray.filter(should_exclude_for_ref)
      |> CCArray.get_safe(_, 0)
    ) {
    | Some(node) =>
      let (runtime, scope, t) = parse(~runtime, ~scope, node);
      (runtime, scope, Some(t));
    | None => (runtime, scope, None)
    };
  };
};

let rec raw_remove_references = (t: Ts_nodes.Generic.t) => {
  let checker = t#getProject()#getTypeChecker();
  switch (t |> Ts_nodes_util.identifyGenericNode) {
  | TypeReference(tf) =>
    let type_symbol =
      checker#getSymbolAtLocation(
        tf#getTypeName()
        |> Ts_nodes.Identifier.toGeneric
        |> Ts_nodes.Generic.toMorphNode,
      )
      |> CCOpt.get_exn
      |> Ts_nodes.Symbol.fromMorphSymbol;

    type_symbol#getDeclarations()
    |> CCArray.get_safe(_, 0)
    |> CCOpt.flat_map(n => {raw_remove_references(n)});
  | _ => Some(t)
  };
};

let rec try_to_resolve_type = (~runtime, ~scope, t: Ts_nodes.Type.t) =>
  // Todo: Eventually maybe 'isEnumLiteral'
  // Todo: Eventually 'isClass'
  // Todo: Eventually 'Ts_nodes_util.Type.has_flag(t, ESSymbol)'
  if (t#isAny()) {
    let (runtime, scope, t) = parse__AssignAny(~runtime, ~scope);
    Some((runtime, scope, t));
  } else if (t#isBoolean()) {
    Some((runtime, scope, Basic(Boolean)));
  } else if (t#isNull()) {
    Some((runtime, scope, Basic(Null)));
  } else if (t#isNumber()) {
    Some((runtime, scope, Basic(Number)));
  } else if (t#isString()) {
    Some((runtime, scope, Basic(String)));
  } else if (t#isUndefined()) {
    Some((runtime, scope, Basic(Undefined)));
  } else if (t#isArray()) {
    t#getArrayElementType()
    |> CCOpt.map(Ts_nodes.Type.fromRootType)
    |> CCOpt.flat_map(inner => try_to_resolve_type(~runtime, ~scope, inner))
    |> ParserBag.Option.map((~runtime, ~scope, t) =>
         (runtime, scope, Array(t))
       );
  } else if (Ts_nodes_util.Type.has_flag(t, Void)) {
    Some((runtime, scope, Basic(Void)));
  } else if (Ts_nodes_util.Type.has_flag(t, BigInt)) {
    Some((runtime, scope, Basic(Number)));
  } else if (Ts_nodes_util.Type.has_flag(t, Never)) {
    None;
  } else if
    //Ts_nodes_util.Type.has_flag(t, Object)  &&
    (t#getAliasSymbol()
     |> CCOpt.is_some
     && t#getAliasTypeArguments()
     |> CCOpt.is_some) {
    let symbol = t#getAliasSymbol() |> CCOpt.get_exn;
    let arguments = t#getAliasTypeArguments() |> CCOpt.get_exn;
    let target =
      build_path_from_ref_string(
        ~with_scope=false,
        ~scope,
        symbol#getFullyQualifiedName(),
      );
    let (runtime, scope, arguments) =
      arguments
      |> ParserBag.map(~runtime, ~scope, (~runtime, ~scope, t) =>
           try_to_resolve_type(~runtime, ~scope, t)
           |> CCOpt.get_or(~default=parse__AssignAny(~runtime, ~scope))
         )
      |> parse__map(CCArray.to_list);
    // Potential error: Might need to pull in the defaults if arguments and parameter count differs
    let parameters =
      CCArray.get_safe(symbol#getDeclarations(), 0)
      |> CCOpt.map(Ts_nodes_util.identifyGenericNode)
      |> CCOpt.flat_map(node =>
           switch (node) {
           | Ts_nodes.Identify.TypeAliasDeclaration(td) => Some(td)
           | _ => None
           }
         )
      |> CCOpt.map(td => td#getTypeParameters())
      |> CCOpt.map(CCArray.map(p => p#getName()))
      |> CCOpt.map_or(~default=[], CCArray.to_list);
    let params = CCList.combine(parameters, arguments);
    Some((runtime, scope, Reference({target, params})));
  } else if (t#isBooleanLiteral()) {
    Some((
      runtime,
      scope,
      Literal(Boolean(t#getText() == "true" ? true : false)),
    ));
  } else if (t#isStringLiteral()) {
    Some((
      runtime,
      scope,
      Literal(
        String(
          {
            let text = t#getText();
            CCString.sub(text, 1, CCString.length(text) - 2);
          },
        ),
      ),
    ));
  } else if (t#isNumberLiteral()) {
    float_of_string_opt(t#getText())
    |> CCOpt.map(v => (runtime, scope, Literal(Number(v))));
  } else if (t#isUnion()) {
    let types = t#getUnionTypes();
    let (runtime, scope, types) =
      types
      |> CCArray.map(Ts_nodes.Type.fromRootType)
      |> CCArray.fold_left(
           ((runtime, scope, types), t) => {
             switch (try_to_resolve_type(~runtime, ~scope, t)) {
             | None => (runtime, scope, types)
             | Some((runtime, scope, t)) => (
                 runtime,
                 scope,
                 CCArray.append(types, [|t|]),
               )
             }
           },
           (runtime, scope, [||]),
         );

    let rec resolve_union =
            (~runtime, ~scope, union_type: option(Parser_union.unionType)) => {
      switch (union_type) {
      | Some(Optional(next)) =>
        resolve_union(
          ~runtime,
          ~scope,
          Parser_union.determine_union_type(~scope, next),
        )
        |> CCOpt.map(((runtime, scope, next)) =>
             (runtime, scope, Node.Optional(next))
           )
      | Some(Nullable(next)) =>
        resolve_union(
          ~runtime,
          ~scope,
          Parser_union.determine_union_type(~scope, next),
        )
        |> CCOpt.map(((runtime, scope, next)) =>
             (runtime, scope, Node.Nullable(next))
           )
      | Some(Single(t)) => Some((runtime, scope, t))
      | Some(StringLiteral(literals)) =>
        Some(
          Parser_generators.generate_string_literal_list(
            ~runtime,
            ~scope,
            literals,
          ),
        )
      | Some(NumericLiteral(literals)) =>
        Some(
          Parser_generators.generate_number_literal_list(
            ~runtime,
            ~scope,
            literals,
          ),
        )
      | Some(MixedLiteral(literals)) =>
        Some(
          Parser_generators.generate_mixed_literal_list(
            ~runtime,
            ~scope,
            literals,
          ),
        )
      | Some(Discriminating) =>
        raise(Exceptions.FeatureMissing("Discriminating", ""))
      | Some(Union(types)) =>
        Some(Parser_union.generate_ast_for_union(~runtime, ~scope, types))
      | None => None
      };
    };
    resolve_union(
      ~runtime,
      ~scope,
      Parser_union.determine_union_type(~scope, types),
    );
  } else if (t#isInterface()) {
    t#getSymbol()
    |> CCOpt.flat_map(sym => CCArray.get_safe(sym#getDeclarations(), 0))
    |> CCOpt.map(root_node => {
         let (runtime, scope, resolved_props) =
           t#getProperties()
           |> CCArray.map(prop_symbol => {
                let is_optional =
                  prop_symbol#hasFlags(Ts_nodes.Symbol.Optional);
                (
                  prop_symbol#getName(),
                  is_optional,
                  prop_symbol#getTypeAtLocation(root_node)
                  |> Ts_nodes.Type.fromRootType,
                );
              })
           |> CCArray.fold_left(
                ((runtime, scope, types), (name, is_optional, t)) => {
                  switch (try_to_resolve_type(~runtime, ~scope, t)) {
                  | None => (runtime, scope, types)
                  | Some((runtime, scope, res)) =>
                    let res_param =
                      Parameter({
                        name: Identifier.PropertyName(name),
                        is_optional,
                        type_: res,
                        named: true,
                      });
                    (runtime, scope, CCArray.append(types, [|res_param|]));
                  }
                },
                (runtime, scope, [||]),
              );
         (runtime, scope, Record(resolved_props));
       });
  } else if (t#isTypeParameter()) {
    t#getSymbol()
    |> CCOpt.flat_map(symbol => {
         let name = symbol#getName();
         scope
         |> Context.get_arg(name)
         |> CCOpt.map(t => (runtime, scope, t));
       });
  } else if (t#isTuple()) {
    let (runtime, scope, types) =
      t#getTupleElements()
      |> CCArray.map(Ts_nodes.Type.fromRootType)
      |> CCArray.fold_left(
           ((runtime, scope, types), t) => {
             switch (try_to_resolve_type(~runtime, ~scope, t)) {
             | None => (runtime, scope, types)
             | Some((runtime, scope, t)) => (
                 runtime,
                 scope,
                 CCArray.append(types, [|t|]),
               )
             }
           },
           (runtime, scope, [||]),
         );
    Some((runtime, scope, Tuple(types)));
  } else if (t#isIntersection()) {
    let (runtime, scope, types) =
      t#getIntersectionTypes()
      |> CCArray.map(Ts_nodes.Type.fromRootType)
      |> CCArray.fold_left(
           ((runtime, scope, types), t) => {
             switch (try_to_resolve_type(~runtime, ~scope, t)) {
             | None => (runtime, scope, types)
             | Some((runtime, scope, t)) => (
                 runtime,
                 scope,
                 CCArray.append(types, [|t|]),
               )
             }
           },
           (runtime, scope, [||]),
         );
    Some((
      runtime,
      scope,
      Parser_generators.generate_intersection_body(types),
    ));
  } else if (CCArray.length(t#getCallSignatures()) > 0) {
    let signature =
      CCArray.get(t#getCallSignatures(), 0)
      |> Ts_nodes.Signature.fromRootSignature;

    let root_node = signature#getDeclaration();
    let (runtime, scope, resolved_parameters) =
      signature#getParameters()
      |> CCArray.map(param_symbol => {
           let is_optional = param_symbol#hasFlags(Ts_nodes.Symbol.Optional);
           (
             param_symbol#getName(),
             is_optional,
             param_symbol#getTypeAtLocation(root_node)
             |> Ts_nodes.Type.fromRootType,
           );
         })
      |> CCArray.fold_left(
           ((runtime, scope, types), (name, is_optional, t)) => {
             switch (try_to_resolve_type(~runtime, ~scope, t)) {
             | None => (runtime, scope, types)
             | Some((runtime, scope, res)) =>
               let res_param =
                 Parameter({
                   name: Identifier.PropertyName(name),
                   is_optional,
                   type_: res,
                   named: is_optional,
                 });
               (runtime, scope, CCArray.append(types, [|res_param|]));
             }
           },
           (runtime, scope, [||]),
         );

    let (runtime, scope, resolved_return_type) =
      try_to_resolve_type(~runtime, ~scope, signature#getReturnType())
      |> CCOpt.value(~default=(runtime, scope, Basic(Any)));

    Some((
      runtime,
      scope,
      Function({
        return_type: resolved_return_type,
        parameters: resolved_parameters,
      }),
    ));
  } else {
    if (runtime.warnings) {
      Console.warn(
        Printexc.to_string(
          Exceptions.FeatureMissing("Type not implemented", t#getText()),
        ),
      );
    };
    None;
  };

let rec follow_references_from_resolved =
        (~runtime, ~scope, node: Node.node(Node.Constraint.any)) => {
  switch (node) {
  | TypeDeclaration({annot, _}) =>
    follow_references_from_resolved(
      ~runtime,
      ~scope,
      annot |> Node.Escape.toAny,
    )
  | GenericReference(TypeParameter(name)) =>
    scope
    |> Context.get_arg(name)
    |> CCOpt.flat_map(arg =>
         follow_references_from_resolved(
           ~runtime,
           ~scope,
           arg |> Node.Escape.toAny,
         )
       )
  | Reference({target, params}) =>
    // Make the parameters available
    let scope = scope |> Context.add_arg_lst(params);
    switch (
      scope.root_declarations
      |> CCArray.find_idx(decl => {
           switch (decl) {
           // TODO: Follow modules if obj_path is longer than one
           | TypeDeclaration({name, _}) =>
             Path.eq(target, [|name |> Identifier.Escape.toAny|])
           | _ => false
           }
         })
    ) {
    | None => None
    | Some((_, node)) =>
      follow_references_from_resolved(
        ~runtime,
        ~scope,
        node |> Node.Escape.toAny,
      )
    };
  | other => Some((runtime, scope, other))
  };
};

let extract_from_resolved =
    (
      ~wrap_sub_node,
      ~runtime,
      ~scope,
      path,
      access_fields: array(array(string)),
    ) => {
  // A few possibilities:
  let path_no_root = path |> Path.strip_root_module;

  let rec resolve_from_root_fields =
          (~runtime, ~scope, obj_path, access_fields: array(array(string))) => {
    let path_eq = Path.eq(obj_path);

    let root_obj =
      scope.root_declarations
      |> CCArray.find_idx(decl => {
           switch (decl) {
           // TODO: Follow modules if obj_path is longer than one
           | TypeDeclaration({name, _}) =>
             path_eq([|name |> Identifier.Escape.toAny|])
           | _ => false
           }
         });
    switch (root_obj) {
    | Some((_, TypeDeclaration({annot: Reference({target, _}), _}))) =>
      resolve_from_root_fields(~runtime, ~scope, target, access_fields)
    | Some((_, TypeDeclaration({annot: Record(fields), path, _}))) =>
      let access_field = CCArray.get(access_fields, 0);
      let access_fields =
        CCArray.sub(access_fields, 1, CCArray.length(access_fields) - 1);

      let fields_found =
        fields
        |> CCArray.mapi((i, field) =>
             switch (field) {
             | Parameter({name: PropertyName(pName), _})
                 when
                   access_field
                   |> CCArray.find_idx(CCString.equal(pName, _))
                   |> CCOpt.is_some =>
               Some((i, pName, field))
             | _ => None
             }
           )
        |> CCArray.filter_map(a => a);
      switch (fields_found, access_fields) {
      | ([|(_, _, Parameter({type_: Basic(_) as t, _}))|], _) =>
        Some((runtime, scope, t))
      | (fields_found, [||]) when CCArray.length(fields_found) > 1 =>
        // TODO: Abstract this next part as it's doubled one step further down
        let (runtime, scope, members) =
          fields_found
          |> CCArray.fold_left(
               ((runtime, scope, acc), field) =>
                 switch (field) {
                 | (field_idx, pName, Parameter({type_: t, _} as type_)) =>
                   let (scope, restore) =
                     scope
                     |> Scope.retain_path(
                          path |> Path.add(Identifier.SubName(pName)),
                        );
                   let (runtime, scope, new_ref) =
                     wrap_sub_node(
                       ~before=?CCArray.get_safe(obj_path, 0),
                       ~runtime,
                       ~scope,
                       t,
                     );
                   let new_ref_path =
                     Path.make_current_scope(scope.path)
                     |> Path.add(
                          TypeName(scope.path |> Path.make_sub_type_name),
                        );
                   let new_ref = new_ref |> Node.Escape.toAny;
                   let scope = restore(scope);
                   // Important to set another ref here to the extracted type
                   let scope =
                     scope |> Scope.add_ref(new_ref_path, scope.path);
                   CCArray.set(
                     fields,
                     field_idx,
                     Parameter({
                       ...type_,
                       type_: new_ref |> Node.Escape.toAssignable,
                     }),
                   );
                   (runtime, scope, CCArray.append(acc, [|new_ref|]));
                 | _ => (runtime, scope, acc)
                 },
               (runtime, scope, [||]),
             );
        switch (members) {
        | [||] => None
        | more =>
          // more |> CCArray.iter(a => Console.log(Pp.ast_node(a)));

          let (runtime, scope, union) =
            Parser_union.generate_ast_for_union(
              ~runtime,
              ~scope,
              more |> CCArray.map(Node.Escape.toAssignable),
            );
          Some((runtime, scope, union |> Node.Escape.toAny));
        };

      | ([|(field_idx, pName, Parameter({type_: t, _} as type_))|], [||]) =>
        let (scope, restore) =
          scope
          |> Scope.retain_path(path |> Path.add(Identifier.SubName(pName)));
        let (runtime, scope, new_ref) =
          wrap_sub_node(
            ~before=?CCArray.get_safe(obj_path, 0),
            ~runtime,
            ~scope,
            t,
          );
        let new_ref_path =
          Path.make_current_scope(scope.path)
          |> Path.add(TypeName(scope.path |> Path.make_sub_type_name));
        let new_ref = new_ref |> Node.Escape.toAny;
        let scope = restore(scope);
        // Important to set another ref here to the extracted type
        let scope = scope |> Scope.add_ref(new_ref_path, scope.path);
        CCArray.set(
          fields,
          field_idx,
          Parameter({...type_, type_: new_ref |> Node.Escape.toAssignable}),
        );
        Some((runtime, scope, new_ref));
      | ([|(_, _, Parameter({type_: Reference({target, _}), _}))|], rest) =>
        resolve_from_root_fields(~runtime, ~scope, target, rest)
      | _ => None
      };
    | Some(_)
    | None => None
    };
  };
  resolve_from_root_fields(~runtime, ~scope, path_no_root, access_fields);
};

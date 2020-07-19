open Ast;
open Node;
open Parser_utils;

// TODO:
// !!!!!!!!!!!!!!!!!!!
// Mapped types should only be parsed if all type args can be resolved
// through context!! So if we have a mapped type WITH type params,
// that stands alone we should ignore it for now and wait for it being
// called through a reference that passes the necessary arguments
// !!!!!!!!!!!!!!!!!!!

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
  Console.log(t#getKindName());
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
    |> CCOpt.flat_map(n => {
         Console.log(n#getKindName());
         raw_remove_references(n);
       });
  | _ => Some(t)
  };
};

let rec try_to_resolve_type = (~runtime, ~scope, t: Ts_nodes.Type.t) =>
  // Todo: Eventually maybe 'isEnumLiteral'
  // Todo: Eventually 'isClass'
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
    |> CCOpt.flat_map(inner => try_to_resolve_type(~runtime, ~scope, inner));
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
          Parser_union.determine_union_type(next),
        )
        |> CCOpt.map(((runtime, scope, next)) =>
             (runtime, scope, Node.Optional(next))
           )
      | Some(Nullable(next)) =>
        resolve_union(
          ~runtime,
          ~scope,
          Parser_union.determine_union_type(next),
        )
        |> CCOpt.map(((runtime, scope, next)) =>
             (runtime, scope, Node.Nullable(next))
           )
      | Some(Single(t)) => Some((runtime, scope, t))
      | Some(StringLiteral(literals)) =>
        Some((
          runtime,
          scope,
          Parser_generators.generate_string_literal_list(literals),
        ))
      | Some(NumericLiteral(literals)) =>
        Some((
          runtime,
          scope,
          Parser_generators.generate_number_literal_list(literals),
        ))
      | Some(MixedLiteral(literals)) =>
        raise(Exceptions.FeatureMissing("MixedLiteral", ""))
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
      Parser_union.determine_union_type(types),
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
  } else {
    Console.error(
      Printexc.to_string(
        Exceptions.FeatureMissing("Type not implemented", t#getText()),
      ),
    );
    None;
  };

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

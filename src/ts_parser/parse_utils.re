open Typescript_t;

let typeParametersOfNode = (node: node) => {
  switch (node) {
  | `FunctionDeclaration({typeParameters: Some(tp), _})
  | `TypeAliasDeclaration({typeParameters: Some(tp), _})
  | `InterfaceDeclaration({typeParameters: Some(tp), _})
  | `MethodSignature({typeParameters: Some(tp), _})
  | `FunctionType({typeParameters: Some(tp), _}) => tp
  //| `ClassDeclaration({ typeParameters: Some(tp) })
  | _ => []
  };
};

let rec identifierOfNode = (node: node) =>
  switch (node) {
  | `FunctionDeclaration(({name: Some(name), _}: node_FunctionDeclaration))
  | `InterfaceDeclaration({name, _})
  | `TypeAliasDeclaration({name, _})
  | `EnumDeclaration({name, _})
  //| `ClassDeclaration({ name, _ })
  | `TypeParameter(({name, _}: node_TypeParameter)) =>
    identifierOfNode(name)
  | `Identifier({escapedText: ident, _}) => ident
  | other =>
    switch (Typescript_unwrap.unwrap_Node(other).resolvedSymbol) {
    | None => raise(Not_found)
    | Some(symbol) => symbol.name
    }
  };

let typeParametersOfNodeRec = (node: node) => {
  let rec walk = (~params=[], node: node) => {
    switch (node) {
    | `TypeAliasDeclaration({typeParameters: tp, type_, _}) =>
      let sub = walk(type_);
      params @ CCOpt.value(~default=[], tp) @ sub;
    | `InterfaceDeclaration({typeParameters: tp, members, _}) =>
      let sub = members |> CCList.map(p => walk(p)) |> CCList.concat;
      params @ CCOpt.value(~default=[], tp) @ sub;
    | `FunctionType({typeParameters: tp, parameters, _})
    | `MethodSignature({typeParameters: tp, parameters, _})
    | `FunctionDeclaration({typeParameters: tp, parameters, _}) =>
      let sub = parameters |> CCList.map(p => walk(p)) |> CCList.concat;
      params @ CCOpt.value(~default=[], tp) @ sub;
    | `TypeLiteral({members, _}) =>
      let sub = members |> CCList.map(p => walk(p)) |> CCList.concat;
      params @ sub;
    | `UnionType({types, _})
    | `IntersectionType({types, _}) =>
      let sub = types |> CCList.map(p => walk(p)) |> CCList.concat;
      params @ sub;
    | `ArrayType({elementType, _}) => walk(~params, elementType)
    | `PropertySignature({type_: Some(type_), _})
    | `Parameter({type_: Some(type_), _}) => walk(~params, type_)
    | `TypeReference(_) => params
    | n =>
      Console.log((n |> Typescript_unwrap.unwrap_Node).kindName);
      params;
    };
  };
  walk(node)
  |> CCList.uniq(~eq=(a, b) =>
       Typescript_unwrap.(
         {
           let a = unwrap_Node(a);
           let b = unwrap_Node(b);
           a.pos === b.pos && a.end_ === b.end_;
         }
       )
     );
};

let getFullyQualifiedName = (node: node) =>
  Typescript_unwrap.unwrap_Node(node).resolvedSymbol
  |> CCOpt.map(symbol => symbol.fullyQualifiedName);

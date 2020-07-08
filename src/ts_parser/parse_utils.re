open Typescript_t;

let rec typeParametersOfNode = (node: node) => {
  switch (node) {
  | `FunctionDeclaration({typeParameters: Some(tp), _})
  | `TypeAliasDeclaration({typeParameters: Some(tp), _})
  | `InterfaceDeclaration({typeParameters: Some(tp), _})
  | `MethodSignature({typeParameters: Some(tp), _})
  | `FunctionType({typeParameters: Some(tp), _}) => tp
  //| `ClassDeclaration({ typeParameters: Some(tp) })
  | _ => []
  };
} and identifierOfNode = (node: node) =>
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
  } and typeParametersOfNodeRec = (~statements: option(list(node))=?, node: node) => {
  let rec walk = (~params=[], node: node) => {
    switch (node) {
    | `TypeAliasDeclaration({typeParameters: tp, type_, _}) =>
      let sub = walk(type_);
      CCOpt.value(~default=[], tp) @ params @ sub;
    | `InterfaceDeclaration({typeParameters: tp, members, _}) =>
      let sub = members |> CCList.map(p => walk(p)) |> CCList.concat;
      CCOpt.value(~default=[], tp) @ params @ sub;
    | `FunctionType({typeParameters: tp, parameters, _})
    | `MethodSignature({typeParameters: tp, parameters, _})
    | `FunctionDeclaration({typeParameters: tp, parameters, _}) =>
      let sub = parameters |> CCList.map(p => walk(p)) |> CCList.concat;
      CCOpt.value(~default=[], tp) @ params @ sub;
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
    | `TypeReference({typeArguments: Some(typeArguments), typeName, _}) =>
      let sub = switch (statements, Typescript_unwrap.unwrap_Node(typeName).resolvedSymbol) {
        | (None, _) => []
        | (Some(statements), Some({id: Some(id), _})) =>
          findNodeForSymbolId(statements, id)
          |> CCOpt.map(walk)
          |> CCOpt.value(~default=[]);
        | _ => []
      };

      let sub = sub @ typeArguments |> CCList.map(p => walk(p)) |> CCList.concat;
      params @ sub;
    | `TypeReference(
                 (
                   {
                     resolvedType: Some(`TypeParameter(_)),
                     flags,
                     typeName,
                     pos,
                     end_,
                     _,
                   }: node_TypeReference
                 ),
               ) =>
               
                let sub = switch (statements, Typescript_unwrap.unwrap_Node(typeName).resolvedSymbol) {
                  | (None, _) => []
                  | (Some(statements), Some({id: Some(id), _})) =>
                    findNodeForSymbolId(statements, id)
                    |> CCOpt.map(walk)
                    |> CCOpt.value(~default=[]);
                  | _ => []
                };

               params @ sub @ [
                 `TypeParameter({
                   pos,
                   end_,
                   name: typeName,
                   constraint_: None,
                   kindName: "TypeParameter",
                   kind: (-1),
                   default: None,
                   expression: None,
                   flags,
                   modifiers: None,
                   decorators: None,
                   resolvedSymbol: None,
                   resolvedType: None,
                 }),
               ]
    | `TypeReference({typeName, _}) =>
      let sub = switch (statements, Typescript_unwrap.unwrap_Node(typeName).resolvedSymbol) {
        | (None, _) => []
        | (Some(statements), Some({id: Some(id), _})) =>
          findNodeForSymbolId(statements, id)
          |> CCOpt.map(walk)
          |> CCOpt.value(~default=[]);
        | _ => []
      };
      params @ sub
    | `TypeParameter(_) as tp => params @ [tp]
    | _ => params
    };
  };
  walk(node)
      |> CCList.rev
      |> CCList.uniq(
        ~eq=(a, b) =>
           Typescript_unwrap.(
             {
               identifierOfNode(a) == identifierOfNode(b)
               || {
                 let a = unwrap_Node(a);
                 let b = unwrap_Node(b);
                 a.pos === b.pos && a.end_ === b.end_;
               };
             }
           )
         )
      |> CCList.rev;
} and findNodeForSymbolId = (statements: list(node), symbolId: int) => {
  statements
  |> CCList.find_opt(stmnt =>
       switch (stmnt) {
       | `FunctionDeclaration(
           (
             {resolvedSymbol: Some({id: Some(id), _}), _}: node_FunctionDeclaration
           ),
         )
       | `TypeAliasDeclaration(
           (
             {resolvedSymbol: Some({id: Some(id), _}), _}: node_TypeAliasDeclaration
           ),
         )
       | `InterfaceDeclaration(
           (
             {resolvedSymbol: Some({id: Some(id), _}), _}: node_InterfaceDeclaration
           ),
         ) =>
         id === symbolId
       | _ => false
       //| `ClassDeclaration({ resolvedSymbol: Some({id: Some(id),_}) })
       }
     );
} and getFullyQualifiedName = (node: node) =>
  Typescript_unwrap.unwrap_Node(node).resolvedSymbol
  |> CCOpt.map(symbol => symbol.fullyQualifiedName);

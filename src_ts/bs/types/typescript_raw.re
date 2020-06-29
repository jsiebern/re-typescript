type node;
type symbol;
type type_;

module Node = {
  [@bs.set]
  external setResolvedSymbol: (node, symbol) => unit = "resolvedSymbol";
  [@bs.set] external setResolvedType: (node, type_) => unit = "resolvedType";
  [@bs.set] external setKindName: (node, string) => unit = "kindName";

  [@bs.send] external isEnumDeclaration: node => bool = "isEnumDeclaration";
  [@bs.get] external kind: node => int = "kind";
};

module Symbol = {
  [@bs.set] external setResolvedType: (symbol, type_) => unit = "resolvedType";
  [@bs.set]
  external setFullyQualifiedName: (symbol, string) => unit =
    "fullyQualifiedName";
  [@bs.set] external setName: (symbol, string) => unit = "name";

  [@bs.get]
  external valueDeclaration: symbol => option(Js.Nullable.t(node)) =
    "valueDeclaration";
  [@bs.send] external getName: symbol => string = "getName";
};

module Type = {
  [@bs.set] external setKindName: (type_, string) => unit = "kindName";
  [@bs.set] external setNodeKind: (type_, string) => unit = "nodeKind";

  [@bs.get]
  external target: type_ => option(Js.Nullable.t(type_)) = "target";
  [@bs.get]
  external symbol: type_ => option(Js.Nullable.t(symbol)) = "symbol";
  [@bs.get] external flags: type_ => Typescript_flags.Type.t = "flags";
  [@bs.get]
  external objectFlags: type_ => Typescript_flags.Object.t = "objectFlags";
  [@bs.send] external isLiteral: type_ => bool = "isLiteral";
  [@bs.send] external isUnion: type_ => bool = "isUnion";
  [@bs.send] external isStringLiteral: type_ => bool = "isStringLiteral";
  [@bs.send] external isClass: type_ => bool = "isClass";
  [@bs.send] external isClassOrInterface: type_ => bool = "isClassOrInterface";
  [@bs.send] external isTypeParameter: type_ => bool = "isTypeParameter";
  [@bs.send] external isIntersection: type_ => bool = "isIntersection";
  [@bs.send]
  external isUnionOrIntersection: type_ => bool = "isUnionOrIntersection";
};

module Node: {
  class t:
    (Ojs.t) =>
    {
      inherit Ojs.obj;
      pub isEnumDeclaration: unit => bool;
      pub kind: Ojs.t;
    };

  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module MappedTypeNode: {
  class t:
    (Ojs.t) =>
    {
      inherit Node.t;
      pub readonlyToken: option(Node.t);
      pub questionToken: option(Node.t);
    };
  [@js.cast]
  let fromNode: Node.t => t;
  [@js.cast]
  let toNode: t => Node.t;

  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module TypeOperatorNode: {
  class t:
    (Ojs.t) =>
    {
      inherit Node.t;
      pub operator: Ojs.t;
    };
  [@js.cast]
  let fromNode: Node.t => t;
  [@js.cast]
  let toNode: t => Node.t;

  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module Symbol: {
  class t:
    (Ojs.t) =>
    {
      inherit Ojs.obj;
      pub valueDeclaration: unit => option(Node.t);
      pub getName: unit => string;
    };

  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module Type: {
  class t:
    (Ojs.t) =>
    {
      inherit Ojs.obj;
      pub target: option(t);
      pub id: option(int);
      pub flags: option(int);
      pub objectFlags: option(int);
      pub symbol: option(Symbol.t);
      // Type checks
      pub isLiteral: unit => bool;
      pub isUnion: unit => bool;
      pub isStringLiteral: unit => bool;
      pub isClass: unit => bool;
      pub isClassOrInterface: unit => bool;
      pub isTypeParameter: unit => bool;
      pub isIntersection: unit => bool;
      pub isUnionOrIntersection: unit => bool;
    };

  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module TypeChecker: {
  class t:
    (Ojs.t) =>
    {
      inherit Ojs.obj;
      pub getSymbolAtLocation: Node.t => option(Symbol.t);
      pub getDeclaredTypeOfSymbol: Symbol.t => option(Type.t);
      pub getTypeArguments: Type.t => option(array(Type.t));
      pub typeToString: Type.t => string;
      pub typeToTypeNode: Type.t => Node.t;
    };

  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};

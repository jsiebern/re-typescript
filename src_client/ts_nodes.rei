module Generic: {
  class t:
    (Ojs.t) =>
    {
      inherit Ts_morph.Node.t;
    };
  [@js.cast]
  let fromMorphNode: Ts_morph.Node.t => t;
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module TypeAliasDeclaration: {
  class t:
    (Ojs.t) =>
    {
      inherit Ts_morph.Node.t;
      pub getName: unit => string;
      pub getTypeNode: unit => Generic.t;
    };
  [@js.cast]
  let fromGeneric: Generic.t => t;
  [@js.cast]
  let toGeneric: t => Generic.t;
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module Parameter: {
  class t:
    (Ojs.t) =>
    {
      inherit Ts_morph.Node.t;
      pub getName: unit => string;
      pub getTypeNode: unit => option(Generic.t);
      pub getQuestionTokenNode: unit => option(Generic.t);
      pub getDotDotDotToken: unit => option(Generic.t);
      pub isRestParameter: unit => bool;
      pub isParameterProperty: unit => bool;
      pub isOptional: unit => bool;
    };
  [@js.cast]
  let fromGeneric: Generic.t => t;
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module FunctionDeclaration: {
  class t:
    (Ojs.t) =>
    {
      inherit Ts_morph.Node.t;
      pub getName: unit => option(string);
      pub isDefaultExport: unit => bool;
      pub getReturnTypeNode: unit => option(Generic.t);
      pub getTypeParameters: unit => array(Generic.t);
      pub getParameters: unit => array(Parameter.t);
    };
  [@js.cast]
  let fromGeneric: Generic.t => t;
  [@js.cast]
  let toGeneric: t => Generic.t;
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module EnumMember: {
  class t:
    (Ojs.t) =>
    {
      inherit Ts_morph.Node.t;
      pub getName: unit => string;
      pub getInitializer: unit => Generic.t;
    };
  [@js.cast]
  let fromGeneric: Generic.t => t;
  [@js.cast]
  let toGeneric: t => Generic.t;
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module EnumDeclaration: {
  class t:
    (Ojs.t) =>
    {
      inherit Ts_morph.Node.t;
      pub getName: unit => string;
      pub getMembers: unit => array(EnumMember.t);
    };
  [@js.cast]
  let fromGeneric: Generic.t => t;
  [@js.cast]
  let toGeneric: t => Generic.t;
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module SourceFile: {
  class t:
    (Ojs.t) =>
    {
      inherit Ts_morph.SourceFile.t;
      pub getStatements: unit => array(Generic.t);
    };
  [@js.cast]
  let fromGeneric: Generic.t => t;
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module Identifier: {
  class t:
    (Ojs.t) =>
    {
      inherit Ts_morph.Node.t;
      pub getImplementations: unit => Generic.t;
    };
  [@js.cast]
  let fromGeneric: Generic.t => t;
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module Array: {
  class t:
    (Ojs.t) =>
    {
      inherit Ts_morph.Node.t;
      pub getElementTypeNode: unit => Generic.t;
    };
  [@js.cast]
  let fromGeneric: Generic.t => t;
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module Tuple: {
  class t:
    (Ojs.t) =>
    {
      inherit Ts_morph.Node.t;
      pub getElementTypeNodes: unit => array(Generic.t);
    };
  [@js.cast]
  let fromGeneric: Generic.t => t;
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};

module Identify: {
  [@js.sum "kindName"]
  type nodeKind =
    | [@js.arg "node"] [@js "TypeAliasDeclaration"] TypeAliasDeclaration(
        TypeAliasDeclaration.t,
      )
    | [@js.arg "node"] [@js "Identifier"] Identifier(Identifier.t)
    | [@js.arg "node"] [@js "SourceFile"] SourceFile(SourceFile.t)
    | [@js.arg "node"] [@js "TypeParameter"] TypeParameter(Generic.t)
    | [@js.arg "node"] [@js "NamedDeclaration"] NamedDeclaration(Generic.t)
    | [@js.arg "node"] [@js "HeritageClause"] HeritageClause(Generic.t)
    | [@js.arg "node"] [@js "TypeElement"] TypeElement(Generic.t)
    | [@js.arg "node"] [@js "StringKeyword"] StringKeyword(Generic.t)
    | [@js.arg "node"] [@js "NumberKeyword"] NumberKeyword(Generic.t)
    | [@js.arg "node"] [@js "NeverKeyword"] NeverKeyword(Generic.t)
    | [@js.arg "node"] [@js "ObjectKeyword"] ObjectKeyword(Generic.t)
    | [@js.arg "node"] [@js "UndefinedKeyword"] UndefinedKeyword(Generic.t)
    | [@js.arg "node"] [@js "BooleanKeyword"] BooleanKeyword(Generic.t)
    | [@js.arg "node"] [@js "VoidKeyword"] VoidKeyword(Generic.t)
    | [@js.arg "node"] [@js "AnyKeyword"] AnyKeyword(Generic.t)
    | [@js.arg "node"] [@js "NullKeyword"] NullKeyword(Generic.t)
    | [@js.arg "node"] [@js "VariableDeclarationList"] VariableDeclarationList(
        Generic.t,
      )
    | [@js.arg "node"] [@js "VariableDeclaration"] VariableDeclaration(
        Generic.t,
      )
    | [@js.arg "node"] [@js "TypeLiteral"] TypeLiteral(Generic.t)
    | [@js.arg "node"] [@js "LiteralType"] LiteralType(Generic.t)
    | [@js.arg "node"] [@js "PropertySignature"] PropertySignature(Generic.t)
    | [@js.arg "node"] [@js "TypeOperator"] TypeOperator(Generic.t)
    | [@js.arg "node"] [@js "NodeWithTypeArguments"] NodeWithTypeArguments(
        Generic.t,
      )
    | [@js.arg "node"] [@js "TypeReference"] TypeReference(Generic.t)
    | [@js.arg "node"] [@js "QualifiedName"] QualifiedName(Generic.t)
    | [@js.arg "node"] [@js "EnumDeclaration"] EnumDeclaration(
        EnumDeclaration.t,
      )
    | [@js.arg "node"] [@js "EnumMember"] EnumMember(EnumMember.t)
    | [@js.arg "node"] [@js "UnionType"] UnionType(Generic.t)
    | [@js.arg "node"] [@js "TupleType"] TupleType(Tuple.t)
    | [@js.arg "node"] [@js "NamedTupleMember"] NamedTupleMember(Generic.t)
    | [@js.arg "node"] [@js "RestType"] RestType(Generic.t)
    | [@js.arg "node"] [@js "OptionalType"] OptionalType(Generic.t)
    | [@js.arg "node"] [@js "SignatureDeclarationBase"]
      SignatureDeclarationBase(
        Generic.t,
      )
    | [@js.arg "node"] [@js "FunctionType"] FunctionType(Generic.t)
    | [@js.arg "node"] [@js "FunctionLikeDeclarationBase"]
      FunctionLikeDeclarationBase(
        Generic.t,
      )
    | [@js.arg "node"] [@js "ArrayType"] ArrayType(Array.t)
    | [@js.arg "node"] [@js "FunctionDeclaration"] FunctionDeclaration(
        FunctionDeclaration.t,
      )
    | [@js.arg "node"] [@js "MethodSignature"] MethodSignature(Generic.t)
    | [@js.arg "node"] [@js "IndexSignature"] IndexSignature(Generic.t)
    | [@js.arg "node"] [@js "CallSignature"] CallSignature(Generic.t)
    | [@js.arg "node"] [@js "Parameter"] Parameter(Parameter.t)
    | [@js.arg "node"] [@js "BindingElement"] BindingElement(Generic.t)
    | [@js.arg "node"] [@js "PropertyDeclaration"] PropertyDeclaration(
        Generic.t,
      )
    | [@js.arg "node"] [@js "IntersectionType"] IntersectionType(Generic.t)
    | [@js.arg "node"] [@js "IndexedAccessType"] IndexedAccessType(Generic.t)
    | [@js.arg "node"] [@js "ParenthesizedType"] ParenthesizedType(Generic.t)
    | [@js.arg "node"] [@js "LiteralLikeNode"] LiteralLikeNode(Generic.t)
    | [@js.arg "node"] [@js "InterfaceDeclaration"] InterfaceDeclaration(
        Generic.t,
      )
    | [@js.default] Other(Ojs.t);

  let nodeKind_of_js: Ojs.t => nodeKind;
  let nodeKind_to_js: nodeKind => Ojs.t;

  type genericWithKindName;
  [@js.builder]
  let makeGenericWithKindName:
    (~kindName: string, ~node: Generic.t, unit) => genericWithKindName;
  let genericWithKindName_of_js: Ojs.t => genericWithKindName;
  let genericWithKindName_to_js: genericWithKindName => Ojs.t;
};
type nodeKind = Identify.nodeKind;

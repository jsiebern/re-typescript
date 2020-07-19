module Generic: {
  class t:
    (Ojs.t) =>
    {
      inherit Ojs.obj;
      // Type checks
      pub isSourceFile: unit => bool;
      // Tree Traversal
      pub forEachDescendant: (t => unit) => unit;
      pub forEachChild: (t => unit) => unit;
      pub getChildren: unit => array(t);
      pub getDescendants: unit => array(t);
      pub getExportedDeclarations: unit => array(t);
      pub getChildCount: unit => int;
      // Type Information
      pub getText: unit => string;
      pub getKind: unit => int;
      pub getPos: unit => int;
      pub getEnd: unit => int;
      pub getStartLineNumber: unit => int;
      pub getEndLineNumber: unit => int;
      pub getKindName: unit => string;
      pub getSymbol: unit => option(Ts_morph.Symbol.t);
      pub getParent: unit => option(t);
      // Raw
      pub compilerNode: Ts_raw.Node.t;
      pub getProject: unit => Ts_morph.Project.t;
    };
  [@js.cast]
  let fromMorphNode: Ts_morph.Node.t => t;
  [@js.cast]
  let toMorphNode: t => Ts_morph.Node.t;
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module RootType: {
  class t:
    (Ojs.t) =>
    {
      inherit Ojs.obj;
      // Info
      pub getUnionTypes: unit => array(t);
      pub getArrayElementType: unit => option(t);
      pub getTupleElements: unit => array(t);
      pub getIntersectionTypes: unit => array(t);
      // Type Checks
      pub isAny: unit => bool;
      pub isArray: unit => bool;
      pub isBoolean: unit => bool;
      pub isString: unit => bool;
      pub isNumber: unit => bool;
      pub isLiteral: unit => bool;
      pub isBooleanLiteral: unit => bool;
      pub isUnion: unit => bool;
      pub isEnumLiteral: unit => bool;
      pub isNumberLiteral: unit => bool;
      pub isStringLiteral: unit => bool;
      pub isClass: unit => bool;
      pub isClassOrInterface: unit => bool;
      pub isInterface: unit => bool;
      pub isObject: unit => bool;
      pub isEnum: unit => bool;
      pub isTypeParameter: unit => bool;
      pub isTuple: unit => bool;
      pub isIntersection: unit => bool;
      pub isUnionOrIntersection: unit => bool;
      pub isUnknown: unit => bool;
      pub isNull: unit => bool;
      pub isUndefined: unit => bool;
      pub isAnonymous: unit => bool;
      // Raw
      pub getText: unit => string;
      pub compilerType: Ts_raw.Type.t;
    };

  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module Symbol: {
  [@js.enum]
  type flags =
    | TypeParameter
    | Optional;
  let flags_of_js: Ojs.t => flags;
  let flags_to_js: flags => Ojs.t;

  class t:
    (Ojs.t) =>
    {
      inherit Ojs.obj;
      // Type Information
      pub getName: unit => string;
      pub getFullyQualifiedName: unit => string;
      pub getMembers: unit => array(t);
      pub getValueDeclaration: unit => option(Generic.t);
      pub getDeclarations: unit => array(Generic.t);
      pub getTypeAtLocation: Generic.t => RootType.t;
      // Type checking
      pub isAlias: unit => bool;
      pub hasFlags: flags => bool;
      // Raw
      pub compilerSymbol: Ts_raw.Symbol.t;
    };
  [@js.cast]
  let fromMorphSymbol: Ts_morph.Symbol.t => t;
  [@js.cast]
  let toMorphSymbol: t => Ts_morph.Symbol.t;
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module Type: {
  class t:
    (Ojs.t) =>
    {
      inherit RootType.t;
      // Symbol based
      pub getSymbol: unit => option(Symbol.t);
      pub getProperties: unit => array(Symbol.t);
    };

  [@js.cast]
  let toRootType: t => RootType.t;
  [@js.cast]
  let fromRootType: RootType.t => t;
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module WithGetType: {
  class t:
    (Ojs.t) =>
    {
      inherit Generic.t;
      pub getType: unit => option(Type.t);
    };
  [@js.cast]
  let fromGeneric: Generic.t => t;
  [@js.cast]
  let toGeneric: t => Generic.t;
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module TypeParameter: {
  class t:
    (Ojs.t) =>
    {
      inherit Generic.t;
      pub getConstraint: unit => option(Generic.t);
      pub getDefault: unit => option(Generic.t);
      pub getName: unit => string;
    };
  [@js.cast]
  let fromGeneric: Generic.t => t;
  [@js.cast]
  let toGeneric: t => Generic.t;
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module TypeParametered: {
  class t:
    (Ojs.t) =>
    {
      inherit Generic.t;
      pub getTypeParameters: unit => array(TypeParameter.t);
    };
  [@js.cast]
  let fromGeneric: Generic.t => t;
  [@js.cast]
  let toGeneric: t => Generic.t;
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module TypeArgumented: {
  class t:
    (Ojs.t) =>
    {
      inherit Generic.t;
      pub getTypeArguments: unit => array(Generic.t);
    };
  [@js.cast]
  let fromGeneric: Generic.t => t;
  [@js.cast]
  let toGeneric: t => Generic.t;
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module TypeAliasDeclaration: {
  class t:
    (Ojs.t) =>
    {
      inherit TypeParametered.t;
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
      inherit Generic.t;
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
      inherit TypeParametered.t;
      pub getName: unit => option(string);
      pub isDefaultExport: unit => bool;
      pub getReturnTypeNode: unit => option(Generic.t);
      pub getParameters: unit => array(Parameter.t);
    };
  [@js.cast]
  let fromGeneric: Generic.t => t;
  [@js.cast]
  let toGeneric: t => Generic.t;
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module FunctionType: {
  class t:
    (Ojs.t) =>
    {
      inherit FunctionDeclaration.t;
    };
};
module MethodSignature: {
  class t:
    (Ojs.t) =>
    {
      inherit FunctionDeclaration.t;
      pub getQuestionTokenNode: unit => option(Generic.t);
    };
};
module CallSignature: {
  class t:
    (Ojs.t) =>
    {
      inherit FunctionType.t;
    };
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module Abstr_ExtendsNode: {
  class t:
    (Ojs.t) =>
    {
      inherit TypeArgumented.t;
      pub getType: unit => Type.t;
    };
};
module InterfaceDeclaration: {
  class t:
    (Ojs.t) =>
    {
      inherit TypeParametered.t;
      pub getMembers: unit => array(Generic.t);
      pub getName: unit => option(string);
      pub getNameNode: unit => Generic.t;
      pub isDefaultExport: unit => bool;
      pub getExtends: unit => array(Abstr_ExtendsNode.t);
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
      inherit Generic.t;
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
      inherit Generic.t;
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
      inherit Generic.t;
      pub getStatements: unit => array(Generic.t);
      pub saveSync: unit => unit;
      pub refreshFromFileSystemSync: unit => unit;
      pub getFilePath: unit => string;
      pub compilerNode: Ts_raw.Node.t;
      pub getBaseName: unit => string;
      pub getBaseNameWithoutExtension: unit => string;
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
      inherit Generic.t;
      pub getImplementations: unit => Generic.t;
    };
  [@js.cast]
  let fromGeneric: Generic.t => t;
  [@js.cast]
  let toGeneric: t => Generic.t;
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module Array: {
  class t:
    (Ojs.t) =>
    {
      inherit Generic.t;
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
      inherit Generic.t;
      pub getElementTypeNodes: unit => array(Generic.t);
    };
  [@js.cast]
  let fromGeneric: Generic.t => t;
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module TypeLiteral: {
  class t:
    (Ojs.t) =>
    {
      inherit Generic.t;
      pub getMembers: unit => array(Generic.t);
    };
  [@js.cast]
  let fromGeneric: Generic.t => t;
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module PropertySignature: {
  class t:
    (Ojs.t) =>
    {
      inherit Generic.t;
      pub getName: unit => string;
      pub isReadonly: unit => bool;
      pub getTypeNode: unit => option(Generic.t);
      pub getQuestionTokenNode: unit => option(Generic.t);
      pub getInitializer: unit => option(Generic.t);
    };
  [@js.cast]
  let fromGeneric: Generic.t => t;
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module UnionType: {
  class t:
    (Ojs.t) =>
    {
      inherit Generic.t;
      pub getTypeNodes: unit => array(Generic.t);
    };
  [@js.cast]
  let fromGeneric: Generic.t => t;
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module TypeReference: {
  class t:
    (Ojs.t) =>
    {
      inherit Generic.t;
      pub getTypeName: unit => Identifier.t;
      pub getTypeArguments: unit => array(Generic.t);
      pub getType: unit => option(Type.t);
    };
  [@js.cast]
  let fromGeneric: Generic.t => t;
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module IndexedAccessType: {
  class t:
    (Ojs.t) =>
    {
      inherit Generic.t;
      pub getObjectTypeNode: unit => Generic.t;
      pub getIndexTypeNode: unit => Generic.t;
      pub getType: unit => option(Type.t);
    };
  [@js.cast]
  let fromGeneric: Generic.t => t;
  [@js.cast]
  let toGeneric: t => Generic.t;
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module LiteralLike: {
  class t:
    (Ojs.t) =>
    {
      inherit Generic.t;
      pub getLiteralText: unit => string;
      pub getLiteralValue: unit => string;
    };
};
module StringLiteral: {
  class t:
    (Ojs.t) =>
    {
      inherit Generic.t;
      pub getLiteralValue: unit => string;
    };
};
module NumericLiteral: {
  class t:
    (Ojs.t) =>
    {
      inherit Generic.t;
      pub getLiteralValue: unit => float;
    };
};
module BooleanLiteral: {
  class t:
    (Ojs.t) =>
    {
      inherit Generic.t;
      pub getLiteralValue: unit => bool;
    };
};
module RegularExpressionLiteral: {
  class t:
    (Ojs.t) =>
    {
      inherit Generic.t;
      pub getLiteralValue: unit => string;
    };
};
module LiteralType: {
  class t:
    (Ojs.t) =>
    {
      inherit Generic.t;
      pub getLiteral: unit => Generic.t;
    };
  [@js.cast]
  let fromGeneric: Generic.t => t;
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module NamespaceDeclaration: {
  class t:
    (Ojs.t) =>
    {
      inherit Generic.t;
      pub getExportDeclarations: unit => array(Generic.t);
      pub getName: unit => string;
      pub getStatements: unit => array(Generic.t);
    };
};
module ModuleDeclaration: {
  class t:
    (Ojs.t) =>
    {
      inherit NamespaceDeclaration.t;
    };
};
module IntersectionType: {
  class t:
    (Ojs.t) =>
    {
      inherit Generic.t;
      pub getTypeNodes: unit => array(Generic.t);
    };
};
module MappedType: {
  class t:
    (Ojs.t) =>
    {
      inherit Generic.t;
    };
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module TypeOperator: {
  class t:
    (Ojs.t) =>
    {
      inherit WithGetType.t;
    };
  [@js.cast]
  let toGeneric: t => Generic.t;
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};

module Identify: {
  [@js.sum "kindName"]
  type nodeKind =
    | [@js.arg "node"] [@js "TypeAliasDeclaration"] TypeAliasDeclaration(
        TypeAliasDeclaration.t,
      )
    | [@js.arg "node"] [@js "NamespaceDeclaration"] NamespaceDeclaration(
        NamespaceDeclaration.t,
      )
    | [@js.arg "node"] [@js "ModuleDeclaration"] ModuleDeclaration(
        ModuleDeclaration.t,
      )
    | [@js.arg "node"] [@js "Identifier"] Identifier(Identifier.t)
    | [@js.arg "node"] [@js "SourceFile"] SourceFile(SourceFile.t)
    | [@js.arg "node"] [@js "TypeParameter"] TypeParameter(TypeParameter.t)
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
    | [@js.arg "node"] [@js "TypeLiteral"] TypeLiteral(TypeLiteral.t)
    | [@js.arg "node"] [@js "LiteralType"] LiteralType(LiteralType.t)
    | [@js.arg "node"] [@js "PropertySignature"] PropertySignature(
        PropertySignature.t,
      )
    | [@js.arg "node"] [@js "MappedType"] MappedType(MappedType.t)
    | [@js.arg "node"] [@js "TypeOperator"] TypeOperator(TypeOperator.t)
    | [@js.arg "node"] [@js "NodeWithTypeArguments"] NodeWithTypeArguments(
        Generic.t,
      )
    | [@js.arg "node"] [@js "TypeReference"] TypeReference(TypeReference.t)
    | [@js.arg "node"] [@js "QualifiedName"] QualifiedName(Generic.t)
    | [@js.arg "node"] [@js "EnumDeclaration"] EnumDeclaration(
        EnumDeclaration.t,
      )
    | [@js.arg "node"] [@js "EnumMember"] EnumMember(EnumMember.t)
    | [@js.arg "node"] [@js "UnionType"] UnionType(UnionType.t)
    | [@js.arg "node"] [@js "TupleType"] TupleType(Tuple.t)
    | [@js.arg "node"] [@js "NamedTupleMember"] NamedTupleMember(Generic.t)
    | [@js.arg "node"] [@js "RestType"] RestType(Generic.t)
    | [@js.arg "node"] [@js "OptionalType"] OptionalType(Generic.t)
    | [@js.arg "node"] [@js "StringLiteral"] StringLiteral(StringLiteral.t)
    | [@js.arg "node"] [@js "NumericLiteral"] NumericLiteral(NumericLiteral.t)
    | [@js.arg "node"] [@js "BooleanLiteral"] BooleanLiteral(BooleanLiteral.t)
    | [@js.arg "node"] [@js "BigIntLiteral"] BigIntLiteral(Generic.t)
    | [@js.arg "node"] [@js "RegularExpressionLiteral"]
      RegularExpressionLiteral(
        RegularExpressionLiteral.t,
      )
    | [@js.arg "node"] [@js "NullLiteral"] NullLiteral(Generic.t)
    | [@js.arg "node"] [@js "CallSignatureDeclaration"]
      CallSignatureDeclaration(
        Generic.t,
      )
    | [@js.arg "node"] [@js "IndexSignatureDeclaration"]
      IndexSignatureDeclaration(
        Generic.t,
      )
    | [@js.arg "node"] [@js "ConstructSignatureDeclaration"]
      ConstructSignatureDeclaration(
        Generic.t,
      )
    | [@js.arg "node"] [@js "SignatureDeclarationBase"]
      SignatureDeclarationBase(
        Generic.t,
      )
    | [@js.arg "node"] [@js "FunctionType"] FunctionType(FunctionType.t)
    | [@js.arg "node"] [@js "FunctionLikeDeclarationBase"]
      FunctionLikeDeclarationBase(
        Generic.t,
      )
    | [@js.arg "node"] [@js "ArrayType"] ArrayType(Array.t)
    | [@js.arg "node"] [@js "FunctionDeclaration"] FunctionDeclaration(
        FunctionDeclaration.t,
      )
    | [@js.arg "node"] [@js "MethodSignature"] MethodSignature(
        MethodSignature.t,
      )
    | [@js.arg "node"] [@js "IndexSignature"] IndexSignature(Generic.t)
    | [@js.arg "node"] [@js "CallSignature"] CallSignature(CallSignature.t)
    | [@js.arg "node"] [@js "Parameter"] Parameter(Parameter.t)
    | [@js.arg "node"] [@js "BindingElement"] BindingElement(Generic.t)
    | [@js.arg "node"] [@js "PropertyDeclaration"] PropertyDeclaration(
        Generic.t,
      )
    | [@js.arg "node"] [@js "IntersectionType"] IntersectionType(
        IntersectionType.t,
      )
    | [@js.arg "node"] [@js "IndexedAccessType"] IndexedAccessType(
        IndexedAccessType.t,
      )
    | [@js.arg "node"] [@js "ParenthesizedType"] ParenthesizedType(Generic.t)
    | [@js.arg "node"] [@js "LiteralLikeNode"] LiteralLikeNode(Generic.t)
    | [@js.arg "node"] [@js "InterfaceDeclaration"] InterfaceDeclaration(
        InterfaceDeclaration.t,
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

module NodeStatic: {
  class t:
    (Ojs.t) =>
    {
      inherit Ojs.obj;
      pub isSignaturedDeclaration: Generic.t => bool;
    };
};

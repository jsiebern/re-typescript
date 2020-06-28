(* Auto-generated from "typescript.atd" *)
              [@@@ocaml.warning "-27-32-35-39"]

type type_unknown = Typescript_t.type_unknown = { flags: int }

type node = Typescript_t.node

and node_Generic = Typescript_t.node_Generic = {
  pos: int;
  end_: int;
  kind: int;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option
}

and node_HeritageClause = Typescript_t.node_HeritageClause = {
  pos: int;
  end_: int;
  kind: int;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  token: int;
  types: node list option
}

and node_Identifier = Typescript_t.node_Identifier = {
  pos: int;
  end_: int;
  kind: int;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  escapedText: string;
  originalKeywordKind: int option
}

and node_InterfaceDeclaration = Typescript_t.node_InterfaceDeclaration = {
  pos: int;
  end_: int;
  kind: int;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  name: node;
  typeParameters: node list option;
  heritageClauses: node list option;
  members: node list
}

and node_PropertySignature = Typescript_t.node_PropertySignature = {
  pos: int;
  end_: int;
  kind: int;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  name: node;
  questionToken: node option;
  type_: node option;
  initializer_: node option
}

and node_SourceFile = Typescript_t.node_SourceFile = {
  pos: int;
  end_: int;
  kind: int;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  statements: node list;
  text: string;
  languageVersion: int;
  fileName: string;
  languageVariant: int;
  isDeclarationFile: bool;
  nodeCount: int;
  identifierCount: int
}

and node_TypeAliasDeclaration = Typescript_t.node_TypeAliasDeclaration = {
  pos: int;
  end_: int;
  kind: int;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  name: node;
  typeParameters: node list option;
  type_: node
}

and node_TypeLiteral = Typescript_t.node_TypeLiteral = {
  pos: int;
  end_: int;
  kind: int;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  members: node list
}

and node_TypeOperator = Typescript_t.node_TypeOperator = {
  pos: int;
  end_: int;
  kind: int;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  operator: int;
  type_: node
}

and node_TypeParameter = Typescript_t.node_TypeParameter = {
  pos: int;
  end_: int;
  kind: int;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  name: node;
  constraint_: node option;
  default: node option;
  expression: node option
}

and node_TypeReference = Typescript_t.node_TypeReference = {
  pos: int;
  end_: int;
  kind: int;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  typeArguments: node list option;
  typeName: node
}

and node_VariableDeclaration = Typescript_t.node_VariableDeclaration = {
  pos: int;
  end_: int;
  kind: int;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  name: node;
  exclamationToken: node option;
  type_: node option;
  initializer_: node option
}

and node_VariableDeclarationList =
  Typescript_t.node_VariableDeclarationList = {
  pos: int;
  end_: int;
  kind: int;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  declarations: node list
}

and symbol = Typescript_t.symbol = {
  flags: int;
  id: int option;
  resolvedType: type_ option;
  fullyQualifiedName: string;
  name: string
}

and type_ = Typescript_t.type_

and type_AnonymousType = Typescript_t.type_AnonymousType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  objectFlags: int;
  target: type_ option
}

and type_InterfaceType = Typescript_t.type_InterfaceType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  objectFlags: int;
  typeParameters: type_ list option;
  outerTypeParameters: type_ list option;
  localTypeParameters: type_ list option;
  thisType: type_ option
}

and type_IntersectionType = Typescript_t.type_IntersectionType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  types: type_ list
}

and type_NumberLiteralType = Typescript_t.type_NumberLiteralType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  value: float
}

and type_ObjectType = Typescript_t.type_ObjectType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  objectFlags: int
}

and type_StringLiteralType = Typescript_t.type_StringLiteralType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  value: string
}

and type_TupleType = Typescript_t.type_TupleType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  minLength: int;
  fixedLength: int;
  hasRestElement: bool;
  readonly: bool;
  elementFlags: int list;
  combinedFlags: int;
  labeledElementDeclarations: node list option
}

and type_TypeParameter = Typescript_t.type_TypeParameter = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  constraint_: type_ option
}

and type_TypeReference = Typescript_t.type_TypeReference = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  objectFlags: int;
  target: type_
}

and type_UnionOrIntersectionType =
  Typescript_t.type_UnionOrIntersectionType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  types: type_ list
}

and type_UnionType = Typescript_t.type_UnionType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  types: type_ list
}

type conditionalRoot = Typescript_t.conditionalRoot = {
  node: node;
  checkType: type_;
  extendsType: type_;
  trueType: type_;
  falseType: type_;
  isDistributive: bool;
  inferTypeParameters: type_ list option;
  outerTypeParameters: type_ list option;
  instantiations: (string * type_) option;
  aliasSymbol: symbol option;
  aliasTypeArguments: type_ list option
}

type type_conditionalType = Typescript_t.type_conditionalType = {
  root: conditionalRoot;
  checkType: type_;
  extendsType: type_;
  resolvedTrueType: type_;
  resolvedFalseType: type_
}

type type_LiteralType = Typescript_t.type_LiteralType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option
}

type type_Generic = Typescript_t.type_Generic = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option
}

type readonlyTextRange = Typescript_t.readonlyTextRange = {
  pos: int;
  end_: int
}

type node_TypeElement = Typescript_t.node_TypeElement = {
  pos: int;
  end_: int;
  kind: int;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  questionToken: node option
}

type node_QualifiedName = Typescript_t.node_QualifiedName = {
  pos: int;
  end_: int;
  kind: int;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  left: node;
  right: node
}

type node_NodeWithTypeArguments = Typescript_t.node_NodeWithTypeArguments = {
  pos: int;
  end_: int;
  kind: int;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  typeArguments: node list option
}

type node_NamedDeclaration = Typescript_t.node_NamedDeclaration = {
  pos: int;
  end_: int;
  kind: int;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  name: node option
}

val read_type_unknown :  type_unknown Atdgen_codec_runtime.Decode.t

val write_type_unknown :  type_unknown Atdgen_codec_runtime.Encode.t

val read_node :  node Atdgen_codec_runtime.Decode.t

val write_node :  node Atdgen_codec_runtime.Encode.t

val read_node_Generic :  node_Generic Atdgen_codec_runtime.Decode.t

val write_node_Generic :  node_Generic Atdgen_codec_runtime.Encode.t

val read_node_HeritageClause :  node_HeritageClause Atdgen_codec_runtime.Decode.t

val write_node_HeritageClause :  node_HeritageClause Atdgen_codec_runtime.Encode.t

val read_node_Identifier :  node_Identifier Atdgen_codec_runtime.Decode.t

val write_node_Identifier :  node_Identifier Atdgen_codec_runtime.Encode.t

val read_node_InterfaceDeclaration :  node_InterfaceDeclaration Atdgen_codec_runtime.Decode.t

val write_node_InterfaceDeclaration :  node_InterfaceDeclaration Atdgen_codec_runtime.Encode.t

val read_node_PropertySignature :  node_PropertySignature Atdgen_codec_runtime.Decode.t

val write_node_PropertySignature :  node_PropertySignature Atdgen_codec_runtime.Encode.t

val read_node_SourceFile :  node_SourceFile Atdgen_codec_runtime.Decode.t

val write_node_SourceFile :  node_SourceFile Atdgen_codec_runtime.Encode.t

val read_node_TypeAliasDeclaration :  node_TypeAliasDeclaration Atdgen_codec_runtime.Decode.t

val write_node_TypeAliasDeclaration :  node_TypeAliasDeclaration Atdgen_codec_runtime.Encode.t

val read_node_TypeLiteral :  node_TypeLiteral Atdgen_codec_runtime.Decode.t

val write_node_TypeLiteral :  node_TypeLiteral Atdgen_codec_runtime.Encode.t

val read_node_TypeOperator :  node_TypeOperator Atdgen_codec_runtime.Decode.t

val write_node_TypeOperator :  node_TypeOperator Atdgen_codec_runtime.Encode.t

val read_node_TypeParameter :  node_TypeParameter Atdgen_codec_runtime.Decode.t

val write_node_TypeParameter :  node_TypeParameter Atdgen_codec_runtime.Encode.t

val read_node_TypeReference :  node_TypeReference Atdgen_codec_runtime.Decode.t

val write_node_TypeReference :  node_TypeReference Atdgen_codec_runtime.Encode.t

val read_node_VariableDeclaration :  node_VariableDeclaration Atdgen_codec_runtime.Decode.t

val write_node_VariableDeclaration :  node_VariableDeclaration Atdgen_codec_runtime.Encode.t

val read_node_VariableDeclarationList :  node_VariableDeclarationList Atdgen_codec_runtime.Decode.t

val write_node_VariableDeclarationList :  node_VariableDeclarationList Atdgen_codec_runtime.Encode.t

val read_symbol :  symbol Atdgen_codec_runtime.Decode.t

val write_symbol :  symbol Atdgen_codec_runtime.Encode.t

val read_type_ :  type_ Atdgen_codec_runtime.Decode.t

val write_type_ :  type_ Atdgen_codec_runtime.Encode.t

val read_type_AnonymousType :  type_AnonymousType Atdgen_codec_runtime.Decode.t

val write_type_AnonymousType :  type_AnonymousType Atdgen_codec_runtime.Encode.t

val read_type_InterfaceType :  type_InterfaceType Atdgen_codec_runtime.Decode.t

val write_type_InterfaceType :  type_InterfaceType Atdgen_codec_runtime.Encode.t

val read_type_IntersectionType :  type_IntersectionType Atdgen_codec_runtime.Decode.t

val write_type_IntersectionType :  type_IntersectionType Atdgen_codec_runtime.Encode.t

val read_type_NumberLiteralType :  type_NumberLiteralType Atdgen_codec_runtime.Decode.t

val write_type_NumberLiteralType :  type_NumberLiteralType Atdgen_codec_runtime.Encode.t

val read_type_ObjectType :  type_ObjectType Atdgen_codec_runtime.Decode.t

val write_type_ObjectType :  type_ObjectType Atdgen_codec_runtime.Encode.t

val read_type_StringLiteralType :  type_StringLiteralType Atdgen_codec_runtime.Decode.t

val write_type_StringLiteralType :  type_StringLiteralType Atdgen_codec_runtime.Encode.t

val read_type_TupleType :  type_TupleType Atdgen_codec_runtime.Decode.t

val write_type_TupleType :  type_TupleType Atdgen_codec_runtime.Encode.t

val read_type_TypeParameter :  type_TypeParameter Atdgen_codec_runtime.Decode.t

val write_type_TypeParameter :  type_TypeParameter Atdgen_codec_runtime.Encode.t

val read_type_TypeReference :  type_TypeReference Atdgen_codec_runtime.Decode.t

val write_type_TypeReference :  type_TypeReference Atdgen_codec_runtime.Encode.t

val read_type_UnionOrIntersectionType :  type_UnionOrIntersectionType Atdgen_codec_runtime.Decode.t

val write_type_UnionOrIntersectionType :  type_UnionOrIntersectionType Atdgen_codec_runtime.Encode.t

val read_type_UnionType :  type_UnionType Atdgen_codec_runtime.Decode.t

val write_type_UnionType :  type_UnionType Atdgen_codec_runtime.Encode.t

val read_conditionalRoot :  conditionalRoot Atdgen_codec_runtime.Decode.t

val write_conditionalRoot :  conditionalRoot Atdgen_codec_runtime.Encode.t

val read_type_conditionalType :  type_conditionalType Atdgen_codec_runtime.Decode.t

val write_type_conditionalType :  type_conditionalType Atdgen_codec_runtime.Encode.t

val read_type_LiteralType :  type_LiteralType Atdgen_codec_runtime.Decode.t

val write_type_LiteralType :  type_LiteralType Atdgen_codec_runtime.Encode.t

val read_type_Generic :  type_Generic Atdgen_codec_runtime.Decode.t

val write_type_Generic :  type_Generic Atdgen_codec_runtime.Encode.t

val read_readonlyTextRange :  readonlyTextRange Atdgen_codec_runtime.Decode.t

val write_readonlyTextRange :  readonlyTextRange Atdgen_codec_runtime.Encode.t

val read_node_TypeElement :  node_TypeElement Atdgen_codec_runtime.Decode.t

val write_node_TypeElement :  node_TypeElement Atdgen_codec_runtime.Encode.t

val read_node_QualifiedName :  node_QualifiedName Atdgen_codec_runtime.Decode.t

val write_node_QualifiedName :  node_QualifiedName Atdgen_codec_runtime.Encode.t

val read_node_NodeWithTypeArguments :  node_NodeWithTypeArguments Atdgen_codec_runtime.Decode.t

val write_node_NodeWithTypeArguments :  node_NodeWithTypeArguments Atdgen_codec_runtime.Encode.t

val read_node_NamedDeclaration :  node_NamedDeclaration Atdgen_codec_runtime.Decode.t

val write_node_NamedDeclaration :  node_NamedDeclaration Atdgen_codec_runtime.Encode.t


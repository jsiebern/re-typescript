(* Auto-generated from "typescript.atd" *)
              [@@@ocaml.warning "-27-32-35-39"]

type node = Typescript_t.node

and node_ArrayType = Typescript_t.node_ArrayType = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  elementType: node
}

and node_BindingElement = Typescript_t.node_BindingElement = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  propertyName: node option;
  dotDotDotToken: node option;
  name: node;
  initializer_: node option
}

and node_CallSignature = Typescript_t.node_CallSignature = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  name: node option;
  typeParameters: node list option;
  parameters: node list;
  type_: node option;
  typeArguments: node list option
}

and node_EnumDeclaration = Typescript_t.node_EnumDeclaration = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  name: node;
  members: node list
}

and node_EnumMember = Typescript_t.node_EnumMember = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  name: node;
  initializer_: node option
}

and node_FunctionDeclaration = Typescript_t.node_FunctionDeclaration = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeParameters: node list option;
  parameters: node list;
  type_: node option;
  typeArguments: node list option;
  asteriskToken: node option;
  questionToken: node option;
  exclamationToken: node option;
  name: node option;
  body: node option
}

and node_FunctionType = Typescript_t.node_FunctionType = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  name: node option;
  typeParameters: node list option;
  parameters: node list;
  type_: node option;
  typeArguments: node list option
}

and node_Generic = Typescript_t.node_Generic = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option
}

and node_HeritageClause = Typescript_t.node_HeritageClause = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  token: int;
  types: node list option
}

and node_Identifier = Typescript_t.node_Identifier = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  escapedText: string;
  originalKeywordKind: int option
}

and node_IndexSignature = Typescript_t.node_IndexSignature = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeParameters: node list option;
  parameters: node list;
  type_: node option;
  typeArguments: node list option;
  name: node option
}

and node_InterfaceDeclaration = Typescript_t.node_InterfaceDeclaration = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  name: node;
  typeParameters: node list option;
  heritageClauses: node list option;
  members: node list
}

and node_MethodSignature = Typescript_t.node_MethodSignature = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeParameters: node list option;
  parameters: node list;
  type_: node option;
  typeArguments: node list option;
  name: node option
}

and node_Parameter = Typescript_t.node_Parameter = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  dotDotDotToken: node option;
  name: node;
  questionToken: node option;
  type_: node option;
  initializer_: node option
}

and node_PropertyDeclaration = Typescript_t.node_PropertyDeclaration = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  name: node;
  questionToken: node option;
  exclamationToken: node option;
  type_: node option;
  initializer_: node option
}

and node_PropertySignature = Typescript_t.node_PropertySignature = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  name: node;
  questionToken: node option;
  type_: node option;
  initializer_: node option
}

and node_QualifiedName = Typescript_t.node_QualifiedName = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  left: node;
  right: node
}

and node_SourceFile = Typescript_t.node_SourceFile = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
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
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  name: node;
  typeParameters: node list option;
  type_: node
}

and node_TypeLiteral = Typescript_t.node_TypeLiteral = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  members: node list
}

and node_TypeOperator = Typescript_t.node_TypeOperator = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  operator: int;
  type_: node
}

and node_TypeParameter = Typescript_t.node_TypeParameter = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  name: node;
  constraint_: node option;
  default: node option;
  expression: node option
}

and node_TypeReference = Typescript_t.node_TypeReference = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeArguments: node list option;
  typeName: node
}

and node_UnionType = Typescript_t.node_UnionType = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  types: node list
}

and node_VariableDeclaration = Typescript_t.node_VariableDeclaration = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
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
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
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
  kindName: string;
  objectFlags: int;
  target: type_ option
}

and type_Generic = Typescript_t.type_Generic = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string
}

and type_InterfaceType = Typescript_t.type_InterfaceType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string;
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
  kindName: string;
  types: type_ list
}

and type_NumberLiteralType = Typescript_t.type_NumberLiteralType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string;
  value: float
}

and type_ObjectType = Typescript_t.type_ObjectType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string;
  objectFlags: int
}

and type_StringLiteralType = Typescript_t.type_StringLiteralType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string;
  value: string
}

and type_TupleType = Typescript_t.type_TupleType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string;
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
  kindName: string;
  constraint_: type_ option
}

and type_TypeReference = Typescript_t.type_TypeReference = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string;
  target: type_ option
}

and type_Unidentified = Typescript_t.type_Unidentified = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string;
  nodeKind: string option
}

and type_UnionOrIntersectionType =
  Typescript_t.type_UnionOrIntersectionType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string;
  types: type_ list
}

and type_UnionType = Typescript_t.type_UnionType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string;
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
  aliasTypeArguments: type_ list option;
  kindName: string
}

type readonlyTextRange = Typescript_t.readonlyTextRange = {
  pos: int;
  end_: int
}

type node_TypeElement = Typescript_t.node_TypeElement = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  questionToken: node option
}

type node_SignatureDeclarationBase =
  Typescript_t.node_SignatureDeclarationBase = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  name: node option;
  typeParameters: node list option;
  parameters: node list;
  type_: node option;
  typeArguments: node list option
}

type node_NodeWithTypeArguments = Typescript_t.node_NodeWithTypeArguments = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  typeArguments: node list option
}

type node_NamedDeclaration = Typescript_t.node_NamedDeclaration = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  name: node option
}

type node_FunctionLikeDeclarationBase =
  Typescript_t.node_FunctionLikeDeclarationBase = {
  pos: int;
  end_: int;
  kind: int;
  kindName: string;
  flags: int;
  modifiers: node list option;
  decorators: node list option;
  resolvedSymbol: symbol option;
  resolvedType: type_ option;
  name: node option;
  typeParameters: node list option;
  parameters: node list;
  type_: node option;
  typeArguments: node list option;
  asteriskToken: node option;
  questionToken: node option;
  exclamationToken: node option;
  body: node option
}

let write__11 = (
  Atdgen_codec_runtime.Encode.option_as_constr (
    Atdgen_codec_runtime.Encode.string
  )
)
let read__11 = (
  Atdgen_codec_runtime.Decode.option_as_constr (
    Atdgen_codec_runtime.Decode.string
  )
)
let write__6 = (
  Atdgen_codec_runtime.Encode.option_as_constr (
    Atdgen_codec_runtime.Encode.int
  )
)
let read__6 = (
  Atdgen_codec_runtime.Decode.option_as_constr (
    Atdgen_codec_runtime.Decode.int
  )
)
let write__9 = (
  Atdgen_codec_runtime.Encode.list (
    Atdgen_codec_runtime.Encode.int
  )
)
let read__9 = (
  Atdgen_codec_runtime.Decode.list (
    Atdgen_codec_runtime.Decode.int
  )
)
let rec write__1 js = (
  Atdgen_codec_runtime.Encode.list (
    write_node
  )
) js
and write__2 js = (
  Atdgen_codec_runtime.Encode.option_as_constr (
    write__1
  )
) js
and write__3 js = (
  Atdgen_codec_runtime.Encode.option_as_constr (
    write_symbol
  )
) js
and write__4 js = (
  Atdgen_codec_runtime.Encode.option_as_constr (
    write_type_
  )
) js
and write__5 js = (
  Atdgen_codec_runtime.Encode.option_as_constr (
    write_node
  )
) js
and write__7 js = (
  Atdgen_codec_runtime.Encode.list (
    write_type_
  )
) js
and write__8 js = (
  Atdgen_codec_runtime.Encode.option_as_constr (
    write__7
  )
) js
and write_node js = (
  Atdgen_codec_runtime.Encode.adapter Typescript_json_adapters.KindName.restore (
    Atdgen_codec_runtime.Encode.make (fun (x : _) -> match x with
      | `SourceFile x ->
      Atdgen_codec_runtime.Encode.constr1 "SourceFile" (
        write_node_SourceFile
      ) x
      | `TypeAliasDeclaration x ->
      Atdgen_codec_runtime.Encode.constr1 "TypeAliasDeclaration" (
        write_node_TypeAliasDeclaration
      ) x
      | `TypeParameter x ->
      Atdgen_codec_runtime.Encode.constr1 "TypeParameter" (
        write_node_TypeParameter
      ) x
      | `InterfaceDeclaration x ->
      Atdgen_codec_runtime.Encode.constr1 "InterfaceDeclaration" (
        write_node_InterfaceDeclaration
      ) x
      | `HeritageClause x ->
      Atdgen_codec_runtime.Encode.constr1 "HeritageClause" (
        write_node_HeritageClause
      ) x
      | `Identifier x ->
      Atdgen_codec_runtime.Encode.constr1 "Identifier" (
        write_node_Identifier
      ) x
      | `PrivateIdentifier x ->
      Atdgen_codec_runtime.Encode.constr1 "PrivateIdentifier" (
        write_node_Generic
      ) x
      | `StringLiteral x ->
      Atdgen_codec_runtime.Encode.constr1 "StringLiteral" (
        write_node_Generic
      ) x
      | `NumericLiteral x ->
      Atdgen_codec_runtime.Encode.constr1 "NumericLiteral" (
        write_node_Generic
      ) x
      | `Unknown x ->
      Atdgen_codec_runtime.Encode.constr1 "Unknown" (
        write_node_Generic
      ) x
      | `EndOfFileToken x ->
      Atdgen_codec_runtime.Encode.constr1 "EndOfFileToken" (
        write_node_Generic
      ) x
      | `SingleLineCommentTrivia x ->
      Atdgen_codec_runtime.Encode.constr1 "SingleLineCommentTrivia" (
        write_node_Generic
      ) x
      | `MultiLineCommentTrivia x ->
      Atdgen_codec_runtime.Encode.constr1 "MultiLineCommentTrivia" (
        write_node_Generic
      ) x
      | `NewLineTrivia x ->
      Atdgen_codec_runtime.Encode.constr1 "NewLineTrivia" (
        write_node_Generic
      ) x
      | `WhitespaceTrivia x ->
      Atdgen_codec_runtime.Encode.constr1 "WhitespaceTrivia" (
        write_node_Generic
      ) x
      | `ShebangTrivia x ->
      Atdgen_codec_runtime.Encode.constr1 "ShebangTrivia" (
        write_node_Generic
      ) x
      | `ConflictMarkerTrivia x ->
      Atdgen_codec_runtime.Encode.constr1 "ConflictMarkerTrivia" (
        write_node_Generic
      ) x
      | `BigIntLiteral x ->
      Atdgen_codec_runtime.Encode.constr1 "BigIntLiteral" (
        write_node_Generic
      ) x
      | `JsxText x ->
      Atdgen_codec_runtime.Encode.constr1 "JsxText" (
        write_node_Generic
      ) x
      | `JsxTextAllWhiteSpaces x ->
      Atdgen_codec_runtime.Encode.constr1 "JsxTextAllWhiteSpaces" (
        write_node_Generic
      ) x
      | `RegularExpressionLiteral x ->
      Atdgen_codec_runtime.Encode.constr1 "RegularExpressionLiteral" (
        write_node_Generic
      ) x
      | `NoSubstitutionTemplateLiteral x ->
      Atdgen_codec_runtime.Encode.constr1 "NoSubstitutionTemplateLiteral" (
        write_node_Generic
      ) x
      | `TemplateHead x ->
      Atdgen_codec_runtime.Encode.constr1 "TemplateHead" (
        write_node_Generic
      ) x
      | `TemplateMiddle x ->
      Atdgen_codec_runtime.Encode.constr1 "TemplateMiddle" (
        write_node_Generic
      ) x
      | `TemplateTail x ->
      Atdgen_codec_runtime.Encode.constr1 "TemplateTail" (
        write_node_Generic
      ) x
      | `OpenBraceToken x ->
      Atdgen_codec_runtime.Encode.constr1 "OpenBraceToken" (
        write_node_Generic
      ) x
      | `CloseBraceToken x ->
      Atdgen_codec_runtime.Encode.constr1 "CloseBraceToken" (
        write_node_Generic
      ) x
      | `OpenParenToken x ->
      Atdgen_codec_runtime.Encode.constr1 "OpenParenToken" (
        write_node_Generic
      ) x
      | `CloseParenToken x ->
      Atdgen_codec_runtime.Encode.constr1 "CloseParenToken" (
        write_node_Generic
      ) x
      | `OpenBracketToken x ->
      Atdgen_codec_runtime.Encode.constr1 "OpenBracketToken" (
        write_node_Generic
      ) x
      | `CloseBracketToken x ->
      Atdgen_codec_runtime.Encode.constr1 "CloseBracketToken" (
        write_node_Generic
      ) x
      | `DotToken x ->
      Atdgen_codec_runtime.Encode.constr1 "DotToken" (
        write_node_Generic
      ) x
      | `DotDotDotToken x ->
      Atdgen_codec_runtime.Encode.constr1 "DotDotDotToken" (
        write_node_Generic
      ) x
      | `SemicolonToken x ->
      Atdgen_codec_runtime.Encode.constr1 "SemicolonToken" (
        write_node_Generic
      ) x
      | `CommaToken x ->
      Atdgen_codec_runtime.Encode.constr1 "CommaToken" (
        write_node_Generic
      ) x
      | `QuestionDotToken x ->
      Atdgen_codec_runtime.Encode.constr1 "QuestionDotToken" (
        write_node_Generic
      ) x
      | `LessThanToken x ->
      Atdgen_codec_runtime.Encode.constr1 "LessThanToken" (
        write_node_Generic
      ) x
      | `LessThanSlashToken x ->
      Atdgen_codec_runtime.Encode.constr1 "LessThanSlashToken" (
        write_node_Generic
      ) x
      | `GreaterThanToken x ->
      Atdgen_codec_runtime.Encode.constr1 "GreaterThanToken" (
        write_node_Generic
      ) x
      | `LessThanEqualsToken x ->
      Atdgen_codec_runtime.Encode.constr1 "LessThanEqualsToken" (
        write_node_Generic
      ) x
      | `GreaterThanEqualsToken x ->
      Atdgen_codec_runtime.Encode.constr1 "GreaterThanEqualsToken" (
        write_node_Generic
      ) x
      | `EqualsEqualsToken x ->
      Atdgen_codec_runtime.Encode.constr1 "EqualsEqualsToken" (
        write_node_Generic
      ) x
      | `ExclamationEqualsToken x ->
      Atdgen_codec_runtime.Encode.constr1 "ExclamationEqualsToken" (
        write_node_Generic
      ) x
      | `EqualsEqualsEqualsToken x ->
      Atdgen_codec_runtime.Encode.constr1 "EqualsEqualsEqualsToken" (
        write_node_Generic
      ) x
      | `ExclamationEqualsEqualsToken x ->
      Atdgen_codec_runtime.Encode.constr1 "ExclamationEqualsEqualsToken" (
        write_node_Generic
      ) x
      | `EqualsGreaterThanToken x ->
      Atdgen_codec_runtime.Encode.constr1 "EqualsGreaterThanToken" (
        write_node_Generic
      ) x
      | `PlusToken x ->
      Atdgen_codec_runtime.Encode.constr1 "PlusToken" (
        write_node_Generic
      ) x
      | `MinusToken x ->
      Atdgen_codec_runtime.Encode.constr1 "MinusToken" (
        write_node_Generic
      ) x
      | `AsteriskToken x ->
      Atdgen_codec_runtime.Encode.constr1 "AsteriskToken" (
        write_node_Generic
      ) x
      | `AsteriskAsteriskToken x ->
      Atdgen_codec_runtime.Encode.constr1 "AsteriskAsteriskToken" (
        write_node_Generic
      ) x
      | `SlashToken x ->
      Atdgen_codec_runtime.Encode.constr1 "SlashToken" (
        write_node_Generic
      ) x
      | `PercentToken x ->
      Atdgen_codec_runtime.Encode.constr1 "PercentToken" (
        write_node_Generic
      ) x
      | `PlusPlusToken x ->
      Atdgen_codec_runtime.Encode.constr1 "PlusPlusToken" (
        write_node_Generic
      ) x
      | `MinusMinusToken x ->
      Atdgen_codec_runtime.Encode.constr1 "MinusMinusToken" (
        write_node_Generic
      ) x
      | `LessThanLessThanToken x ->
      Atdgen_codec_runtime.Encode.constr1 "LessThanLessThanToken" (
        write_node_Generic
      ) x
      | `GreaterThanGreaterThanToken x ->
      Atdgen_codec_runtime.Encode.constr1 "GreaterThanGreaterThanToken" (
        write_node_Generic
      ) x
      | `GreaterThanGreaterThanGreaterThanToken x ->
      Atdgen_codec_runtime.Encode.constr1 "GreaterThanGreaterThanGreaterThanToken" (
        write_node_Generic
      ) x
      | `AmpersandToken x ->
      Atdgen_codec_runtime.Encode.constr1 "AmpersandToken" (
        write_node_Generic
      ) x
      | `BarToken x ->
      Atdgen_codec_runtime.Encode.constr1 "BarToken" (
        write_node_Generic
      ) x
      | `CaretToken x ->
      Atdgen_codec_runtime.Encode.constr1 "CaretToken" (
        write_node_Generic
      ) x
      | `ExclamationToken x ->
      Atdgen_codec_runtime.Encode.constr1 "ExclamationToken" (
        write_node_Generic
      ) x
      | `TildeToken x ->
      Atdgen_codec_runtime.Encode.constr1 "TildeToken" (
        write_node_Generic
      ) x
      | `AmpersandAmpersandToken x ->
      Atdgen_codec_runtime.Encode.constr1 "AmpersandAmpersandToken" (
        write_node_Generic
      ) x
      | `BarBarToken x ->
      Atdgen_codec_runtime.Encode.constr1 "BarBarToken" (
        write_node_Generic
      ) x
      | `QuestionToken x ->
      Atdgen_codec_runtime.Encode.constr1 "QuestionToken" (
        write_node_Generic
      ) x
      | `ColonToken x ->
      Atdgen_codec_runtime.Encode.constr1 "ColonToken" (
        write_node_Generic
      ) x
      | `AtToken x ->
      Atdgen_codec_runtime.Encode.constr1 "AtToken" (
        write_node_Generic
      ) x
      | `QuestionQuestionToken x ->
      Atdgen_codec_runtime.Encode.constr1 "QuestionQuestionToken" (
        write_node_Generic
      ) x
      | `BacktickToken x ->
      Atdgen_codec_runtime.Encode.constr1 "BacktickToken" (
        write_node_Generic
      ) x
      | `EqualsToken x ->
      Atdgen_codec_runtime.Encode.constr1 "EqualsToken" (
        write_node_Generic
      ) x
      | `PlusEqualsToken x ->
      Atdgen_codec_runtime.Encode.constr1 "PlusEqualsToken" (
        write_node_Generic
      ) x
      | `MinusEqualsToken x ->
      Atdgen_codec_runtime.Encode.constr1 "MinusEqualsToken" (
        write_node_Generic
      ) x
      | `AsteriskEqualsToken x ->
      Atdgen_codec_runtime.Encode.constr1 "AsteriskEqualsToken" (
        write_node_Generic
      ) x
      | `AsteriskAsteriskEqualsToken x ->
      Atdgen_codec_runtime.Encode.constr1 "AsteriskAsteriskEqualsToken" (
        write_node_Generic
      ) x
      | `SlashEqualsToken x ->
      Atdgen_codec_runtime.Encode.constr1 "SlashEqualsToken" (
        write_node_Generic
      ) x
      | `PercentEqualsToken x ->
      Atdgen_codec_runtime.Encode.constr1 "PercentEqualsToken" (
        write_node_Generic
      ) x
      | `LessThanLessThanEqualsToken x ->
      Atdgen_codec_runtime.Encode.constr1 "LessThanLessThanEqualsToken" (
        write_node_Generic
      ) x
      | `GreaterThanGreaterThanEqualsToken x ->
      Atdgen_codec_runtime.Encode.constr1 "GreaterThanGreaterThanEqualsToken" (
        write_node_Generic
      ) x
      | `GreaterThanGreaterThanGreaterThanEqualsToken x ->
      Atdgen_codec_runtime.Encode.constr1 "GreaterThanGreaterThanGreaterThanEqualsToken" (
        write_node_Generic
      ) x
      | `AmpersandEqualsToken x ->
      Atdgen_codec_runtime.Encode.constr1 "AmpersandEqualsToken" (
        write_node_Generic
      ) x
      | `BarEqualsToken x ->
      Atdgen_codec_runtime.Encode.constr1 "BarEqualsToken" (
        write_node_Generic
      ) x
      | `BarBarEqualsToken x ->
      Atdgen_codec_runtime.Encode.constr1 "BarBarEqualsToken" (
        write_node_Generic
      ) x
      | `AmpersandAmpersandEqualsToken x ->
      Atdgen_codec_runtime.Encode.constr1 "AmpersandAmpersandEqualsToken" (
        write_node_Generic
      ) x
      | `QuestionQuestionEqualsToken x ->
      Atdgen_codec_runtime.Encode.constr1 "QuestionQuestionEqualsToken" (
        write_node_Generic
      ) x
      | `CaretEqualsToken x ->
      Atdgen_codec_runtime.Encode.constr1 "CaretEqualsToken" (
        write_node_Generic
      ) x
      | `BreakKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "BreakKeyword" (
        write_node_Generic
      ) x
      | `CaseKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "CaseKeyword" (
        write_node_Generic
      ) x
      | `CatchKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "CatchKeyword" (
        write_node_Generic
      ) x
      | `ClassKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "ClassKeyword" (
        write_node_Generic
      ) x
      | `ConstKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "ConstKeyword" (
        write_node_Generic
      ) x
      | `ContinueKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "ContinueKeyword" (
        write_node_Generic
      ) x
      | `DebuggerKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "DebuggerKeyword" (
        write_node_Generic
      ) x
      | `DefaultKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "DefaultKeyword" (
        write_node_Generic
      ) x
      | `DeleteKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "DeleteKeyword" (
        write_node_Generic
      ) x
      | `DoKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "DoKeyword" (
        write_node_Generic
      ) x
      | `ElseKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "ElseKeyword" (
        write_node_Generic
      ) x
      | `EnumKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "EnumKeyword" (
        write_node_Generic
      ) x
      | `ExportKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "ExportKeyword" (
        write_node_Generic
      ) x
      | `ExtendsKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "ExtendsKeyword" (
        write_node_Generic
      ) x
      | `FalseKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "FalseKeyword" (
        write_node_Generic
      ) x
      | `FinallyKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "FinallyKeyword" (
        write_node_Generic
      ) x
      | `ForKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "ForKeyword" (
        write_node_Generic
      ) x
      | `FunctionKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "FunctionKeyword" (
        write_node_Generic
      ) x
      | `IfKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "IfKeyword" (
        write_node_Generic
      ) x
      | `ImportKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "ImportKeyword" (
        write_node_Generic
      ) x
      | `InKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "InKeyword" (
        write_node_Generic
      ) x
      | `InstanceOfKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "InstanceOfKeyword" (
        write_node_Generic
      ) x
      | `NewKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "NewKeyword" (
        write_node_Generic
      ) x
      | `NullKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "NullKeyword" (
        write_node_Generic
      ) x
      | `ReturnKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "ReturnKeyword" (
        write_node_Generic
      ) x
      | `SuperKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "SuperKeyword" (
        write_node_Generic
      ) x
      | `SwitchKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "SwitchKeyword" (
        write_node_Generic
      ) x
      | `ThisKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "ThisKeyword" (
        write_node_Generic
      ) x
      | `ThrowKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "ThrowKeyword" (
        write_node_Generic
      ) x
      | `TrueKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "TrueKeyword" (
        write_node_Generic
      ) x
      | `TryKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "TryKeyword" (
        write_node_Generic
      ) x
      | `TypeOfKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "TypeOfKeyword" (
        write_node_Generic
      ) x
      | `VarKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "VarKeyword" (
        write_node_Generic
      ) x
      | `VoidKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "VoidKeyword" (
        write_node_Generic
      ) x
      | `WhileKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "WhileKeyword" (
        write_node_Generic
      ) x
      | `WithKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "WithKeyword" (
        write_node_Generic
      ) x
      | `ImplementsKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "ImplementsKeyword" (
        write_node_Generic
      ) x
      | `InterfaceKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "InterfaceKeyword" (
        write_node_Generic
      ) x
      | `LetKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "LetKeyword" (
        write_node_Generic
      ) x
      | `PackageKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "PackageKeyword" (
        write_node_Generic
      ) x
      | `PrivateKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "PrivateKeyword" (
        write_node_Generic
      ) x
      | `ProtectedKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "ProtectedKeyword" (
        write_node_Generic
      ) x
      | `PublicKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "PublicKeyword" (
        write_node_Generic
      ) x
      | `StaticKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "StaticKeyword" (
        write_node_Generic
      ) x
      | `YieldKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "YieldKeyword" (
        write_node_Generic
      ) x
      | `AbstractKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "AbstractKeyword" (
        write_node_Generic
      ) x
      | `AsKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "AsKeyword" (
        write_node_Generic
      ) x
      | `AssertsKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "AssertsKeyword" (
        write_node_Generic
      ) x
      | `AnyKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "AnyKeyword" (
        write_node_Generic
      ) x
      | `AsyncKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "AsyncKeyword" (
        write_node_Generic
      ) x
      | `AwaitKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "AwaitKeyword" (
        write_node_Generic
      ) x
      | `BooleanKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "BooleanKeyword" (
        write_node_Generic
      ) x
      | `ConstructorKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "ConstructorKeyword" (
        write_node_Generic
      ) x
      | `DeclareKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "DeclareKeyword" (
        write_node_Generic
      ) x
      | `GetKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "GetKeyword" (
        write_node_Generic
      ) x
      | `InferKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "InferKeyword" (
        write_node_Generic
      ) x
      | `IsKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "IsKeyword" (
        write_node_Generic
      ) x
      | `KeyOfKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "KeyOfKeyword" (
        write_node_Generic
      ) x
      | `ModuleKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "ModuleKeyword" (
        write_node_Generic
      ) x
      | `NamespaceKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "NamespaceKeyword" (
        write_node_Generic
      ) x
      | `NeverKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "NeverKeyword" (
        write_node_Generic
      ) x
      | `ReadonlyKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "ReadonlyKeyword" (
        write_node_Generic
      ) x
      | `RequireKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "RequireKeyword" (
        write_node_Generic
      ) x
      | `NumberKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "NumberKeyword" (
        write_node_Generic
      ) x
      | `ObjectKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "ObjectKeyword" (
        write_node_Generic
      ) x
      | `SetKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "SetKeyword" (
        write_node_Generic
      ) x
      | `StringKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "StringKeyword" (
        write_node_Generic
      ) x
      | `SymbolKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "SymbolKeyword" (
        write_node_Generic
      ) x
      | `TypeKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "TypeKeyword" (
        write_node_Generic
      ) x
      | `UndefinedKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "UndefinedKeyword" (
        write_node_Generic
      ) x
      | `UniqueKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "UniqueKeyword" (
        write_node_Generic
      ) x
      | `UnknownKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "UnknownKeyword" (
        write_node_Generic
      ) x
      | `FromKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "FromKeyword" (
        write_node_Generic
      ) x
      | `GlobalKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "GlobalKeyword" (
        write_node_Generic
      ) x
      | `BigIntKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "BigIntKeyword" (
        write_node_Generic
      ) x
      | `OfKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "OfKeyword" (
        write_node_Generic
      ) x
      | `QualifiedName x ->
      Atdgen_codec_runtime.Encode.constr1 "QualifiedName" (
        write_node_QualifiedName
      ) x
      | `ComputedPropertyName x ->
      Atdgen_codec_runtime.Encode.constr1 "ComputedPropertyName" (
        write_node_Generic
      ) x
      | `Parameter x ->
      Atdgen_codec_runtime.Encode.constr1 "Parameter" (
        write_node_Parameter
      ) x
      | `Decorator x ->
      Atdgen_codec_runtime.Encode.constr1 "Decorator" (
        write_node_Generic
      ) x
      | `PropertySignature x ->
      Atdgen_codec_runtime.Encode.constr1 "PropertySignature" (
        write_node_PropertySignature
      ) x
      | `PropertyDeclaration x ->
      Atdgen_codec_runtime.Encode.constr1 "PropertyDeclaration" (
        write_node_PropertyDeclaration
      ) x
      | `MethodSignature x ->
      Atdgen_codec_runtime.Encode.constr1 "MethodSignature" (
        write_node_MethodSignature
      ) x
      | `MethodDeclaration x ->
      Atdgen_codec_runtime.Encode.constr1 "MethodDeclaration" (
        write_node_Generic
      ) x
      | `Constructor x ->
      Atdgen_codec_runtime.Encode.constr1 "Constructor" (
        write_node_Generic
      ) x
      | `GetAccessor x ->
      Atdgen_codec_runtime.Encode.constr1 "GetAccessor" (
        write_node_Generic
      ) x
      | `SetAccessor x ->
      Atdgen_codec_runtime.Encode.constr1 "SetAccessor" (
        write_node_Generic
      ) x
      | `CallSignature x ->
      Atdgen_codec_runtime.Encode.constr1 "CallSignature" (
        write_node_CallSignature
      ) x
      | `ConstructSignature x ->
      Atdgen_codec_runtime.Encode.constr1 "ConstructSignature" (
        write_node_Generic
      ) x
      | `IndexSignature x ->
      Atdgen_codec_runtime.Encode.constr1 "IndexSignature" (
        write_node_IndexSignature
      ) x
      | `TypePredicate x ->
      Atdgen_codec_runtime.Encode.constr1 "TypePredicate" (
        write_node_Generic
      ) x
      | `TypeReference x ->
      Atdgen_codec_runtime.Encode.constr1 "TypeReference" (
        write_node_TypeReference
      ) x
      | `FunctionType x ->
      Atdgen_codec_runtime.Encode.constr1 "FunctionType" (
        write_node_FunctionType
      ) x
      | `ConstructorType x ->
      Atdgen_codec_runtime.Encode.constr1 "ConstructorType" (
        write_node_Generic
      ) x
      | `TypeQuery x ->
      Atdgen_codec_runtime.Encode.constr1 "TypeQuery" (
        write_node_Generic
      ) x
      | `TypeLiteral x ->
      Atdgen_codec_runtime.Encode.constr1 "TypeLiteral" (
        write_node_TypeLiteral
      ) x
      | `ArrayType x ->
      Atdgen_codec_runtime.Encode.constr1 "ArrayType" (
        write_node_ArrayType
      ) x
      | `TupleType x ->
      Atdgen_codec_runtime.Encode.constr1 "TupleType" (
        write_node_Generic
      ) x
      | `OptionalType x ->
      Atdgen_codec_runtime.Encode.constr1 "OptionalType" (
        write_node_Generic
      ) x
      | `RestType x ->
      Atdgen_codec_runtime.Encode.constr1 "RestType" (
        write_node_Generic
      ) x
      | `UnionType x ->
      Atdgen_codec_runtime.Encode.constr1 "UnionType" (
        write_node_UnionType
      ) x
      | `IntersectionType x ->
      Atdgen_codec_runtime.Encode.constr1 "IntersectionType" (
        write_node_Generic
      ) x
      | `ConditionalType x ->
      Atdgen_codec_runtime.Encode.constr1 "ConditionalType" (
        write_node_Generic
      ) x
      | `InferType x ->
      Atdgen_codec_runtime.Encode.constr1 "InferType" (
        write_node_Generic
      ) x
      | `ParenthesizedType x ->
      Atdgen_codec_runtime.Encode.constr1 "ParenthesizedType" (
        write_node_Generic
      ) x
      | `ThisType x ->
      Atdgen_codec_runtime.Encode.constr1 "ThisType" (
        write_node_Generic
      ) x
      | `TypeOperator x ->
      Atdgen_codec_runtime.Encode.constr1 "TypeOperator" (
        write_node_TypeOperator
      ) x
      | `IndexedAccessType x ->
      Atdgen_codec_runtime.Encode.constr1 "IndexedAccessType" (
        write_node_Generic
      ) x
      | `MappedType x ->
      Atdgen_codec_runtime.Encode.constr1 "MappedType" (
        write_node_Generic
      ) x
      | `LiteralType x ->
      Atdgen_codec_runtime.Encode.constr1 "LiteralType" (
        write_node_Generic
      ) x
      | `NamedTupleMember x ->
      Atdgen_codec_runtime.Encode.constr1 "NamedTupleMember" (
        write_node_Generic
      ) x
      | `ImportType x ->
      Atdgen_codec_runtime.Encode.constr1 "ImportType" (
        write_node_Generic
      ) x
      | `ObjectBindingPattern x ->
      Atdgen_codec_runtime.Encode.constr1 "ObjectBindingPattern" (
        write_node_Generic
      ) x
      | `ArrayBindingPattern x ->
      Atdgen_codec_runtime.Encode.constr1 "ArrayBindingPattern" (
        write_node_Generic
      ) x
      | `BindingElement x ->
      Atdgen_codec_runtime.Encode.constr1 "BindingElement" (
        write_node_BindingElement
      ) x
      | `ArrayLiteralExpression x ->
      Atdgen_codec_runtime.Encode.constr1 "ArrayLiteralExpression" (
        write_node_Generic
      ) x
      | `ObjectLiteralExpression x ->
      Atdgen_codec_runtime.Encode.constr1 "ObjectLiteralExpression" (
        write_node_Generic
      ) x
      | `PropertyAccessExpression x ->
      Atdgen_codec_runtime.Encode.constr1 "PropertyAccessExpression" (
        write_node_Generic
      ) x
      | `ElementAccessExpression x ->
      Atdgen_codec_runtime.Encode.constr1 "ElementAccessExpression" (
        write_node_Generic
      ) x
      | `CallExpression x ->
      Atdgen_codec_runtime.Encode.constr1 "CallExpression" (
        write_node_Generic
      ) x
      | `NewExpression x ->
      Atdgen_codec_runtime.Encode.constr1 "NewExpression" (
        write_node_Generic
      ) x
      | `TaggedTemplateExpression x ->
      Atdgen_codec_runtime.Encode.constr1 "TaggedTemplateExpression" (
        write_node_Generic
      ) x
      | `TypeAssertionExpression x ->
      Atdgen_codec_runtime.Encode.constr1 "TypeAssertionExpression" (
        write_node_Generic
      ) x
      | `ParenthesizedExpression x ->
      Atdgen_codec_runtime.Encode.constr1 "ParenthesizedExpression" (
        write_node_Generic
      ) x
      | `FunctionExpression x ->
      Atdgen_codec_runtime.Encode.constr1 "FunctionExpression" (
        write_node_Generic
      ) x
      | `ArrowFunction x ->
      Atdgen_codec_runtime.Encode.constr1 "ArrowFunction" (
        write_node_Generic
      ) x
      | `DeleteExpression x ->
      Atdgen_codec_runtime.Encode.constr1 "DeleteExpression" (
        write_node_Generic
      ) x
      | `TypeOfExpression x ->
      Atdgen_codec_runtime.Encode.constr1 "TypeOfExpression" (
        write_node_Generic
      ) x
      | `VoidExpression x ->
      Atdgen_codec_runtime.Encode.constr1 "VoidExpression" (
        write_node_Generic
      ) x
      | `AwaitExpression x ->
      Atdgen_codec_runtime.Encode.constr1 "AwaitExpression" (
        write_node_Generic
      ) x
      | `PrefixUnaryExpression x ->
      Atdgen_codec_runtime.Encode.constr1 "PrefixUnaryExpression" (
        write_node_Generic
      ) x
      | `PostfixUnaryExpression x ->
      Atdgen_codec_runtime.Encode.constr1 "PostfixUnaryExpression" (
        write_node_Generic
      ) x
      | `BinaryExpression x ->
      Atdgen_codec_runtime.Encode.constr1 "BinaryExpression" (
        write_node_Generic
      ) x
      | `ConditionalExpression x ->
      Atdgen_codec_runtime.Encode.constr1 "ConditionalExpression" (
        write_node_Generic
      ) x
      | `TemplateExpression x ->
      Atdgen_codec_runtime.Encode.constr1 "TemplateExpression" (
        write_node_Generic
      ) x
      | `YieldExpression x ->
      Atdgen_codec_runtime.Encode.constr1 "YieldExpression" (
        write_node_Generic
      ) x
      | `SpreadElement x ->
      Atdgen_codec_runtime.Encode.constr1 "SpreadElement" (
        write_node_Generic
      ) x
      | `ClassExpression x ->
      Atdgen_codec_runtime.Encode.constr1 "ClassExpression" (
        write_node_Generic
      ) x
      | `OmittedExpression x ->
      Atdgen_codec_runtime.Encode.constr1 "OmittedExpression" (
        write_node_Generic
      ) x
      | `ExpressionWithTypeArguments x ->
      Atdgen_codec_runtime.Encode.constr1 "ExpressionWithTypeArguments" (
        write_node_Generic
      ) x
      | `AsExpression x ->
      Atdgen_codec_runtime.Encode.constr1 "AsExpression" (
        write_node_Generic
      ) x
      | `NonNullExpression x ->
      Atdgen_codec_runtime.Encode.constr1 "NonNullExpression" (
        write_node_Generic
      ) x
      | `MetaProperty x ->
      Atdgen_codec_runtime.Encode.constr1 "MetaProperty" (
        write_node_Generic
      ) x
      | `SyntheticExpression x ->
      Atdgen_codec_runtime.Encode.constr1 "SyntheticExpression" (
        write_node_Generic
      ) x
      | `TemplateSpan x ->
      Atdgen_codec_runtime.Encode.constr1 "TemplateSpan" (
        write_node_Generic
      ) x
      | `SemicolonClassElement x ->
      Atdgen_codec_runtime.Encode.constr1 "SemicolonClassElement" (
        write_node_Generic
      ) x
      | `Block x ->
      Atdgen_codec_runtime.Encode.constr1 "Block" (
        write_node_Generic
      ) x
      | `EmptyStatement x ->
      Atdgen_codec_runtime.Encode.constr1 "EmptyStatement" (
        write_node_Generic
      ) x
      | `VariableStatement x ->
      Atdgen_codec_runtime.Encode.constr1 "VariableStatement" (
        write_node_Generic
      ) x
      | `ExpressionStatement x ->
      Atdgen_codec_runtime.Encode.constr1 "ExpressionStatement" (
        write_node_Generic
      ) x
      | `IfStatement x ->
      Atdgen_codec_runtime.Encode.constr1 "IfStatement" (
        write_node_Generic
      ) x
      | `DoStatement x ->
      Atdgen_codec_runtime.Encode.constr1 "DoStatement" (
        write_node_Generic
      ) x
      | `WhileStatement x ->
      Atdgen_codec_runtime.Encode.constr1 "WhileStatement" (
        write_node_Generic
      ) x
      | `ForStatement x ->
      Atdgen_codec_runtime.Encode.constr1 "ForStatement" (
        write_node_Generic
      ) x
      | `ForInStatement x ->
      Atdgen_codec_runtime.Encode.constr1 "ForInStatement" (
        write_node_Generic
      ) x
      | `ForOfStatement x ->
      Atdgen_codec_runtime.Encode.constr1 "ForOfStatement" (
        write_node_Generic
      ) x
      | `ContinueStatement x ->
      Atdgen_codec_runtime.Encode.constr1 "ContinueStatement" (
        write_node_Generic
      ) x
      | `BreakStatement x ->
      Atdgen_codec_runtime.Encode.constr1 "BreakStatement" (
        write_node_Generic
      ) x
      | `ReturnStatement x ->
      Atdgen_codec_runtime.Encode.constr1 "ReturnStatement" (
        write_node_Generic
      ) x
      | `WithStatement x ->
      Atdgen_codec_runtime.Encode.constr1 "WithStatement" (
        write_node_Generic
      ) x
      | `SwitchStatement x ->
      Atdgen_codec_runtime.Encode.constr1 "SwitchStatement" (
        write_node_Generic
      ) x
      | `LabeledStatement x ->
      Atdgen_codec_runtime.Encode.constr1 "LabeledStatement" (
        write_node_Generic
      ) x
      | `ThrowStatement x ->
      Atdgen_codec_runtime.Encode.constr1 "ThrowStatement" (
        write_node_Generic
      ) x
      | `TryStatement x ->
      Atdgen_codec_runtime.Encode.constr1 "TryStatement" (
        write_node_Generic
      ) x
      | `DebuggerStatement x ->
      Atdgen_codec_runtime.Encode.constr1 "DebuggerStatement" (
        write_node_Generic
      ) x
      | `VariableDeclaration x ->
      Atdgen_codec_runtime.Encode.constr1 "VariableDeclaration" (
        write_node_VariableDeclaration
      ) x
      | `VariableDeclarationList x ->
      Atdgen_codec_runtime.Encode.constr1 "VariableDeclarationList" (
        write_node_VariableDeclarationList
      ) x
      | `FunctionDeclaration x ->
      Atdgen_codec_runtime.Encode.constr1 "FunctionDeclaration" (
        write_node_FunctionDeclaration
      ) x
      | `ClassDeclaration x ->
      Atdgen_codec_runtime.Encode.constr1 "ClassDeclaration" (
        write_node_Generic
      ) x
      | `EnumDeclaration x ->
      Atdgen_codec_runtime.Encode.constr1 "EnumDeclaration" (
        write_node_EnumDeclaration
      ) x
      | `ModuleDeclaration x ->
      Atdgen_codec_runtime.Encode.constr1 "ModuleDeclaration" (
        write_node_Generic
      ) x
      | `ModuleBlock x ->
      Atdgen_codec_runtime.Encode.constr1 "ModuleBlock" (
        write_node_Generic
      ) x
      | `CaseBlock x ->
      Atdgen_codec_runtime.Encode.constr1 "CaseBlock" (
        write_node_Generic
      ) x
      | `NamespaceExportDeclaration x ->
      Atdgen_codec_runtime.Encode.constr1 "NamespaceExportDeclaration" (
        write_node_Generic
      ) x
      | `ImportEqualsDeclaration x ->
      Atdgen_codec_runtime.Encode.constr1 "ImportEqualsDeclaration" (
        write_node_Generic
      ) x
      | `ImportDeclaration x ->
      Atdgen_codec_runtime.Encode.constr1 "ImportDeclaration" (
        write_node_Generic
      ) x
      | `ImportClause x ->
      Atdgen_codec_runtime.Encode.constr1 "ImportClause" (
        write_node_Generic
      ) x
      | `NamespaceImport x ->
      Atdgen_codec_runtime.Encode.constr1 "NamespaceImport" (
        write_node_Generic
      ) x
      | `NamedImports x ->
      Atdgen_codec_runtime.Encode.constr1 "NamedImports" (
        write_node_Generic
      ) x
      | `ImportSpecifier x ->
      Atdgen_codec_runtime.Encode.constr1 "ImportSpecifier" (
        write_node_Generic
      ) x
      | `ExportAssignment x ->
      Atdgen_codec_runtime.Encode.constr1 "ExportAssignment" (
        write_node_Generic
      ) x
      | `ExportDeclaration x ->
      Atdgen_codec_runtime.Encode.constr1 "ExportDeclaration" (
        write_node_Generic
      ) x
      | `NamedExports x ->
      Atdgen_codec_runtime.Encode.constr1 "NamedExports" (
        write_node_Generic
      ) x
      | `NamespaceExport x ->
      Atdgen_codec_runtime.Encode.constr1 "NamespaceExport" (
        write_node_Generic
      ) x
      | `ExportSpecifier x ->
      Atdgen_codec_runtime.Encode.constr1 "ExportSpecifier" (
        write_node_Generic
      ) x
      | `MissingDeclaration x ->
      Atdgen_codec_runtime.Encode.constr1 "MissingDeclaration" (
        write_node_Generic
      ) x
      | `ExternalModuleReference x ->
      Atdgen_codec_runtime.Encode.constr1 "ExternalModuleReference" (
        write_node_Generic
      ) x
      | `JsxElement x ->
      Atdgen_codec_runtime.Encode.constr1 "JsxElement" (
        write_node_Generic
      ) x
      | `JsxSelfClosingElement x ->
      Atdgen_codec_runtime.Encode.constr1 "JsxSelfClosingElement" (
        write_node_Generic
      ) x
      | `JsxOpeningElement x ->
      Atdgen_codec_runtime.Encode.constr1 "JsxOpeningElement" (
        write_node_Generic
      ) x
      | `JsxClosingElement x ->
      Atdgen_codec_runtime.Encode.constr1 "JsxClosingElement" (
        write_node_Generic
      ) x
      | `JsxFragment x ->
      Atdgen_codec_runtime.Encode.constr1 "JsxFragment" (
        write_node_Generic
      ) x
      | `JsxOpeningFragment x ->
      Atdgen_codec_runtime.Encode.constr1 "JsxOpeningFragment" (
        write_node_Generic
      ) x
      | `JsxClosingFragment x ->
      Atdgen_codec_runtime.Encode.constr1 "JsxClosingFragment" (
        write_node_Generic
      ) x
      | `JsxAttribute x ->
      Atdgen_codec_runtime.Encode.constr1 "JsxAttribute" (
        write_node_Generic
      ) x
      | `JsxAttributes x ->
      Atdgen_codec_runtime.Encode.constr1 "JsxAttributes" (
        write_node_Generic
      ) x
      | `JsxSpreadAttribute x ->
      Atdgen_codec_runtime.Encode.constr1 "JsxSpreadAttribute" (
        write_node_Generic
      ) x
      | `JsxExpression x ->
      Atdgen_codec_runtime.Encode.constr1 "JsxExpression" (
        write_node_Generic
      ) x
      | `CaseClause x ->
      Atdgen_codec_runtime.Encode.constr1 "CaseClause" (
        write_node_Generic
      ) x
      | `DefaultClause x ->
      Atdgen_codec_runtime.Encode.constr1 "DefaultClause" (
        write_node_Generic
      ) x
      | `CatchClause x ->
      Atdgen_codec_runtime.Encode.constr1 "CatchClause" (
        write_node_Generic
      ) x
      | `PropertyAssignment x ->
      Atdgen_codec_runtime.Encode.constr1 "PropertyAssignment" (
        write_node_Generic
      ) x
      | `ShorthandPropertyAssignment x ->
      Atdgen_codec_runtime.Encode.constr1 "ShorthandPropertyAssignment" (
        write_node_Generic
      ) x
      | `SpreadAssignment x ->
      Atdgen_codec_runtime.Encode.constr1 "SpreadAssignment" (
        write_node_Generic
      ) x
      | `EnumMember x ->
      Atdgen_codec_runtime.Encode.constr1 "EnumMember" (
        write_node_EnumMember
      ) x
      | `UnparsedPrologue x ->
      Atdgen_codec_runtime.Encode.constr1 "UnparsedPrologue" (
        write_node_Generic
      ) x
      | `UnparsedPrepend x ->
      Atdgen_codec_runtime.Encode.constr1 "UnparsedPrepend" (
        write_node_Generic
      ) x
      | `UnparsedText x ->
      Atdgen_codec_runtime.Encode.constr1 "UnparsedText" (
        write_node_Generic
      ) x
      | `UnparsedInternalText x ->
      Atdgen_codec_runtime.Encode.constr1 "UnparsedInternalText" (
        write_node_Generic
      ) x
      | `UnparsedSyntheticReference x ->
      Atdgen_codec_runtime.Encode.constr1 "UnparsedSyntheticReference" (
        write_node_Generic
      ) x
      | `Bundle x ->
      Atdgen_codec_runtime.Encode.constr1 "Bundle" (
        write_node_Generic
      ) x
      | `UnparsedSource x ->
      Atdgen_codec_runtime.Encode.constr1 "UnparsedSource" (
        write_node_Generic
      ) x
      | `InputFiles x ->
      Atdgen_codec_runtime.Encode.constr1 "InputFiles" (
        write_node_Generic
      ) x
      | `JSDocTypeExpression x ->
      Atdgen_codec_runtime.Encode.constr1 "JSDocTypeExpression" (
        write_node_Generic
      ) x
      | `JSDocAllType x ->
      Atdgen_codec_runtime.Encode.constr1 "JSDocAllType" (
        write_node_Generic
      ) x
      | `JSDocUnknownType x ->
      Atdgen_codec_runtime.Encode.constr1 "JSDocUnknownType" (
        write_node_Generic
      ) x
      | `JSDocNullableType x ->
      Atdgen_codec_runtime.Encode.constr1 "JSDocNullableType" (
        write_node_Generic
      ) x
      | `JSDocNonNullableType x ->
      Atdgen_codec_runtime.Encode.constr1 "JSDocNonNullableType" (
        write_node_Generic
      ) x
      | `JSDocOptionalType x ->
      Atdgen_codec_runtime.Encode.constr1 "JSDocOptionalType" (
        write_node_Generic
      ) x
      | `JSDocFunctionType x ->
      Atdgen_codec_runtime.Encode.constr1 "JSDocFunctionType" (
        write_node_Generic
      ) x
      | `JSDocVariadicType x ->
      Atdgen_codec_runtime.Encode.constr1 "JSDocVariadicType" (
        write_node_Generic
      ) x
      | `JSDocNamepathType x ->
      Atdgen_codec_runtime.Encode.constr1 "JSDocNamepathType" (
        write_node_Generic
      ) x
      | `JSDocComment x ->
      Atdgen_codec_runtime.Encode.constr1 "JSDocComment" (
        write_node_Generic
      ) x
      | `JSDocTypeLiteral x ->
      Atdgen_codec_runtime.Encode.constr1 "JSDocTypeLiteral" (
        write_node_Generic
      ) x
      | `JSDocSignature x ->
      Atdgen_codec_runtime.Encode.constr1 "JSDocSignature" (
        write_node_Generic
      ) x
      | `JSDocTag x ->
      Atdgen_codec_runtime.Encode.constr1 "JSDocTag" (
        write_node_Generic
      ) x
      | `JSDocAugmentsTag x ->
      Atdgen_codec_runtime.Encode.constr1 "JSDocAugmentsTag" (
        write_node_Generic
      ) x
      | `JSDocImplementsTag x ->
      Atdgen_codec_runtime.Encode.constr1 "JSDocImplementsTag" (
        write_node_Generic
      ) x
      | `JSDocAuthorTag x ->
      Atdgen_codec_runtime.Encode.constr1 "JSDocAuthorTag" (
        write_node_Generic
      ) x
      | `JSDocDeprecatedTag x ->
      Atdgen_codec_runtime.Encode.constr1 "JSDocDeprecatedTag" (
        write_node_Generic
      ) x
      | `JSDocClassTag x ->
      Atdgen_codec_runtime.Encode.constr1 "JSDocClassTag" (
        write_node_Generic
      ) x
      | `JSDocPublicTag x ->
      Atdgen_codec_runtime.Encode.constr1 "JSDocPublicTag" (
        write_node_Generic
      ) x
      | `JSDocPrivateTag x ->
      Atdgen_codec_runtime.Encode.constr1 "JSDocPrivateTag" (
        write_node_Generic
      ) x
      | `JSDocProtectedTag x ->
      Atdgen_codec_runtime.Encode.constr1 "JSDocProtectedTag" (
        write_node_Generic
      ) x
      | `JSDocReadonlyTag x ->
      Atdgen_codec_runtime.Encode.constr1 "JSDocReadonlyTag" (
        write_node_Generic
      ) x
      | `JSDocCallbackTag x ->
      Atdgen_codec_runtime.Encode.constr1 "JSDocCallbackTag" (
        write_node_Generic
      ) x
      | `JSDocEnumTag x ->
      Atdgen_codec_runtime.Encode.constr1 "JSDocEnumTag" (
        write_node_Generic
      ) x
      | `JSDocParameterTag x ->
      Atdgen_codec_runtime.Encode.constr1 "JSDocParameterTag" (
        write_node_Generic
      ) x
      | `JSDocReturnTag x ->
      Atdgen_codec_runtime.Encode.constr1 "JSDocReturnTag" (
        write_node_Generic
      ) x
      | `JSDocThisTag x ->
      Atdgen_codec_runtime.Encode.constr1 "JSDocThisTag" (
        write_node_Generic
      ) x
      | `JSDocTypeTag x ->
      Atdgen_codec_runtime.Encode.constr1 "JSDocTypeTag" (
        write_node_Generic
      ) x
      | `JSDocTemplateTag x ->
      Atdgen_codec_runtime.Encode.constr1 "JSDocTemplateTag" (
        write_node_Generic
      ) x
      | `JSDocTypedefTag x ->
      Atdgen_codec_runtime.Encode.constr1 "JSDocTypedefTag" (
        write_node_Generic
      ) x
      | `JSDocPropertyTag x ->
      Atdgen_codec_runtime.Encode.constr1 "JSDocPropertyTag" (
        write_node_Generic
      ) x
      | `SyntaxList x ->
      Atdgen_codec_runtime.Encode.constr1 "SyntaxList" (
        write_node_Generic
      ) x
      | `NotEmittedStatement x ->
      Atdgen_codec_runtime.Encode.constr1 "NotEmittedStatement" (
        write_node_Generic
      ) x
      | `PartiallyEmittedExpression x ->
      Atdgen_codec_runtime.Encode.constr1 "PartiallyEmittedExpression" (
        write_node_Generic
      ) x
      | `CommaListExpression x ->
      Atdgen_codec_runtime.Encode.constr1 "CommaListExpression" (
        write_node_Generic
      ) x
      | `MergeDeclarationMarker x ->
      Atdgen_codec_runtime.Encode.constr1 "MergeDeclarationMarker" (
        write_node_Generic
      ) x
      | `EndOfDeclarationMarker x ->
      Atdgen_codec_runtime.Encode.constr1 "EndOfDeclarationMarker" (
        write_node_Generic
      ) x
      | `SyntheticReferenceExpression x ->
      Atdgen_codec_runtime.Encode.constr1 "SyntheticReferenceExpression" (
        write_node_Generic
      ) x
      | `Count x ->
      Atdgen_codec_runtime.Encode.constr1 "Count" (
        write_node_Generic
      ) x
    )
  )
) js
and write_node_ArrayType js = (
  Atdgen_codec_runtime.Encode.make (fun (t : node_ArrayType) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"pos"
          t.pos
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"end"
          t.end_
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"kind"
          t.kind
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"modifiers"
          t.modifiers
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"decorators"
          t.decorators
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_symbol
            )
          ~name:"resolvedSymbol"
          t.resolvedSymbol
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"resolvedType"
          t.resolvedType
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write_node
            )
          ~name:"elementType"
          t.elementType
      ]
    )
  )
) js
and write_node_BindingElement js = (
  Atdgen_codec_runtime.Encode.make (fun (t : node_BindingElement) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"pos"
          t.pos
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"end"
          t.end_
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"kind"
          t.kind
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"modifiers"
          t.modifiers
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"decorators"
          t.decorators
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_symbol
            )
          ~name:"resolvedSymbol"
          t.resolvedSymbol
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"resolvedType"
          t.resolvedType
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"propertyName"
          t.propertyName
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"dotDotDotToken"
          t.dotDotDotToken
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write_node
            )
          ~name:"name"
          t.name
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"initializer"
          t.initializer_
      ]
    )
  )
) js
and write_node_CallSignature js = (
  Atdgen_codec_runtime.Encode.make (fun (t : node_CallSignature) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"pos"
          t.pos
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"end"
          t.end_
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"kind"
          t.kind
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"modifiers"
          t.modifiers
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"decorators"
          t.decorators
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_symbol
            )
          ~name:"resolvedSymbol"
          t.resolvedSymbol
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"resolvedType"
          t.resolvedType
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"name"
          t.name
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"typeParameters"
          t.typeParameters
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write__1
            )
          ~name:"parameters"
          t.parameters
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"type"
          t.type_
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"typeArguments"
          t.typeArguments
      ]
    )
  )
) js
and write_node_EnumDeclaration js = (
  Atdgen_codec_runtime.Encode.make (fun (t : node_EnumDeclaration) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"pos"
          t.pos
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"end"
          t.end_
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"kind"
          t.kind
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"modifiers"
          t.modifiers
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"decorators"
          t.decorators
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_symbol
            )
          ~name:"resolvedSymbol"
          t.resolvedSymbol
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"resolvedType"
          t.resolvedType
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write_node
            )
          ~name:"name"
          t.name
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write__1
            )
          ~name:"members"
          t.members
      ]
    )
  )
) js
and write_node_EnumMember js = (
  Atdgen_codec_runtime.Encode.make (fun (t : node_EnumMember) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"pos"
          t.pos
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"end"
          t.end_
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"kind"
          t.kind
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"modifiers"
          t.modifiers
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"decorators"
          t.decorators
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_symbol
            )
          ~name:"resolvedSymbol"
          t.resolvedSymbol
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"resolvedType"
          t.resolvedType
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write_node
            )
          ~name:"name"
          t.name
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"initializer"
          t.initializer_
      ]
    )
  )
) js
and write_node_FunctionDeclaration js = (
  Atdgen_codec_runtime.Encode.make (fun (t : node_FunctionDeclaration) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"pos"
          t.pos
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"end"
          t.end_
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"kind"
          t.kind
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"modifiers"
          t.modifiers
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"decorators"
          t.decorators
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_symbol
            )
          ~name:"resolvedSymbol"
          t.resolvedSymbol
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"resolvedType"
          t.resolvedType
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"typeParameters"
          t.typeParameters
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write__1
            )
          ~name:"parameters"
          t.parameters
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"type"
          t.type_
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"typeArguments"
          t.typeArguments
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"asteriskToken"
          t.asteriskToken
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"questionToken"
          t.questionToken
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"exclamationToken"
          t.exclamationToken
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"name"
          t.name
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"body"
          t.body
      ]
    )
  )
) js
and write_node_FunctionType js = (
  Atdgen_codec_runtime.Encode.make (fun (t : node_FunctionType) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"pos"
          t.pos
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"end"
          t.end_
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"kind"
          t.kind
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"modifiers"
          t.modifiers
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"decorators"
          t.decorators
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_symbol
            )
          ~name:"resolvedSymbol"
          t.resolvedSymbol
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"resolvedType"
          t.resolvedType
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"name"
          t.name
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"typeParameters"
          t.typeParameters
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write__1
            )
          ~name:"parameters"
          t.parameters
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"type"
          t.type_
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"typeArguments"
          t.typeArguments
      ]
    )
  )
) js
and write_node_Generic js = (
  Atdgen_codec_runtime.Encode.make (fun (t : node_Generic) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"pos"
          t.pos
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"end"
          t.end_
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"kind"
          t.kind
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"modifiers"
          t.modifiers
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"decorators"
          t.decorators
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_symbol
            )
          ~name:"resolvedSymbol"
          t.resolvedSymbol
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"resolvedType"
          t.resolvedType
      ]
    )
  )
) js
and write_node_HeritageClause js = (
  Atdgen_codec_runtime.Encode.make (fun (t : node_HeritageClause) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"pos"
          t.pos
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"end"
          t.end_
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"kind"
          t.kind
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"modifiers"
          t.modifiers
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"decorators"
          t.decorators
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_symbol
            )
          ~name:"resolvedSymbol"
          t.resolvedSymbol
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"resolvedType"
          t.resolvedType
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"token"
          t.token
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write__2
            )
          ~name:"types"
          t.types
      ]
    )
  )
) js
and write_node_Identifier js = (
  Atdgen_codec_runtime.Encode.make (fun (t : node_Identifier) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"pos"
          t.pos
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"end"
          t.end_
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"kind"
          t.kind
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"modifiers"
          t.modifiers
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"decorators"
          t.decorators
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_symbol
            )
          ~name:"resolvedSymbol"
          t.resolvedSymbol
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"resolvedType"
          t.resolvedType
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"escapedText"
          t.escapedText
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"originalKeywordKind"
          t.originalKeywordKind
      ]
    )
  )
) js
and write_node_IndexSignature js = (
  Atdgen_codec_runtime.Encode.make (fun (t : node_IndexSignature) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"pos"
          t.pos
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"end"
          t.end_
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"kind"
          t.kind
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"modifiers"
          t.modifiers
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"decorators"
          t.decorators
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_symbol
            )
          ~name:"resolvedSymbol"
          t.resolvedSymbol
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"resolvedType"
          t.resolvedType
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"typeParameters"
          t.typeParameters
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write__1
            )
          ~name:"parameters"
          t.parameters
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"type"
          t.type_
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"typeArguments"
          t.typeArguments
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"name"
          t.name
      ]
    )
  )
) js
and write_node_InterfaceDeclaration js = (
  Atdgen_codec_runtime.Encode.make (fun (t : node_InterfaceDeclaration) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"pos"
          t.pos
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"end"
          t.end_
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"kind"
          t.kind
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"modifiers"
          t.modifiers
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"decorators"
          t.decorators
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_symbol
            )
          ~name:"resolvedSymbol"
          t.resolvedSymbol
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"resolvedType"
          t.resolvedType
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write_node
            )
          ~name:"name"
          t.name
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"typeParameters"
          t.typeParameters
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"heritageClauses"
          t.heritageClauses
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write__1
            )
          ~name:"members"
          t.members
      ]
    )
  )
) js
and write_node_MethodSignature js = (
  Atdgen_codec_runtime.Encode.make (fun (t : node_MethodSignature) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"pos"
          t.pos
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"end"
          t.end_
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"kind"
          t.kind
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"modifiers"
          t.modifiers
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"decorators"
          t.decorators
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_symbol
            )
          ~name:"resolvedSymbol"
          t.resolvedSymbol
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"resolvedType"
          t.resolvedType
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"typeParameters"
          t.typeParameters
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write__1
            )
          ~name:"parameters"
          t.parameters
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"type"
          t.type_
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"typeArguments"
          t.typeArguments
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"name"
          t.name
      ]
    )
  )
) js
and write_node_Parameter js = (
  Atdgen_codec_runtime.Encode.make (fun (t : node_Parameter) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"pos"
          t.pos
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"end"
          t.end_
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"kind"
          t.kind
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"modifiers"
          t.modifiers
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"decorators"
          t.decorators
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_symbol
            )
          ~name:"resolvedSymbol"
          t.resolvedSymbol
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"resolvedType"
          t.resolvedType
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"dotDotDotToken"
          t.dotDotDotToken
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write_node
            )
          ~name:"name"
          t.name
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"questionToken"
          t.questionToken
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"type"
          t.type_
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"initializer"
          t.initializer_
      ]
    )
  )
) js
and write_node_PropertyDeclaration js = (
  Atdgen_codec_runtime.Encode.make (fun (t : node_PropertyDeclaration) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"pos"
          t.pos
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"end"
          t.end_
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"kind"
          t.kind
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"modifiers"
          t.modifiers
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"decorators"
          t.decorators
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_symbol
            )
          ~name:"resolvedSymbol"
          t.resolvedSymbol
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"resolvedType"
          t.resolvedType
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write_node
            )
          ~name:"name"
          t.name
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"questionToken"
          t.questionToken
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"exclamationToken"
          t.exclamationToken
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"type"
          t.type_
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"initializer"
          t.initializer_
      ]
    )
  )
) js
and write_node_PropertySignature js = (
  Atdgen_codec_runtime.Encode.make (fun (t : node_PropertySignature) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"pos"
          t.pos
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"end"
          t.end_
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"kind"
          t.kind
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"modifiers"
          t.modifiers
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"decorators"
          t.decorators
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_symbol
            )
          ~name:"resolvedSymbol"
          t.resolvedSymbol
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"resolvedType"
          t.resolvedType
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write_node
            )
          ~name:"name"
          t.name
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"questionToken"
          t.questionToken
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"type"
          t.type_
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"initializer"
          t.initializer_
      ]
    )
  )
) js
and write_node_QualifiedName js = (
  Atdgen_codec_runtime.Encode.make (fun (t : node_QualifiedName) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"pos"
          t.pos
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"end"
          t.end_
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"kind"
          t.kind
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"modifiers"
          t.modifiers
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"decorators"
          t.decorators
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_symbol
            )
          ~name:"resolvedSymbol"
          t.resolvedSymbol
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"resolvedType"
          t.resolvedType
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write_node
            )
          ~name:"left"
          t.left
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write_node
            )
          ~name:"right"
          t.right
      ]
    )
  )
) js
and write_node_SourceFile js = (
  Atdgen_codec_runtime.Encode.make (fun (t : node_SourceFile) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"pos"
          t.pos
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"end"
          t.end_
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"kind"
          t.kind
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"modifiers"
          t.modifiers
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"decorators"
          t.decorators
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_symbol
            )
          ~name:"resolvedSymbol"
          t.resolvedSymbol
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"resolvedType"
          t.resolvedType
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write__1
            )
          ~name:"statements"
          t.statements
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"text"
          t.text
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"languageVersion"
          t.languageVersion
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"fileName"
          t.fileName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"languageVariant"
          t.languageVariant
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.bool
            )
          ~name:"isDeclarationFile"
          t.isDeclarationFile
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"nodeCount"
          t.nodeCount
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"identifierCount"
          t.identifierCount
      ]
    )
  )
) js
and write_node_TypeAliasDeclaration js = (
  Atdgen_codec_runtime.Encode.make (fun (t : node_TypeAliasDeclaration) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"pos"
          t.pos
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"end"
          t.end_
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"kind"
          t.kind
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"modifiers"
          t.modifiers
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"decorators"
          t.decorators
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_symbol
            )
          ~name:"resolvedSymbol"
          t.resolvedSymbol
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"resolvedType"
          t.resolvedType
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write_node
            )
          ~name:"name"
          t.name
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"typeParameters"
          t.typeParameters
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write_node
            )
          ~name:"type"
          t.type_
      ]
    )
  )
) js
and write_node_TypeLiteral js = (
  Atdgen_codec_runtime.Encode.make (fun (t : node_TypeLiteral) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"pos"
          t.pos
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"end"
          t.end_
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"kind"
          t.kind
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"modifiers"
          t.modifiers
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"decorators"
          t.decorators
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_symbol
            )
          ~name:"resolvedSymbol"
          t.resolvedSymbol
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"resolvedType"
          t.resolvedType
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write__1
            )
          ~name:"members"
          t.members
      ]
    )
  )
) js
and write_node_TypeOperator js = (
  Atdgen_codec_runtime.Encode.make (fun (t : node_TypeOperator) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"pos"
          t.pos
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"end"
          t.end_
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"kind"
          t.kind
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"modifiers"
          t.modifiers
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"decorators"
          t.decorators
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_symbol
            )
          ~name:"resolvedSymbol"
          t.resolvedSymbol
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"resolvedType"
          t.resolvedType
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"operator"
          t.operator
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write_node
            )
          ~name:"type"
          t.type_
      ]
    )
  )
) js
and write_node_TypeParameter js = (
  Atdgen_codec_runtime.Encode.make (fun (t : node_TypeParameter) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"pos"
          t.pos
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"end"
          t.end_
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"kind"
          t.kind
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"modifiers"
          t.modifiers
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"decorators"
          t.decorators
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_symbol
            )
          ~name:"resolvedSymbol"
          t.resolvedSymbol
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"resolvedType"
          t.resolvedType
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write_node
            )
          ~name:"name"
          t.name
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"constraint"
          t.constraint_
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"default"
          t.default
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"expression"
          t.expression
      ]
    )
  )
) js
and write_node_TypeReference js = (
  Atdgen_codec_runtime.Encode.make (fun (t : node_TypeReference) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"pos"
          t.pos
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"end"
          t.end_
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"kind"
          t.kind
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"modifiers"
          t.modifiers
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"decorators"
          t.decorators
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_symbol
            )
          ~name:"resolvedSymbol"
          t.resolvedSymbol
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"resolvedType"
          t.resolvedType
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"typeArguments"
          t.typeArguments
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write_node
            )
          ~name:"typeName"
          t.typeName
      ]
    )
  )
) js
and write_node_UnionType js = (
  Atdgen_codec_runtime.Encode.make (fun (t : node_UnionType) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"pos"
          t.pos
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"end"
          t.end_
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"kind"
          t.kind
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"modifiers"
          t.modifiers
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"decorators"
          t.decorators
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_symbol
            )
          ~name:"resolvedSymbol"
          t.resolvedSymbol
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"resolvedType"
          t.resolvedType
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write__1
            )
          ~name:"types"
          t.types
      ]
    )
  )
) js
and write_node_VariableDeclaration js = (
  Atdgen_codec_runtime.Encode.make (fun (t : node_VariableDeclaration) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"pos"
          t.pos
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"end"
          t.end_
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"kind"
          t.kind
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"modifiers"
          t.modifiers
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"decorators"
          t.decorators
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_symbol
            )
          ~name:"resolvedSymbol"
          t.resolvedSymbol
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"resolvedType"
          t.resolvedType
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write_node
            )
          ~name:"name"
          t.name
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"exclamationToken"
          t.exclamationToken
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"type"
          t.type_
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"initializer"
          t.initializer_
      ]
    )
  )
) js
and write_node_VariableDeclarationList js = (
  Atdgen_codec_runtime.Encode.make (fun (t : node_VariableDeclarationList) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"pos"
          t.pos
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"end"
          t.end_
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"kind"
          t.kind
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"modifiers"
          t.modifiers
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"decorators"
          t.decorators
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_symbol
            )
          ~name:"resolvedSymbol"
          t.resolvedSymbol
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"resolvedType"
          t.resolvedType
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write__1
            )
          ~name:"declarations"
          t.declarations
      ]
    )
  )
) js
and write_symbol js = (
  Atdgen_codec_runtime.Encode.make (fun (t : symbol) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"id"
          t.id
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"resolvedType"
          t.resolvedType
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"fullyQualifiedName"
          t.fullyQualifiedName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"name"
          t.name
      ]
    )
  )
) js
and write_type_ js = (
  Atdgen_codec_runtime.Encode.adapter Typescript_json_adapters.KindName.restore (
    Atdgen_codec_runtime.Encode.make (fun (x : _) -> match x with
      | `ObjectKeyword x ->
      Atdgen_codec_runtime.Encode.constr1 "ObjectKeyword" (
        write_type_Generic
      ) x
      | `Void x ->
      Atdgen_codec_runtime.Encode.constr1 "Void" (
        write_type_Generic
      ) x
      | `Any x ->
      Atdgen_codec_runtime.Encode.constr1 "Any" (
        write_type_Generic
      ) x
      | `Array x ->
      Atdgen_codec_runtime.Encode.constr1 "Array" (
        write_type_Generic
      ) x
      | `Boolean x ->
      Atdgen_codec_runtime.Encode.constr1 "Boolean" (
        write_type_Generic
      ) x
      | `String x ->
      Atdgen_codec_runtime.Encode.constr1 "String" (
        write_type_Generic
      ) x
      | `Number x ->
      Atdgen_codec_runtime.Encode.constr1 "Number" (
        write_type_Generic
      ) x
      | `Literal x ->
      Atdgen_codec_runtime.Encode.constr1 "Literal" (
        write_type_Generic
      ) x
      | `BooleanLiteral x ->
      Atdgen_codec_runtime.Encode.constr1 "BooleanLiteral" (
        write_type_Generic
      ) x
      | `Union x ->
      Atdgen_codec_runtime.Encode.constr1 "Union" (
        write_type_UnionType
      ) x
      | `EnumLiteral x ->
      Atdgen_codec_runtime.Encode.constr1 "EnumLiteral" (
        write_type_StringLiteralType
      ) x
      | `NumberLiteral x ->
      Atdgen_codec_runtime.Encode.constr1 "NumberLiteral" (
        write_type_NumberLiteralType
      ) x
      | `StringLiteral x ->
      Atdgen_codec_runtime.Encode.constr1 "StringLiteral" (
        write_type_StringLiteralType
      ) x
      | `Class x ->
      Atdgen_codec_runtime.Encode.constr1 "Class" (
        write_type_Generic
      ) x
      | `ClassOrInterface x ->
      Atdgen_codec_runtime.Encode.constr1 "ClassOrInterface" (
        write_type_Generic
      ) x
      | `Interface x ->
      Atdgen_codec_runtime.Encode.constr1 "Interface" (
        write_type_InterfaceType
      ) x
      | `Object x ->
      Atdgen_codec_runtime.Encode.constr1 "Object" (
        write_type_ObjectType
      ) x
      | `Enum x ->
      Atdgen_codec_runtime.Encode.constr1 "Enum" (
        write_type_Generic
      ) x
      | `TypeParameter x ->
      Atdgen_codec_runtime.Encode.constr1 "TypeParameter" (
        write_type_TypeParameter
      ) x
      | `Tuple x ->
      Atdgen_codec_runtime.Encode.constr1 "Tuple" (
        write_type_TupleType
      ) x
      | `Intersection x ->
      Atdgen_codec_runtime.Encode.constr1 "Intersection" (
        write_type_IntersectionType
      ) x
      | `UnionOrIntersection x ->
      Atdgen_codec_runtime.Encode.constr1 "UnionOrIntersection" (
        write_type_UnionOrIntersectionType
      ) x
      | `Unknown x ->
      Atdgen_codec_runtime.Encode.constr1 "Unknown" (
        write_type_Generic
      ) x
      | `Null x ->
      Atdgen_codec_runtime.Encode.constr1 "Null" (
        write_type_Generic
      ) x
      | `Undefined x ->
      Atdgen_codec_runtime.Encode.constr1 "Undefined" (
        write_type_Generic
      ) x
      | `TypeReference x ->
      Atdgen_codec_runtime.Encode.constr1 "TypeReference" (
        write_type_TypeReference
      ) x
      | `Anonymous x ->
      Atdgen_codec_runtime.Encode.constr1 "Anonymous" (
        write_type_AnonymousType
      ) x
      | `Unidentified x ->
      Atdgen_codec_runtime.Encode.constr1 "Unidentified" (
        write_type_Unidentified
      ) x
    )
  )
) js
and write_type_AnonymousType js = (
  Atdgen_codec_runtime.Encode.make (fun (t : type_AnonymousType) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"id"
          t.id
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"pattern"
          t.pattern
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__7
            )
          ~name:"aliasTypeArguments"
          t.aliasTypeArguments
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"objectFlags"
          t.objectFlags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"target"
          t.target
      ]
    )
  )
) js
and write_type_Generic js = (
  Atdgen_codec_runtime.Encode.make (fun (t : type_Generic) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"id"
          t.id
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"pattern"
          t.pattern
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__7
            )
          ~name:"aliasTypeArguments"
          t.aliasTypeArguments
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
      ]
    )
  )
) js
and write_type_InterfaceType js = (
  Atdgen_codec_runtime.Encode.make (fun (t : type_InterfaceType) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"id"
          t.id
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"pattern"
          t.pattern
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__7
            )
          ~name:"aliasTypeArguments"
          t.aliasTypeArguments
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"objectFlags"
          t.objectFlags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__7
            )
          ~name:"typeParameters"
          t.typeParameters
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__7
            )
          ~name:"outerTypeParameters"
          t.outerTypeParameters
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__7
            )
          ~name:"localTypeParameters"
          t.localTypeParameters
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"thisType"
          t.thisType
      ]
    )
  )
) js
and write_type_IntersectionType js = (
  Atdgen_codec_runtime.Encode.make (fun (t : type_IntersectionType) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"id"
          t.id
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"pattern"
          t.pattern
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__7
            )
          ~name:"aliasTypeArguments"
          t.aliasTypeArguments
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write__7
            )
          ~name:"types"
          t.types
      ]
    )
  )
) js
and write_type_NumberLiteralType js = (
  Atdgen_codec_runtime.Encode.make (fun (t : type_NumberLiteralType) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"id"
          t.id
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"pattern"
          t.pattern
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__7
            )
          ~name:"aliasTypeArguments"
          t.aliasTypeArguments
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.float
            )
          ~name:"value"
          t.value
      ]
    )
  )
) js
and write_type_ObjectType js = (
  Atdgen_codec_runtime.Encode.make (fun (t : type_ObjectType) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"id"
          t.id
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"pattern"
          t.pattern
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__7
            )
          ~name:"aliasTypeArguments"
          t.aliasTypeArguments
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"objectFlags"
          t.objectFlags
      ]
    )
  )
) js
and write_type_StringLiteralType js = (
  Atdgen_codec_runtime.Encode.make (fun (t : type_StringLiteralType) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"id"
          t.id
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"pattern"
          t.pattern
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__7
            )
          ~name:"aliasTypeArguments"
          t.aliasTypeArguments
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"value"
          t.value
      ]
    )
  )
) js
and write_type_TupleType js = (
  Atdgen_codec_runtime.Encode.make (fun (t : type_TupleType) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"id"
          t.id
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"pattern"
          t.pattern
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__7
            )
          ~name:"aliasTypeArguments"
          t.aliasTypeArguments
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"minLength"
          t.minLength
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"fixedLength"
          t.fixedLength
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.bool
            )
          ~name:"hasRestElement"
          t.hasRestElement
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.bool
            )
          ~name:"readonly"
          t.readonly
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write__9
            )
          ~name:"elementFlags"
          t.elementFlags
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"combinedFlags"
          t.combinedFlags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"labeledElementDeclarations"
          t.labeledElementDeclarations
      ]
    )
  )
) js
and write_type_TypeParameter js = (
  Atdgen_codec_runtime.Encode.make (fun (t : type_TypeParameter) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"id"
          t.id
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"pattern"
          t.pattern
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__7
            )
          ~name:"aliasTypeArguments"
          t.aliasTypeArguments
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"constraint"
          t.constraint_
      ]
    )
  )
) js
and write_type_TypeReference js = (
  Atdgen_codec_runtime.Encode.make (fun (t : type_TypeReference) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"id"
          t.id
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"pattern"
          t.pattern
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__7
            )
          ~name:"aliasTypeArguments"
          t.aliasTypeArguments
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"target"
          t.target
      ]
    )
  )
) js
and write_type_Unidentified js = (
  Atdgen_codec_runtime.Encode.make (fun (t : type_Unidentified) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"id"
          t.id
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"pattern"
          t.pattern
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__7
            )
          ~name:"aliasTypeArguments"
          t.aliasTypeArguments
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"nodeKind"
          t.nodeKind
      ]
    )
  )
) js
and write_type_UnionOrIntersectionType js = (
  Atdgen_codec_runtime.Encode.make (fun (t : type_UnionOrIntersectionType) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"id"
          t.id
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"pattern"
          t.pattern
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__7
            )
          ~name:"aliasTypeArguments"
          t.aliasTypeArguments
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write__7
            )
          ~name:"types"
          t.types
      ]
    )
  )
) js
and write_type_UnionType js = (
  Atdgen_codec_runtime.Encode.make (fun (t : type_UnionType) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"id"
          t.id
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"pattern"
          t.pattern
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__7
            )
          ~name:"aliasTypeArguments"
          t.aliasTypeArguments
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write__7
            )
          ~name:"types"
          t.types
      ]
    )
  )
) js
let rec read__1 js = (
  Atdgen_codec_runtime.Decode.list (
    read_node
  )
) js
and read__2 js = (
  Atdgen_codec_runtime.Decode.option_as_constr (
    read__1
  )
) js
and read__3 js = (
  Atdgen_codec_runtime.Decode.option_as_constr (
    read_symbol
  )
) js
and read__4 js = (
  Atdgen_codec_runtime.Decode.option_as_constr (
    read_type_
  )
) js
and read__5 js = (
  Atdgen_codec_runtime.Decode.option_as_constr (
    read_node
  )
) js
and read__7 js = (
  Atdgen_codec_runtime.Decode.list (
    read_type_
  )
) js
and read__8 js = (
  Atdgen_codec_runtime.Decode.option_as_constr (
    read__7
  )
) js
and read_node js = (
  Atdgen_codec_runtime.Decode.adapter Typescript_json_adapters.KindName.normalize (
    Atdgen_codec_runtime.Decode.enum
    [
        (
        "SourceFile"
        ,
          `Decode (
          read_node_SourceFile
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`SourceFile x) : _))
          )
        )
      ;
        (
        "TypeAliasDeclaration"
        ,
          `Decode (
          read_node_TypeAliasDeclaration
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`TypeAliasDeclaration x) : _))
          )
        )
      ;
        (
        "TypeParameter"
        ,
          `Decode (
          read_node_TypeParameter
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`TypeParameter x) : _))
          )
        )
      ;
        (
        "InterfaceDeclaration"
        ,
          `Decode (
          read_node_InterfaceDeclaration
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`InterfaceDeclaration x) : _))
          )
        )
      ;
        (
        "HeritageClause"
        ,
          `Decode (
          read_node_HeritageClause
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`HeritageClause x) : _))
          )
        )
      ;
        (
        "Identifier"
        ,
          `Decode (
          read_node_Identifier
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`Identifier x) : _))
          )
        )
      ;
        (
        "PrivateIdentifier"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`PrivateIdentifier x) : _))
          )
        )
      ;
        (
        "StringLiteral"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`StringLiteral x) : _))
          )
        )
      ;
        (
        "NumericLiteral"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`NumericLiteral x) : _))
          )
        )
      ;
        (
        "Unknown"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`Unknown x) : _))
          )
        )
      ;
        (
        "EndOfFileToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`EndOfFileToken x) : _))
          )
        )
      ;
        (
        "SingleLineCommentTrivia"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`SingleLineCommentTrivia x) : _))
          )
        )
      ;
        (
        "MultiLineCommentTrivia"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`MultiLineCommentTrivia x) : _))
          )
        )
      ;
        (
        "NewLineTrivia"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`NewLineTrivia x) : _))
          )
        )
      ;
        (
        "WhitespaceTrivia"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`WhitespaceTrivia x) : _))
          )
        )
      ;
        (
        "ShebangTrivia"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ShebangTrivia x) : _))
          )
        )
      ;
        (
        "ConflictMarkerTrivia"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ConflictMarkerTrivia x) : _))
          )
        )
      ;
        (
        "BigIntLiteral"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`BigIntLiteral x) : _))
          )
        )
      ;
        (
        "JsxText"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JsxText x) : _))
          )
        )
      ;
        (
        "JsxTextAllWhiteSpaces"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JsxTextAllWhiteSpaces x) : _))
          )
        )
      ;
        (
        "RegularExpressionLiteral"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`RegularExpressionLiteral x) : _))
          )
        )
      ;
        (
        "NoSubstitutionTemplateLiteral"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`NoSubstitutionTemplateLiteral x) : _))
          )
        )
      ;
        (
        "TemplateHead"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`TemplateHead x) : _))
          )
        )
      ;
        (
        "TemplateMiddle"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`TemplateMiddle x) : _))
          )
        )
      ;
        (
        "TemplateTail"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`TemplateTail x) : _))
          )
        )
      ;
        (
        "OpenBraceToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`OpenBraceToken x) : _))
          )
        )
      ;
        (
        "CloseBraceToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`CloseBraceToken x) : _))
          )
        )
      ;
        (
        "OpenParenToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`OpenParenToken x) : _))
          )
        )
      ;
        (
        "CloseParenToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`CloseParenToken x) : _))
          )
        )
      ;
        (
        "OpenBracketToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`OpenBracketToken x) : _))
          )
        )
      ;
        (
        "CloseBracketToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`CloseBracketToken x) : _))
          )
        )
      ;
        (
        "DotToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`DotToken x) : _))
          )
        )
      ;
        (
        "DotDotDotToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`DotDotDotToken x) : _))
          )
        )
      ;
        (
        "SemicolonToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`SemicolonToken x) : _))
          )
        )
      ;
        (
        "CommaToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`CommaToken x) : _))
          )
        )
      ;
        (
        "QuestionDotToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`QuestionDotToken x) : _))
          )
        )
      ;
        (
        "LessThanToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`LessThanToken x) : _))
          )
        )
      ;
        (
        "LessThanSlashToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`LessThanSlashToken x) : _))
          )
        )
      ;
        (
        "GreaterThanToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`GreaterThanToken x) : _))
          )
        )
      ;
        (
        "LessThanEqualsToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`LessThanEqualsToken x) : _))
          )
        )
      ;
        (
        "GreaterThanEqualsToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`GreaterThanEqualsToken x) : _))
          )
        )
      ;
        (
        "EqualsEqualsToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`EqualsEqualsToken x) : _))
          )
        )
      ;
        (
        "ExclamationEqualsToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ExclamationEqualsToken x) : _))
          )
        )
      ;
        (
        "EqualsEqualsEqualsToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`EqualsEqualsEqualsToken x) : _))
          )
        )
      ;
        (
        "ExclamationEqualsEqualsToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ExclamationEqualsEqualsToken x) : _))
          )
        )
      ;
        (
        "EqualsGreaterThanToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`EqualsGreaterThanToken x) : _))
          )
        )
      ;
        (
        "PlusToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`PlusToken x) : _))
          )
        )
      ;
        (
        "MinusToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`MinusToken x) : _))
          )
        )
      ;
        (
        "AsteriskToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`AsteriskToken x) : _))
          )
        )
      ;
        (
        "AsteriskAsteriskToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`AsteriskAsteriskToken x) : _))
          )
        )
      ;
        (
        "SlashToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`SlashToken x) : _))
          )
        )
      ;
        (
        "PercentToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`PercentToken x) : _))
          )
        )
      ;
        (
        "PlusPlusToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`PlusPlusToken x) : _))
          )
        )
      ;
        (
        "MinusMinusToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`MinusMinusToken x) : _))
          )
        )
      ;
        (
        "LessThanLessThanToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`LessThanLessThanToken x) : _))
          )
        )
      ;
        (
        "GreaterThanGreaterThanToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`GreaterThanGreaterThanToken x) : _))
          )
        )
      ;
        (
        "GreaterThanGreaterThanGreaterThanToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`GreaterThanGreaterThanGreaterThanToken x) : _))
          )
        )
      ;
        (
        "AmpersandToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`AmpersandToken x) : _))
          )
        )
      ;
        (
        "BarToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`BarToken x) : _))
          )
        )
      ;
        (
        "CaretToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`CaretToken x) : _))
          )
        )
      ;
        (
        "ExclamationToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ExclamationToken x) : _))
          )
        )
      ;
        (
        "TildeToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`TildeToken x) : _))
          )
        )
      ;
        (
        "AmpersandAmpersandToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`AmpersandAmpersandToken x) : _))
          )
        )
      ;
        (
        "BarBarToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`BarBarToken x) : _))
          )
        )
      ;
        (
        "QuestionToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`QuestionToken x) : _))
          )
        )
      ;
        (
        "ColonToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ColonToken x) : _))
          )
        )
      ;
        (
        "AtToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`AtToken x) : _))
          )
        )
      ;
        (
        "QuestionQuestionToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`QuestionQuestionToken x) : _))
          )
        )
      ;
        (
        "BacktickToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`BacktickToken x) : _))
          )
        )
      ;
        (
        "EqualsToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`EqualsToken x) : _))
          )
        )
      ;
        (
        "PlusEqualsToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`PlusEqualsToken x) : _))
          )
        )
      ;
        (
        "MinusEqualsToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`MinusEqualsToken x) : _))
          )
        )
      ;
        (
        "AsteriskEqualsToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`AsteriskEqualsToken x) : _))
          )
        )
      ;
        (
        "AsteriskAsteriskEqualsToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`AsteriskAsteriskEqualsToken x) : _))
          )
        )
      ;
        (
        "SlashEqualsToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`SlashEqualsToken x) : _))
          )
        )
      ;
        (
        "PercentEqualsToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`PercentEqualsToken x) : _))
          )
        )
      ;
        (
        "LessThanLessThanEqualsToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`LessThanLessThanEqualsToken x) : _))
          )
        )
      ;
        (
        "GreaterThanGreaterThanEqualsToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`GreaterThanGreaterThanEqualsToken x) : _))
          )
        )
      ;
        (
        "GreaterThanGreaterThanGreaterThanEqualsToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`GreaterThanGreaterThanGreaterThanEqualsToken x) : _))
          )
        )
      ;
        (
        "AmpersandEqualsToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`AmpersandEqualsToken x) : _))
          )
        )
      ;
        (
        "BarEqualsToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`BarEqualsToken x) : _))
          )
        )
      ;
        (
        "BarBarEqualsToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`BarBarEqualsToken x) : _))
          )
        )
      ;
        (
        "AmpersandAmpersandEqualsToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`AmpersandAmpersandEqualsToken x) : _))
          )
        )
      ;
        (
        "QuestionQuestionEqualsToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`QuestionQuestionEqualsToken x) : _))
          )
        )
      ;
        (
        "CaretEqualsToken"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`CaretEqualsToken x) : _))
          )
        )
      ;
        (
        "BreakKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`BreakKeyword x) : _))
          )
        )
      ;
        (
        "CaseKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`CaseKeyword x) : _))
          )
        )
      ;
        (
        "CatchKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`CatchKeyword x) : _))
          )
        )
      ;
        (
        "ClassKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ClassKeyword x) : _))
          )
        )
      ;
        (
        "ConstKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ConstKeyword x) : _))
          )
        )
      ;
        (
        "ContinueKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ContinueKeyword x) : _))
          )
        )
      ;
        (
        "DebuggerKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`DebuggerKeyword x) : _))
          )
        )
      ;
        (
        "DefaultKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`DefaultKeyword x) : _))
          )
        )
      ;
        (
        "DeleteKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`DeleteKeyword x) : _))
          )
        )
      ;
        (
        "DoKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`DoKeyword x) : _))
          )
        )
      ;
        (
        "ElseKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ElseKeyword x) : _))
          )
        )
      ;
        (
        "EnumKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`EnumKeyword x) : _))
          )
        )
      ;
        (
        "ExportKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ExportKeyword x) : _))
          )
        )
      ;
        (
        "ExtendsKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ExtendsKeyword x) : _))
          )
        )
      ;
        (
        "FalseKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`FalseKeyword x) : _))
          )
        )
      ;
        (
        "FinallyKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`FinallyKeyword x) : _))
          )
        )
      ;
        (
        "ForKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ForKeyword x) : _))
          )
        )
      ;
        (
        "FunctionKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`FunctionKeyword x) : _))
          )
        )
      ;
        (
        "IfKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`IfKeyword x) : _))
          )
        )
      ;
        (
        "ImportKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ImportKeyword x) : _))
          )
        )
      ;
        (
        "InKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`InKeyword x) : _))
          )
        )
      ;
        (
        "InstanceOfKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`InstanceOfKeyword x) : _))
          )
        )
      ;
        (
        "NewKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`NewKeyword x) : _))
          )
        )
      ;
        (
        "NullKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`NullKeyword x) : _))
          )
        )
      ;
        (
        "ReturnKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ReturnKeyword x) : _))
          )
        )
      ;
        (
        "SuperKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`SuperKeyword x) : _))
          )
        )
      ;
        (
        "SwitchKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`SwitchKeyword x) : _))
          )
        )
      ;
        (
        "ThisKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ThisKeyword x) : _))
          )
        )
      ;
        (
        "ThrowKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ThrowKeyword x) : _))
          )
        )
      ;
        (
        "TrueKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`TrueKeyword x) : _))
          )
        )
      ;
        (
        "TryKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`TryKeyword x) : _))
          )
        )
      ;
        (
        "TypeOfKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`TypeOfKeyword x) : _))
          )
        )
      ;
        (
        "VarKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`VarKeyword x) : _))
          )
        )
      ;
        (
        "VoidKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`VoidKeyword x) : _))
          )
        )
      ;
        (
        "WhileKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`WhileKeyword x) : _))
          )
        )
      ;
        (
        "WithKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`WithKeyword x) : _))
          )
        )
      ;
        (
        "ImplementsKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ImplementsKeyword x) : _))
          )
        )
      ;
        (
        "InterfaceKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`InterfaceKeyword x) : _))
          )
        )
      ;
        (
        "LetKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`LetKeyword x) : _))
          )
        )
      ;
        (
        "PackageKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`PackageKeyword x) : _))
          )
        )
      ;
        (
        "PrivateKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`PrivateKeyword x) : _))
          )
        )
      ;
        (
        "ProtectedKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ProtectedKeyword x) : _))
          )
        )
      ;
        (
        "PublicKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`PublicKeyword x) : _))
          )
        )
      ;
        (
        "StaticKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`StaticKeyword x) : _))
          )
        )
      ;
        (
        "YieldKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`YieldKeyword x) : _))
          )
        )
      ;
        (
        "AbstractKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`AbstractKeyword x) : _))
          )
        )
      ;
        (
        "AsKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`AsKeyword x) : _))
          )
        )
      ;
        (
        "AssertsKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`AssertsKeyword x) : _))
          )
        )
      ;
        (
        "AnyKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`AnyKeyword x) : _))
          )
        )
      ;
        (
        "AsyncKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`AsyncKeyword x) : _))
          )
        )
      ;
        (
        "AwaitKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`AwaitKeyword x) : _))
          )
        )
      ;
        (
        "BooleanKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`BooleanKeyword x) : _))
          )
        )
      ;
        (
        "ConstructorKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ConstructorKeyword x) : _))
          )
        )
      ;
        (
        "DeclareKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`DeclareKeyword x) : _))
          )
        )
      ;
        (
        "GetKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`GetKeyword x) : _))
          )
        )
      ;
        (
        "InferKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`InferKeyword x) : _))
          )
        )
      ;
        (
        "IsKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`IsKeyword x) : _))
          )
        )
      ;
        (
        "KeyOfKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`KeyOfKeyword x) : _))
          )
        )
      ;
        (
        "ModuleKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ModuleKeyword x) : _))
          )
        )
      ;
        (
        "NamespaceKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`NamespaceKeyword x) : _))
          )
        )
      ;
        (
        "NeverKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`NeverKeyword x) : _))
          )
        )
      ;
        (
        "ReadonlyKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ReadonlyKeyword x) : _))
          )
        )
      ;
        (
        "RequireKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`RequireKeyword x) : _))
          )
        )
      ;
        (
        "NumberKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`NumberKeyword x) : _))
          )
        )
      ;
        (
        "ObjectKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ObjectKeyword x) : _))
          )
        )
      ;
        (
        "SetKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`SetKeyword x) : _))
          )
        )
      ;
        (
        "StringKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`StringKeyword x) : _))
          )
        )
      ;
        (
        "SymbolKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`SymbolKeyword x) : _))
          )
        )
      ;
        (
        "TypeKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`TypeKeyword x) : _))
          )
        )
      ;
        (
        "UndefinedKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`UndefinedKeyword x) : _))
          )
        )
      ;
        (
        "UniqueKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`UniqueKeyword x) : _))
          )
        )
      ;
        (
        "UnknownKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`UnknownKeyword x) : _))
          )
        )
      ;
        (
        "FromKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`FromKeyword x) : _))
          )
        )
      ;
        (
        "GlobalKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`GlobalKeyword x) : _))
          )
        )
      ;
        (
        "BigIntKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`BigIntKeyword x) : _))
          )
        )
      ;
        (
        "OfKeyword"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`OfKeyword x) : _))
          )
        )
      ;
        (
        "QualifiedName"
        ,
          `Decode (
          read_node_QualifiedName
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`QualifiedName x) : _))
          )
        )
      ;
        (
        "ComputedPropertyName"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ComputedPropertyName x) : _))
          )
        )
      ;
        (
        "Parameter"
        ,
          `Decode (
          read_node_Parameter
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`Parameter x) : _))
          )
        )
      ;
        (
        "Decorator"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`Decorator x) : _))
          )
        )
      ;
        (
        "PropertySignature"
        ,
          `Decode (
          read_node_PropertySignature
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`PropertySignature x) : _))
          )
        )
      ;
        (
        "PropertyDeclaration"
        ,
          `Decode (
          read_node_PropertyDeclaration
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`PropertyDeclaration x) : _))
          )
        )
      ;
        (
        "MethodSignature"
        ,
          `Decode (
          read_node_MethodSignature
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`MethodSignature x) : _))
          )
        )
      ;
        (
        "MethodDeclaration"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`MethodDeclaration x) : _))
          )
        )
      ;
        (
        "Constructor"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`Constructor x) : _))
          )
        )
      ;
        (
        "GetAccessor"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`GetAccessor x) : _))
          )
        )
      ;
        (
        "SetAccessor"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`SetAccessor x) : _))
          )
        )
      ;
        (
        "CallSignature"
        ,
          `Decode (
          read_node_CallSignature
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`CallSignature x) : _))
          )
        )
      ;
        (
        "ConstructSignature"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ConstructSignature x) : _))
          )
        )
      ;
        (
        "IndexSignature"
        ,
          `Decode (
          read_node_IndexSignature
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`IndexSignature x) : _))
          )
        )
      ;
        (
        "TypePredicate"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`TypePredicate x) : _))
          )
        )
      ;
        (
        "TypeReference"
        ,
          `Decode (
          read_node_TypeReference
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`TypeReference x) : _))
          )
        )
      ;
        (
        "FunctionType"
        ,
          `Decode (
          read_node_FunctionType
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`FunctionType x) : _))
          )
        )
      ;
        (
        "ConstructorType"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ConstructorType x) : _))
          )
        )
      ;
        (
        "TypeQuery"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`TypeQuery x) : _))
          )
        )
      ;
        (
        "TypeLiteral"
        ,
          `Decode (
          read_node_TypeLiteral
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`TypeLiteral x) : _))
          )
        )
      ;
        (
        "ArrayType"
        ,
          `Decode (
          read_node_ArrayType
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ArrayType x) : _))
          )
        )
      ;
        (
        "TupleType"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`TupleType x) : _))
          )
        )
      ;
        (
        "OptionalType"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`OptionalType x) : _))
          )
        )
      ;
        (
        "RestType"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`RestType x) : _))
          )
        )
      ;
        (
        "UnionType"
        ,
          `Decode (
          read_node_UnionType
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`UnionType x) : _))
          )
        )
      ;
        (
        "IntersectionType"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`IntersectionType x) : _))
          )
        )
      ;
        (
        "ConditionalType"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ConditionalType x) : _))
          )
        )
      ;
        (
        "InferType"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`InferType x) : _))
          )
        )
      ;
        (
        "ParenthesizedType"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ParenthesizedType x) : _))
          )
        )
      ;
        (
        "ThisType"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ThisType x) : _))
          )
        )
      ;
        (
        "TypeOperator"
        ,
          `Decode (
          read_node_TypeOperator
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`TypeOperator x) : _))
          )
        )
      ;
        (
        "IndexedAccessType"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`IndexedAccessType x) : _))
          )
        )
      ;
        (
        "MappedType"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`MappedType x) : _))
          )
        )
      ;
        (
        "LiteralType"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`LiteralType x) : _))
          )
        )
      ;
        (
        "NamedTupleMember"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`NamedTupleMember x) : _))
          )
        )
      ;
        (
        "ImportType"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ImportType x) : _))
          )
        )
      ;
        (
        "ObjectBindingPattern"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ObjectBindingPattern x) : _))
          )
        )
      ;
        (
        "ArrayBindingPattern"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ArrayBindingPattern x) : _))
          )
        )
      ;
        (
        "BindingElement"
        ,
          `Decode (
          read_node_BindingElement
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`BindingElement x) : _))
          )
        )
      ;
        (
        "ArrayLiteralExpression"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ArrayLiteralExpression x) : _))
          )
        )
      ;
        (
        "ObjectLiteralExpression"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ObjectLiteralExpression x) : _))
          )
        )
      ;
        (
        "PropertyAccessExpression"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`PropertyAccessExpression x) : _))
          )
        )
      ;
        (
        "ElementAccessExpression"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ElementAccessExpression x) : _))
          )
        )
      ;
        (
        "CallExpression"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`CallExpression x) : _))
          )
        )
      ;
        (
        "NewExpression"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`NewExpression x) : _))
          )
        )
      ;
        (
        "TaggedTemplateExpression"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`TaggedTemplateExpression x) : _))
          )
        )
      ;
        (
        "TypeAssertionExpression"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`TypeAssertionExpression x) : _))
          )
        )
      ;
        (
        "ParenthesizedExpression"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ParenthesizedExpression x) : _))
          )
        )
      ;
        (
        "FunctionExpression"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`FunctionExpression x) : _))
          )
        )
      ;
        (
        "ArrowFunction"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ArrowFunction x) : _))
          )
        )
      ;
        (
        "DeleteExpression"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`DeleteExpression x) : _))
          )
        )
      ;
        (
        "TypeOfExpression"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`TypeOfExpression x) : _))
          )
        )
      ;
        (
        "VoidExpression"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`VoidExpression x) : _))
          )
        )
      ;
        (
        "AwaitExpression"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`AwaitExpression x) : _))
          )
        )
      ;
        (
        "PrefixUnaryExpression"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`PrefixUnaryExpression x) : _))
          )
        )
      ;
        (
        "PostfixUnaryExpression"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`PostfixUnaryExpression x) : _))
          )
        )
      ;
        (
        "BinaryExpression"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`BinaryExpression x) : _))
          )
        )
      ;
        (
        "ConditionalExpression"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ConditionalExpression x) : _))
          )
        )
      ;
        (
        "TemplateExpression"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`TemplateExpression x) : _))
          )
        )
      ;
        (
        "YieldExpression"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`YieldExpression x) : _))
          )
        )
      ;
        (
        "SpreadElement"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`SpreadElement x) : _))
          )
        )
      ;
        (
        "ClassExpression"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ClassExpression x) : _))
          )
        )
      ;
        (
        "OmittedExpression"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`OmittedExpression x) : _))
          )
        )
      ;
        (
        "ExpressionWithTypeArguments"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ExpressionWithTypeArguments x) : _))
          )
        )
      ;
        (
        "AsExpression"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`AsExpression x) : _))
          )
        )
      ;
        (
        "NonNullExpression"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`NonNullExpression x) : _))
          )
        )
      ;
        (
        "MetaProperty"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`MetaProperty x) : _))
          )
        )
      ;
        (
        "SyntheticExpression"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`SyntheticExpression x) : _))
          )
        )
      ;
        (
        "TemplateSpan"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`TemplateSpan x) : _))
          )
        )
      ;
        (
        "SemicolonClassElement"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`SemicolonClassElement x) : _))
          )
        )
      ;
        (
        "Block"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`Block x) : _))
          )
        )
      ;
        (
        "EmptyStatement"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`EmptyStatement x) : _))
          )
        )
      ;
        (
        "VariableStatement"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`VariableStatement x) : _))
          )
        )
      ;
        (
        "ExpressionStatement"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ExpressionStatement x) : _))
          )
        )
      ;
        (
        "IfStatement"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`IfStatement x) : _))
          )
        )
      ;
        (
        "DoStatement"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`DoStatement x) : _))
          )
        )
      ;
        (
        "WhileStatement"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`WhileStatement x) : _))
          )
        )
      ;
        (
        "ForStatement"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ForStatement x) : _))
          )
        )
      ;
        (
        "ForInStatement"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ForInStatement x) : _))
          )
        )
      ;
        (
        "ForOfStatement"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ForOfStatement x) : _))
          )
        )
      ;
        (
        "ContinueStatement"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ContinueStatement x) : _))
          )
        )
      ;
        (
        "BreakStatement"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`BreakStatement x) : _))
          )
        )
      ;
        (
        "ReturnStatement"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ReturnStatement x) : _))
          )
        )
      ;
        (
        "WithStatement"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`WithStatement x) : _))
          )
        )
      ;
        (
        "SwitchStatement"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`SwitchStatement x) : _))
          )
        )
      ;
        (
        "LabeledStatement"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`LabeledStatement x) : _))
          )
        )
      ;
        (
        "ThrowStatement"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ThrowStatement x) : _))
          )
        )
      ;
        (
        "TryStatement"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`TryStatement x) : _))
          )
        )
      ;
        (
        "DebuggerStatement"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`DebuggerStatement x) : _))
          )
        )
      ;
        (
        "VariableDeclaration"
        ,
          `Decode (
          read_node_VariableDeclaration
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`VariableDeclaration x) : _))
          )
        )
      ;
        (
        "VariableDeclarationList"
        ,
          `Decode (
          read_node_VariableDeclarationList
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`VariableDeclarationList x) : _))
          )
        )
      ;
        (
        "FunctionDeclaration"
        ,
          `Decode (
          read_node_FunctionDeclaration
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`FunctionDeclaration x) : _))
          )
        )
      ;
        (
        "ClassDeclaration"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ClassDeclaration x) : _))
          )
        )
      ;
        (
        "EnumDeclaration"
        ,
          `Decode (
          read_node_EnumDeclaration
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`EnumDeclaration x) : _))
          )
        )
      ;
        (
        "ModuleDeclaration"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ModuleDeclaration x) : _))
          )
        )
      ;
        (
        "ModuleBlock"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ModuleBlock x) : _))
          )
        )
      ;
        (
        "CaseBlock"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`CaseBlock x) : _))
          )
        )
      ;
        (
        "NamespaceExportDeclaration"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`NamespaceExportDeclaration x) : _))
          )
        )
      ;
        (
        "ImportEqualsDeclaration"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ImportEqualsDeclaration x) : _))
          )
        )
      ;
        (
        "ImportDeclaration"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ImportDeclaration x) : _))
          )
        )
      ;
        (
        "ImportClause"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ImportClause x) : _))
          )
        )
      ;
        (
        "NamespaceImport"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`NamespaceImport x) : _))
          )
        )
      ;
        (
        "NamedImports"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`NamedImports x) : _))
          )
        )
      ;
        (
        "ImportSpecifier"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ImportSpecifier x) : _))
          )
        )
      ;
        (
        "ExportAssignment"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ExportAssignment x) : _))
          )
        )
      ;
        (
        "ExportDeclaration"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ExportDeclaration x) : _))
          )
        )
      ;
        (
        "NamedExports"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`NamedExports x) : _))
          )
        )
      ;
        (
        "NamespaceExport"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`NamespaceExport x) : _))
          )
        )
      ;
        (
        "ExportSpecifier"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ExportSpecifier x) : _))
          )
        )
      ;
        (
        "MissingDeclaration"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`MissingDeclaration x) : _))
          )
        )
      ;
        (
        "ExternalModuleReference"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ExternalModuleReference x) : _))
          )
        )
      ;
        (
        "JsxElement"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JsxElement x) : _))
          )
        )
      ;
        (
        "JsxSelfClosingElement"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JsxSelfClosingElement x) : _))
          )
        )
      ;
        (
        "JsxOpeningElement"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JsxOpeningElement x) : _))
          )
        )
      ;
        (
        "JsxClosingElement"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JsxClosingElement x) : _))
          )
        )
      ;
        (
        "JsxFragment"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JsxFragment x) : _))
          )
        )
      ;
        (
        "JsxOpeningFragment"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JsxOpeningFragment x) : _))
          )
        )
      ;
        (
        "JsxClosingFragment"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JsxClosingFragment x) : _))
          )
        )
      ;
        (
        "JsxAttribute"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JsxAttribute x) : _))
          )
        )
      ;
        (
        "JsxAttributes"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JsxAttributes x) : _))
          )
        )
      ;
        (
        "JsxSpreadAttribute"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JsxSpreadAttribute x) : _))
          )
        )
      ;
        (
        "JsxExpression"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JsxExpression x) : _))
          )
        )
      ;
        (
        "CaseClause"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`CaseClause x) : _))
          )
        )
      ;
        (
        "DefaultClause"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`DefaultClause x) : _))
          )
        )
      ;
        (
        "CatchClause"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`CatchClause x) : _))
          )
        )
      ;
        (
        "PropertyAssignment"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`PropertyAssignment x) : _))
          )
        )
      ;
        (
        "ShorthandPropertyAssignment"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ShorthandPropertyAssignment x) : _))
          )
        )
      ;
        (
        "SpreadAssignment"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`SpreadAssignment x) : _))
          )
        )
      ;
        (
        "EnumMember"
        ,
          `Decode (
          read_node_EnumMember
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`EnumMember x) : _))
          )
        )
      ;
        (
        "UnparsedPrologue"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`UnparsedPrologue x) : _))
          )
        )
      ;
        (
        "UnparsedPrepend"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`UnparsedPrepend x) : _))
          )
        )
      ;
        (
        "UnparsedText"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`UnparsedText x) : _))
          )
        )
      ;
        (
        "UnparsedInternalText"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`UnparsedInternalText x) : _))
          )
        )
      ;
        (
        "UnparsedSyntheticReference"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`UnparsedSyntheticReference x) : _))
          )
        )
      ;
        (
        "Bundle"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`Bundle x) : _))
          )
        )
      ;
        (
        "UnparsedSource"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`UnparsedSource x) : _))
          )
        )
      ;
        (
        "InputFiles"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`InputFiles x) : _))
          )
        )
      ;
        (
        "JSDocTypeExpression"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JSDocTypeExpression x) : _))
          )
        )
      ;
        (
        "JSDocAllType"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JSDocAllType x) : _))
          )
        )
      ;
        (
        "JSDocUnknownType"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JSDocUnknownType x) : _))
          )
        )
      ;
        (
        "JSDocNullableType"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JSDocNullableType x) : _))
          )
        )
      ;
        (
        "JSDocNonNullableType"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JSDocNonNullableType x) : _))
          )
        )
      ;
        (
        "JSDocOptionalType"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JSDocOptionalType x) : _))
          )
        )
      ;
        (
        "JSDocFunctionType"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JSDocFunctionType x) : _))
          )
        )
      ;
        (
        "JSDocVariadicType"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JSDocVariadicType x) : _))
          )
        )
      ;
        (
        "JSDocNamepathType"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JSDocNamepathType x) : _))
          )
        )
      ;
        (
        "JSDocComment"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JSDocComment x) : _))
          )
        )
      ;
        (
        "JSDocTypeLiteral"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JSDocTypeLiteral x) : _))
          )
        )
      ;
        (
        "JSDocSignature"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JSDocSignature x) : _))
          )
        )
      ;
        (
        "JSDocTag"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JSDocTag x) : _))
          )
        )
      ;
        (
        "JSDocAugmentsTag"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JSDocAugmentsTag x) : _))
          )
        )
      ;
        (
        "JSDocImplementsTag"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JSDocImplementsTag x) : _))
          )
        )
      ;
        (
        "JSDocAuthorTag"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JSDocAuthorTag x) : _))
          )
        )
      ;
        (
        "JSDocDeprecatedTag"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JSDocDeprecatedTag x) : _))
          )
        )
      ;
        (
        "JSDocClassTag"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JSDocClassTag x) : _))
          )
        )
      ;
        (
        "JSDocPublicTag"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JSDocPublicTag x) : _))
          )
        )
      ;
        (
        "JSDocPrivateTag"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JSDocPrivateTag x) : _))
          )
        )
      ;
        (
        "JSDocProtectedTag"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JSDocProtectedTag x) : _))
          )
        )
      ;
        (
        "JSDocReadonlyTag"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JSDocReadonlyTag x) : _))
          )
        )
      ;
        (
        "JSDocCallbackTag"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JSDocCallbackTag x) : _))
          )
        )
      ;
        (
        "JSDocEnumTag"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JSDocEnumTag x) : _))
          )
        )
      ;
        (
        "JSDocParameterTag"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JSDocParameterTag x) : _))
          )
        )
      ;
        (
        "JSDocReturnTag"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JSDocReturnTag x) : _))
          )
        )
      ;
        (
        "JSDocThisTag"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JSDocThisTag x) : _))
          )
        )
      ;
        (
        "JSDocTypeTag"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JSDocTypeTag x) : _))
          )
        )
      ;
        (
        "JSDocTemplateTag"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JSDocTemplateTag x) : _))
          )
        )
      ;
        (
        "JSDocTypedefTag"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JSDocTypedefTag x) : _))
          )
        )
      ;
        (
        "JSDocPropertyTag"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`JSDocPropertyTag x) : _))
          )
        )
      ;
        (
        "SyntaxList"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`SyntaxList x) : _))
          )
        )
      ;
        (
        "NotEmittedStatement"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`NotEmittedStatement x) : _))
          )
        )
      ;
        (
        "PartiallyEmittedExpression"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`PartiallyEmittedExpression x) : _))
          )
        )
      ;
        (
        "CommaListExpression"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`CommaListExpression x) : _))
          )
        )
      ;
        (
        "MergeDeclarationMarker"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`MergeDeclarationMarker x) : _))
          )
        )
      ;
        (
        "EndOfDeclarationMarker"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`EndOfDeclarationMarker x) : _))
          )
        )
      ;
        (
        "SyntheticReferenceExpression"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`SyntheticReferenceExpression x) : _))
          )
        )
      ;
        (
        "Count"
        ,
          `Decode (
          read_node_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`Count x) : _))
          )
        )
    ]
  )
) js
and read_node_ArrayType js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          pos =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "pos"
            ) json;
          end_ =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "end"
            ) json;
          kind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "kind"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          modifiers =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "modifiers"
            ) json;
          decorators =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "decorators"
            ) json;
          resolvedSymbol =
            Atdgen_codec_runtime.Decode.decode
            (
              read_symbol
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedSymbol"
            ) json;
          resolvedType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedType"
            ) json;
          elementType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.field "elementType"
            ) json;
      } : node_ArrayType)
    )
  )
) js
and read_node_BindingElement js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          pos =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "pos"
            ) json;
          end_ =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "end"
            ) json;
          kind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "kind"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          modifiers =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "modifiers"
            ) json;
          decorators =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "decorators"
            ) json;
          resolvedSymbol =
            Atdgen_codec_runtime.Decode.decode
            (
              read_symbol
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedSymbol"
            ) json;
          resolvedType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedType"
            ) json;
          propertyName =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "propertyName"
            ) json;
          dotDotDotToken =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "dotDotDotToken"
            ) json;
          name =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.field "name"
            ) json;
          initializer_ =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "initializer"
            ) json;
      } : node_BindingElement)
    )
  )
) js
and read_node_CallSignature js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          pos =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "pos"
            ) json;
          end_ =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "end"
            ) json;
          kind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "kind"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          modifiers =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "modifiers"
            ) json;
          decorators =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "decorators"
            ) json;
          resolvedSymbol =
            Atdgen_codec_runtime.Decode.decode
            (
              read_symbol
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedSymbol"
            ) json;
          resolvedType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedType"
            ) json;
          name =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "name"
            ) json;
          typeParameters =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "typeParameters"
            ) json;
          parameters =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.field "parameters"
            ) json;
          type_ =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "type"
            ) json;
          typeArguments =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "typeArguments"
            ) json;
      } : node_CallSignature)
    )
  )
) js
and read_node_EnumDeclaration js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          pos =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "pos"
            ) json;
          end_ =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "end"
            ) json;
          kind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "kind"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          modifiers =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "modifiers"
            ) json;
          decorators =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "decorators"
            ) json;
          resolvedSymbol =
            Atdgen_codec_runtime.Decode.decode
            (
              read_symbol
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedSymbol"
            ) json;
          resolvedType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedType"
            ) json;
          name =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.field "name"
            ) json;
          members =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.field "members"
            ) json;
      } : node_EnumDeclaration)
    )
  )
) js
and read_node_EnumMember js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          pos =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "pos"
            ) json;
          end_ =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "end"
            ) json;
          kind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "kind"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          modifiers =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "modifiers"
            ) json;
          decorators =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "decorators"
            ) json;
          resolvedSymbol =
            Atdgen_codec_runtime.Decode.decode
            (
              read_symbol
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedSymbol"
            ) json;
          resolvedType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedType"
            ) json;
          name =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.field "name"
            ) json;
          initializer_ =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "initializer"
            ) json;
      } : node_EnumMember)
    )
  )
) js
and read_node_FunctionDeclaration js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          pos =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "pos"
            ) json;
          end_ =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "end"
            ) json;
          kind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "kind"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          modifiers =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "modifiers"
            ) json;
          decorators =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "decorators"
            ) json;
          resolvedSymbol =
            Atdgen_codec_runtime.Decode.decode
            (
              read_symbol
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedSymbol"
            ) json;
          resolvedType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedType"
            ) json;
          typeParameters =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "typeParameters"
            ) json;
          parameters =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.field "parameters"
            ) json;
          type_ =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "type"
            ) json;
          typeArguments =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "typeArguments"
            ) json;
          asteriskToken =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "asteriskToken"
            ) json;
          questionToken =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "questionToken"
            ) json;
          exclamationToken =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "exclamationToken"
            ) json;
          name =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "name"
            ) json;
          body =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "body"
            ) json;
      } : node_FunctionDeclaration)
    )
  )
) js
and read_node_FunctionType js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          pos =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "pos"
            ) json;
          end_ =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "end"
            ) json;
          kind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "kind"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          modifiers =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "modifiers"
            ) json;
          decorators =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "decorators"
            ) json;
          resolvedSymbol =
            Atdgen_codec_runtime.Decode.decode
            (
              read_symbol
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedSymbol"
            ) json;
          resolvedType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedType"
            ) json;
          name =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "name"
            ) json;
          typeParameters =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "typeParameters"
            ) json;
          parameters =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.field "parameters"
            ) json;
          type_ =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "type"
            ) json;
          typeArguments =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "typeArguments"
            ) json;
      } : node_FunctionType)
    )
  )
) js
and read_node_Generic js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          pos =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "pos"
            ) json;
          end_ =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "end"
            ) json;
          kind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "kind"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          modifiers =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "modifiers"
            ) json;
          decorators =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "decorators"
            ) json;
          resolvedSymbol =
            Atdgen_codec_runtime.Decode.decode
            (
              read_symbol
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedSymbol"
            ) json;
          resolvedType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedType"
            ) json;
      } : node_Generic)
    )
  )
) js
and read_node_HeritageClause js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          pos =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "pos"
            ) json;
          end_ =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "end"
            ) json;
          kind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "kind"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          modifiers =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "modifiers"
            ) json;
          decorators =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "decorators"
            ) json;
          resolvedSymbol =
            Atdgen_codec_runtime.Decode.decode
            (
              read_symbol
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedSymbol"
            ) json;
          resolvedType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedType"
            ) json;
          token =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "token"
            ) json;
          types =
            Atdgen_codec_runtime.Decode.decode
            (
              read__2
              |> Atdgen_codec_runtime.Decode.field "types"
            ) json;
      } : node_HeritageClause)
    )
  )
) js
and read_node_Identifier js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          pos =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "pos"
            ) json;
          end_ =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "end"
            ) json;
          kind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "kind"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          modifiers =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "modifiers"
            ) json;
          decorators =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "decorators"
            ) json;
          resolvedSymbol =
            Atdgen_codec_runtime.Decode.decode
            (
              read_symbol
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedSymbol"
            ) json;
          resolvedType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedType"
            ) json;
          escapedText =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "escapedText"
            ) json;
          originalKeywordKind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.fieldOptional "originalKeywordKind"
            ) json;
      } : node_Identifier)
    )
  )
) js
and read_node_IndexSignature js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          pos =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "pos"
            ) json;
          end_ =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "end"
            ) json;
          kind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "kind"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          modifiers =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "modifiers"
            ) json;
          decorators =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "decorators"
            ) json;
          resolvedSymbol =
            Atdgen_codec_runtime.Decode.decode
            (
              read_symbol
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedSymbol"
            ) json;
          resolvedType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedType"
            ) json;
          typeParameters =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "typeParameters"
            ) json;
          parameters =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.field "parameters"
            ) json;
          type_ =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "type"
            ) json;
          typeArguments =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "typeArguments"
            ) json;
          name =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "name"
            ) json;
      } : node_IndexSignature)
    )
  )
) js
and read_node_InterfaceDeclaration js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          pos =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "pos"
            ) json;
          end_ =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "end"
            ) json;
          kind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "kind"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          modifiers =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "modifiers"
            ) json;
          decorators =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "decorators"
            ) json;
          resolvedSymbol =
            Atdgen_codec_runtime.Decode.decode
            (
              read_symbol
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedSymbol"
            ) json;
          resolvedType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedType"
            ) json;
          name =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.field "name"
            ) json;
          typeParameters =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "typeParameters"
            ) json;
          heritageClauses =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "heritageClauses"
            ) json;
          members =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.field "members"
            ) json;
      } : node_InterfaceDeclaration)
    )
  )
) js
and read_node_MethodSignature js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          pos =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "pos"
            ) json;
          end_ =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "end"
            ) json;
          kind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "kind"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          modifiers =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "modifiers"
            ) json;
          decorators =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "decorators"
            ) json;
          resolvedSymbol =
            Atdgen_codec_runtime.Decode.decode
            (
              read_symbol
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedSymbol"
            ) json;
          resolvedType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedType"
            ) json;
          typeParameters =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "typeParameters"
            ) json;
          parameters =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.field "parameters"
            ) json;
          type_ =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "type"
            ) json;
          typeArguments =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "typeArguments"
            ) json;
          name =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "name"
            ) json;
      } : node_MethodSignature)
    )
  )
) js
and read_node_Parameter js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          pos =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "pos"
            ) json;
          end_ =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "end"
            ) json;
          kind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "kind"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          modifiers =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "modifiers"
            ) json;
          decorators =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "decorators"
            ) json;
          resolvedSymbol =
            Atdgen_codec_runtime.Decode.decode
            (
              read_symbol
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedSymbol"
            ) json;
          resolvedType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedType"
            ) json;
          dotDotDotToken =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "dotDotDotToken"
            ) json;
          name =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.field "name"
            ) json;
          questionToken =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "questionToken"
            ) json;
          type_ =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "type"
            ) json;
          initializer_ =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "initializer"
            ) json;
      } : node_Parameter)
    )
  )
) js
and read_node_PropertyDeclaration js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          pos =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "pos"
            ) json;
          end_ =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "end"
            ) json;
          kind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "kind"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          modifiers =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "modifiers"
            ) json;
          decorators =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "decorators"
            ) json;
          resolvedSymbol =
            Atdgen_codec_runtime.Decode.decode
            (
              read_symbol
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedSymbol"
            ) json;
          resolvedType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedType"
            ) json;
          name =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.field "name"
            ) json;
          questionToken =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "questionToken"
            ) json;
          exclamationToken =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "exclamationToken"
            ) json;
          type_ =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "type"
            ) json;
          initializer_ =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "initializer"
            ) json;
      } : node_PropertyDeclaration)
    )
  )
) js
and read_node_PropertySignature js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          pos =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "pos"
            ) json;
          end_ =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "end"
            ) json;
          kind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "kind"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          modifiers =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "modifiers"
            ) json;
          decorators =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "decorators"
            ) json;
          resolvedSymbol =
            Atdgen_codec_runtime.Decode.decode
            (
              read_symbol
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedSymbol"
            ) json;
          resolvedType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedType"
            ) json;
          name =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.field "name"
            ) json;
          questionToken =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "questionToken"
            ) json;
          type_ =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "type"
            ) json;
          initializer_ =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "initializer"
            ) json;
      } : node_PropertySignature)
    )
  )
) js
and read_node_QualifiedName js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          pos =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "pos"
            ) json;
          end_ =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "end"
            ) json;
          kind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "kind"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          modifiers =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "modifiers"
            ) json;
          decorators =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "decorators"
            ) json;
          resolvedSymbol =
            Atdgen_codec_runtime.Decode.decode
            (
              read_symbol
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedSymbol"
            ) json;
          resolvedType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedType"
            ) json;
          left =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.field "left"
            ) json;
          right =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.field "right"
            ) json;
      } : node_QualifiedName)
    )
  )
) js
and read_node_SourceFile js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          pos =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "pos"
            ) json;
          end_ =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "end"
            ) json;
          kind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "kind"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          modifiers =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "modifiers"
            ) json;
          decorators =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "decorators"
            ) json;
          resolvedSymbol =
            Atdgen_codec_runtime.Decode.decode
            (
              read_symbol
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedSymbol"
            ) json;
          resolvedType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedType"
            ) json;
          statements =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.field "statements"
            ) json;
          text =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "text"
            ) json;
          languageVersion =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "languageVersion"
            ) json;
          fileName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "fileName"
            ) json;
          languageVariant =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "languageVariant"
            ) json;
          isDeclarationFile =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.bool
              |> Atdgen_codec_runtime.Decode.field "isDeclarationFile"
            ) json;
          nodeCount =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "nodeCount"
            ) json;
          identifierCount =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "identifierCount"
            ) json;
      } : node_SourceFile)
    )
  )
) js
and read_node_TypeAliasDeclaration js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          pos =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "pos"
            ) json;
          end_ =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "end"
            ) json;
          kind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "kind"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          modifiers =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "modifiers"
            ) json;
          decorators =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "decorators"
            ) json;
          resolvedSymbol =
            Atdgen_codec_runtime.Decode.decode
            (
              read_symbol
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedSymbol"
            ) json;
          resolvedType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedType"
            ) json;
          name =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.field "name"
            ) json;
          typeParameters =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "typeParameters"
            ) json;
          type_ =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.field "type"
            ) json;
      } : node_TypeAliasDeclaration)
    )
  )
) js
and read_node_TypeLiteral js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          pos =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "pos"
            ) json;
          end_ =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "end"
            ) json;
          kind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "kind"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          modifiers =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "modifiers"
            ) json;
          decorators =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "decorators"
            ) json;
          resolvedSymbol =
            Atdgen_codec_runtime.Decode.decode
            (
              read_symbol
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedSymbol"
            ) json;
          resolvedType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedType"
            ) json;
          members =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.field "members"
            ) json;
      } : node_TypeLiteral)
    )
  )
) js
and read_node_TypeOperator js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          pos =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "pos"
            ) json;
          end_ =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "end"
            ) json;
          kind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "kind"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          modifiers =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "modifiers"
            ) json;
          decorators =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "decorators"
            ) json;
          resolvedSymbol =
            Atdgen_codec_runtime.Decode.decode
            (
              read_symbol
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedSymbol"
            ) json;
          resolvedType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedType"
            ) json;
          operator =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "operator"
            ) json;
          type_ =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.field "type"
            ) json;
      } : node_TypeOperator)
    )
  )
) js
and read_node_TypeParameter js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          pos =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "pos"
            ) json;
          end_ =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "end"
            ) json;
          kind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "kind"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          modifiers =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "modifiers"
            ) json;
          decorators =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "decorators"
            ) json;
          resolvedSymbol =
            Atdgen_codec_runtime.Decode.decode
            (
              read_symbol
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedSymbol"
            ) json;
          resolvedType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedType"
            ) json;
          name =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.field "name"
            ) json;
          constraint_ =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "constraint"
            ) json;
          default =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "default"
            ) json;
          expression =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "expression"
            ) json;
      } : node_TypeParameter)
    )
  )
) js
and read_node_TypeReference js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          pos =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "pos"
            ) json;
          end_ =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "end"
            ) json;
          kind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "kind"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          modifiers =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "modifiers"
            ) json;
          decorators =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "decorators"
            ) json;
          resolvedSymbol =
            Atdgen_codec_runtime.Decode.decode
            (
              read_symbol
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedSymbol"
            ) json;
          resolvedType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedType"
            ) json;
          typeArguments =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "typeArguments"
            ) json;
          typeName =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.field "typeName"
            ) json;
      } : node_TypeReference)
    )
  )
) js
and read_node_UnionType js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          pos =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "pos"
            ) json;
          end_ =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "end"
            ) json;
          kind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "kind"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          modifiers =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "modifiers"
            ) json;
          decorators =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "decorators"
            ) json;
          resolvedSymbol =
            Atdgen_codec_runtime.Decode.decode
            (
              read_symbol
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedSymbol"
            ) json;
          resolvedType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedType"
            ) json;
          types =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.field "types"
            ) json;
      } : node_UnionType)
    )
  )
) js
and read_node_VariableDeclaration js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          pos =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "pos"
            ) json;
          end_ =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "end"
            ) json;
          kind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "kind"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          modifiers =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "modifiers"
            ) json;
          decorators =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "decorators"
            ) json;
          resolvedSymbol =
            Atdgen_codec_runtime.Decode.decode
            (
              read_symbol
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedSymbol"
            ) json;
          resolvedType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedType"
            ) json;
          name =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.field "name"
            ) json;
          exclamationToken =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "exclamationToken"
            ) json;
          type_ =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "type"
            ) json;
          initializer_ =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "initializer"
            ) json;
      } : node_VariableDeclaration)
    )
  )
) js
and read_node_VariableDeclarationList js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          pos =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "pos"
            ) json;
          end_ =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "end"
            ) json;
          kind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "kind"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          modifiers =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "modifiers"
            ) json;
          decorators =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "decorators"
            ) json;
          resolvedSymbol =
            Atdgen_codec_runtime.Decode.decode
            (
              read_symbol
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedSymbol"
            ) json;
          resolvedType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedType"
            ) json;
          declarations =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.field "declarations"
            ) json;
      } : node_VariableDeclarationList)
    )
  )
) js
and read_symbol js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          id =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.fieldOptional "id"
            ) json;
          resolvedType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedType"
            ) json;
          fullyQualifiedName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "fullyQualifiedName"
            ) json;
          name =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "name"
            ) json;
      } : symbol)
    )
  )
) js
and read_type_ js = (
  Atdgen_codec_runtime.Decode.adapter Typescript_json_adapters.KindName.normalize (
    Atdgen_codec_runtime.Decode.enum
    [
        (
        "ObjectKeyword"
        ,
          `Decode (
          read_type_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ObjectKeyword x) : _))
          )
        )
      ;
        (
        "Void"
        ,
          `Decode (
          read_type_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`Void x) : _))
          )
        )
      ;
        (
        "Any"
        ,
          `Decode (
          read_type_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`Any x) : _))
          )
        )
      ;
        (
        "Array"
        ,
          `Decode (
          read_type_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`Array x) : _))
          )
        )
      ;
        (
        "Boolean"
        ,
          `Decode (
          read_type_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`Boolean x) : _))
          )
        )
      ;
        (
        "String"
        ,
          `Decode (
          read_type_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`String x) : _))
          )
        )
      ;
        (
        "Number"
        ,
          `Decode (
          read_type_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`Number x) : _))
          )
        )
      ;
        (
        "Literal"
        ,
          `Decode (
          read_type_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`Literal x) : _))
          )
        )
      ;
        (
        "BooleanLiteral"
        ,
          `Decode (
          read_type_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`BooleanLiteral x) : _))
          )
        )
      ;
        (
        "Union"
        ,
          `Decode (
          read_type_UnionType
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`Union x) : _))
          )
        )
      ;
        (
        "EnumLiteral"
        ,
          `Decode (
          read_type_StringLiteralType
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`EnumLiteral x) : _))
          )
        )
      ;
        (
        "NumberLiteral"
        ,
          `Decode (
          read_type_NumberLiteralType
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`NumberLiteral x) : _))
          )
        )
      ;
        (
        "StringLiteral"
        ,
          `Decode (
          read_type_StringLiteralType
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`StringLiteral x) : _))
          )
        )
      ;
        (
        "Class"
        ,
          `Decode (
          read_type_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`Class x) : _))
          )
        )
      ;
        (
        "ClassOrInterface"
        ,
          `Decode (
          read_type_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ClassOrInterface x) : _))
          )
        )
      ;
        (
        "Interface"
        ,
          `Decode (
          read_type_InterfaceType
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`Interface x) : _))
          )
        )
      ;
        (
        "Object"
        ,
          `Decode (
          read_type_ObjectType
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`Object x) : _))
          )
        )
      ;
        (
        "Enum"
        ,
          `Decode (
          read_type_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`Enum x) : _))
          )
        )
      ;
        (
        "TypeParameter"
        ,
          `Decode (
          read_type_TypeParameter
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`TypeParameter x) : _))
          )
        )
      ;
        (
        "Tuple"
        ,
          `Decode (
          read_type_TupleType
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`Tuple x) : _))
          )
        )
      ;
        (
        "Intersection"
        ,
          `Decode (
          read_type_IntersectionType
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`Intersection x) : _))
          )
        )
      ;
        (
        "UnionOrIntersection"
        ,
          `Decode (
          read_type_UnionOrIntersectionType
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`UnionOrIntersection x) : _))
          )
        )
      ;
        (
        "Unknown"
        ,
          `Decode (
          read_type_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`Unknown x) : _))
          )
        )
      ;
        (
        "Null"
        ,
          `Decode (
          read_type_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`Null x) : _))
          )
        )
      ;
        (
        "Undefined"
        ,
          `Decode (
          read_type_Generic
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`Undefined x) : _))
          )
        )
      ;
        (
        "TypeReference"
        ,
          `Decode (
          read_type_TypeReference
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`TypeReference x) : _))
          )
        )
      ;
        (
        "Anonymous"
        ,
          `Decode (
          read_type_AnonymousType
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`Anonymous x) : _))
          )
        )
      ;
        (
        "Unidentified"
        ,
          `Decode (
          read_type_Unidentified
          |> Atdgen_codec_runtime.Decode.map (fun x -> ((`Unidentified x) : _))
          )
        )
    ]
  )
) js
and read_type_AnonymousType js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          id =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.fieldOptional "id"
            ) json;
          pattern =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "pattern"
            ) json;
          aliasTypeArguments =
            Atdgen_codec_runtime.Decode.decode
            (
              read__7
              |> Atdgen_codec_runtime.Decode.fieldOptional "aliasTypeArguments"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          objectFlags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "objectFlags"
            ) json;
          target =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "target"
            ) json;
      } : type_AnonymousType)
    )
  )
) js
and read_type_Generic js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          id =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.fieldOptional "id"
            ) json;
          pattern =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "pattern"
            ) json;
          aliasTypeArguments =
            Atdgen_codec_runtime.Decode.decode
            (
              read__7
              |> Atdgen_codec_runtime.Decode.fieldOptional "aliasTypeArguments"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
      } : type_Generic)
    )
  )
) js
and read_type_InterfaceType js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          id =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.fieldOptional "id"
            ) json;
          pattern =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "pattern"
            ) json;
          aliasTypeArguments =
            Atdgen_codec_runtime.Decode.decode
            (
              read__7
              |> Atdgen_codec_runtime.Decode.fieldOptional "aliasTypeArguments"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          objectFlags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "objectFlags"
            ) json;
          typeParameters =
            Atdgen_codec_runtime.Decode.decode
            (
              read__7
              |> Atdgen_codec_runtime.Decode.fieldOptional "typeParameters"
            ) json;
          outerTypeParameters =
            Atdgen_codec_runtime.Decode.decode
            (
              read__7
              |> Atdgen_codec_runtime.Decode.fieldOptional "outerTypeParameters"
            ) json;
          localTypeParameters =
            Atdgen_codec_runtime.Decode.decode
            (
              read__7
              |> Atdgen_codec_runtime.Decode.fieldOptional "localTypeParameters"
            ) json;
          thisType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "thisType"
            ) json;
      } : type_InterfaceType)
    )
  )
) js
and read_type_IntersectionType js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          id =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.fieldOptional "id"
            ) json;
          pattern =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "pattern"
            ) json;
          aliasTypeArguments =
            Atdgen_codec_runtime.Decode.decode
            (
              read__7
              |> Atdgen_codec_runtime.Decode.fieldOptional "aliasTypeArguments"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          types =
            Atdgen_codec_runtime.Decode.decode
            (
              read__7
              |> Atdgen_codec_runtime.Decode.field "types"
            ) json;
      } : type_IntersectionType)
    )
  )
) js
and read_type_NumberLiteralType js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          id =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.fieldOptional "id"
            ) json;
          pattern =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "pattern"
            ) json;
          aliasTypeArguments =
            Atdgen_codec_runtime.Decode.decode
            (
              read__7
              |> Atdgen_codec_runtime.Decode.fieldOptional "aliasTypeArguments"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          value =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.float
              |> Atdgen_codec_runtime.Decode.field "value"
            ) json;
      } : type_NumberLiteralType)
    )
  )
) js
and read_type_ObjectType js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          id =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.fieldOptional "id"
            ) json;
          pattern =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "pattern"
            ) json;
          aliasTypeArguments =
            Atdgen_codec_runtime.Decode.decode
            (
              read__7
              |> Atdgen_codec_runtime.Decode.fieldOptional "aliasTypeArguments"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          objectFlags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "objectFlags"
            ) json;
      } : type_ObjectType)
    )
  )
) js
and read_type_StringLiteralType js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          id =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.fieldOptional "id"
            ) json;
          pattern =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "pattern"
            ) json;
          aliasTypeArguments =
            Atdgen_codec_runtime.Decode.decode
            (
              read__7
              |> Atdgen_codec_runtime.Decode.fieldOptional "aliasTypeArguments"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          value =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "value"
            ) json;
      } : type_StringLiteralType)
    )
  )
) js
and read_type_TupleType js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          id =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.fieldOptional "id"
            ) json;
          pattern =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "pattern"
            ) json;
          aliasTypeArguments =
            Atdgen_codec_runtime.Decode.decode
            (
              read__7
              |> Atdgen_codec_runtime.Decode.fieldOptional "aliasTypeArguments"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          minLength =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "minLength"
            ) json;
          fixedLength =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "fixedLength"
            ) json;
          hasRestElement =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.bool
              |> Atdgen_codec_runtime.Decode.field "hasRestElement"
            ) json;
          readonly =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.bool
              |> Atdgen_codec_runtime.Decode.field "readonly"
            ) json;
          elementFlags =
            Atdgen_codec_runtime.Decode.decode
            (
              read__9
              |> Atdgen_codec_runtime.Decode.field "elementFlags"
            ) json;
          combinedFlags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "combinedFlags"
            ) json;
          labeledElementDeclarations =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "labeledElementDeclarations"
            ) json;
      } : type_TupleType)
    )
  )
) js
and read_type_TypeParameter js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          id =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.fieldOptional "id"
            ) json;
          pattern =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "pattern"
            ) json;
          aliasTypeArguments =
            Atdgen_codec_runtime.Decode.decode
            (
              read__7
              |> Atdgen_codec_runtime.Decode.fieldOptional "aliasTypeArguments"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          constraint_ =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "constraint"
            ) json;
      } : type_TypeParameter)
    )
  )
) js
and read_type_TypeReference js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          id =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.fieldOptional "id"
            ) json;
          pattern =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "pattern"
            ) json;
          aliasTypeArguments =
            Atdgen_codec_runtime.Decode.decode
            (
              read__7
              |> Atdgen_codec_runtime.Decode.fieldOptional "aliasTypeArguments"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          target =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "target"
            ) json;
      } : type_TypeReference)
    )
  )
) js
and read_type_Unidentified js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          id =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.fieldOptional "id"
            ) json;
          pattern =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "pattern"
            ) json;
          aliasTypeArguments =
            Atdgen_codec_runtime.Decode.decode
            (
              read__7
              |> Atdgen_codec_runtime.Decode.fieldOptional "aliasTypeArguments"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          nodeKind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.fieldOptional "nodeKind"
            ) json;
      } : type_Unidentified)
    )
  )
) js
and read_type_UnionOrIntersectionType js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          id =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.fieldOptional "id"
            ) json;
          pattern =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "pattern"
            ) json;
          aliasTypeArguments =
            Atdgen_codec_runtime.Decode.decode
            (
              read__7
              |> Atdgen_codec_runtime.Decode.fieldOptional "aliasTypeArguments"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          types =
            Atdgen_codec_runtime.Decode.decode
            (
              read__7
              |> Atdgen_codec_runtime.Decode.field "types"
            ) json;
      } : type_UnionOrIntersectionType)
    )
  )
) js
and read_type_UnionType js = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          id =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.fieldOptional "id"
            ) json;
          pattern =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "pattern"
            ) json;
          aliasTypeArguments =
            Atdgen_codec_runtime.Decode.decode
            (
              read__7
              |> Atdgen_codec_runtime.Decode.fieldOptional "aliasTypeArguments"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          types =
            Atdgen_codec_runtime.Decode.decode
            (
              read__7
              |> Atdgen_codec_runtime.Decode.field "types"
            ) json;
      } : type_UnionType)
    )
  )
) js
let write__10 = (
  Atdgen_codec_runtime.Encode.option_as_constr (
    Atdgen_codec_runtime.Encode.tuple2
      (
        Atdgen_codec_runtime.Encode.string
      )
      (
        write_type_
      )
  )
)
let read__10 = (
  Atdgen_codec_runtime.Decode.option_as_constr (
    Atdgen_codec_runtime.Decode.tuple2
      (
        Atdgen_codec_runtime.Decode.string
      )
      (
        read_type_
      )
  )
)
let write_conditionalRoot = (
  Atdgen_codec_runtime.Encode.make (fun (t : conditionalRoot) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            write_node
            )
          ~name:"node"
          t.node
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write_type_
            )
          ~name:"checkType"
          t.checkType
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write_type_
            )
          ~name:"extendsType"
          t.extendsType
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write_type_
            )
          ~name:"trueType"
          t.trueType
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write_type_
            )
          ~name:"falseType"
          t.falseType
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.bool
            )
          ~name:"isDistributive"
          t.isDistributive
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__7
            )
          ~name:"inferTypeParameters"
          t.inferTypeParameters
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__7
            )
          ~name:"outerTypeParameters"
          t.outerTypeParameters
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            Atdgen_codec_runtime.Encode.tuple2
              (
                Atdgen_codec_runtime.Encode.string
              )
              (
                write_type_
              )
            )
          ~name:"instantiations"
          t.instantiations
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_symbol
            )
          ~name:"aliasSymbol"
          t.aliasSymbol
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__7
            )
          ~name:"aliasTypeArguments"
          t.aliasTypeArguments
      ]
    )
  )
)
let read_conditionalRoot = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          node =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.field "node"
            ) json;
          checkType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.field "checkType"
            ) json;
          extendsType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.field "extendsType"
            ) json;
          trueType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.field "trueType"
            ) json;
          falseType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.field "falseType"
            ) json;
          isDistributive =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.bool
              |> Atdgen_codec_runtime.Decode.field "isDistributive"
            ) json;
          inferTypeParameters =
            Atdgen_codec_runtime.Decode.decode
            (
              read__7
              |> Atdgen_codec_runtime.Decode.fieldOptional "inferTypeParameters"
            ) json;
          outerTypeParameters =
            Atdgen_codec_runtime.Decode.decode
            (
              read__7
              |> Atdgen_codec_runtime.Decode.fieldOptional "outerTypeParameters"
            ) json;
          instantiations =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.tuple2
                (
                  Atdgen_codec_runtime.Decode.string
                )
                (
                  read_type_
                )
              |> Atdgen_codec_runtime.Decode.fieldOptional "instantiations"
            ) json;
          aliasSymbol =
            Atdgen_codec_runtime.Decode.decode
            (
              read_symbol
              |> Atdgen_codec_runtime.Decode.fieldOptional "aliasSymbol"
            ) json;
          aliasTypeArguments =
            Atdgen_codec_runtime.Decode.decode
            (
              read__7
              |> Atdgen_codec_runtime.Decode.fieldOptional "aliasTypeArguments"
            ) json;
      } : conditionalRoot)
    )
  )
)
let write_type_conditionalType = (
  Atdgen_codec_runtime.Encode.make (fun (t : type_conditionalType) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            write_conditionalRoot
            )
          ~name:"root"
          t.root
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write_type_
            )
          ~name:"checkType"
          t.checkType
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write_type_
            )
          ~name:"extendsType"
          t.extendsType
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write_type_
            )
          ~name:"resolvedTrueType"
          t.resolvedTrueType
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write_type_
            )
          ~name:"resolvedFalseType"
          t.resolvedFalseType
      ]
    )
  )
)
let read_type_conditionalType = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          root =
            Atdgen_codec_runtime.Decode.decode
            (
              read_conditionalRoot
              |> Atdgen_codec_runtime.Decode.field "root"
            ) json;
          checkType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.field "checkType"
            ) json;
          extendsType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.field "extendsType"
            ) json;
          resolvedTrueType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.field "resolvedTrueType"
            ) json;
          resolvedFalseType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.field "resolvedFalseType"
            ) json;
      } : type_conditionalType)
    )
  )
)
let write_type_LiteralType = (
  Atdgen_codec_runtime.Encode.make (fun (t : type_LiteralType) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"id"
          t.id
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"pattern"
          t.pattern
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__7
            )
          ~name:"aliasTypeArguments"
          t.aliasTypeArguments
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
      ]
    )
  )
)
let read_type_LiteralType = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          id =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.fieldOptional "id"
            ) json;
          pattern =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "pattern"
            ) json;
          aliasTypeArguments =
            Atdgen_codec_runtime.Decode.decode
            (
              read__7
              |> Atdgen_codec_runtime.Decode.fieldOptional "aliasTypeArguments"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
      } : type_LiteralType)
    )
  )
)
let write_readonlyTextRange = (
  Atdgen_codec_runtime.Encode.make (fun (t : readonlyTextRange) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"pos"
          t.pos
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"end"
          t.end_
      ]
    )
  )
)
let read_readonlyTextRange = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          pos =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "pos"
            ) json;
          end_ =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "end"
            ) json;
      } : readonlyTextRange)
    )
  )
)
let write_node_TypeElement = (
  Atdgen_codec_runtime.Encode.make (fun (t : node_TypeElement) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"pos"
          t.pos
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"end"
          t.end_
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"kind"
          t.kind
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"modifiers"
          t.modifiers
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"decorators"
          t.decorators
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_symbol
            )
          ~name:"resolvedSymbol"
          t.resolvedSymbol
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"resolvedType"
          t.resolvedType
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"questionToken"
          t.questionToken
      ]
    )
  )
)
let read_node_TypeElement = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          pos =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "pos"
            ) json;
          end_ =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "end"
            ) json;
          kind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "kind"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          modifiers =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "modifiers"
            ) json;
          decorators =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "decorators"
            ) json;
          resolvedSymbol =
            Atdgen_codec_runtime.Decode.decode
            (
              read_symbol
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedSymbol"
            ) json;
          resolvedType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedType"
            ) json;
          questionToken =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "questionToken"
            ) json;
      } : node_TypeElement)
    )
  )
)
let write_node_SignatureDeclarationBase = (
  Atdgen_codec_runtime.Encode.make (fun (t : node_SignatureDeclarationBase) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"pos"
          t.pos
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"end"
          t.end_
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"kind"
          t.kind
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"modifiers"
          t.modifiers
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"decorators"
          t.decorators
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_symbol
            )
          ~name:"resolvedSymbol"
          t.resolvedSymbol
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"resolvedType"
          t.resolvedType
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"name"
          t.name
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"typeParameters"
          t.typeParameters
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write__1
            )
          ~name:"parameters"
          t.parameters
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"type"
          t.type_
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"typeArguments"
          t.typeArguments
      ]
    )
  )
)
let read_node_SignatureDeclarationBase = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          pos =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "pos"
            ) json;
          end_ =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "end"
            ) json;
          kind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "kind"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          modifiers =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "modifiers"
            ) json;
          decorators =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "decorators"
            ) json;
          resolvedSymbol =
            Atdgen_codec_runtime.Decode.decode
            (
              read_symbol
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedSymbol"
            ) json;
          resolvedType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedType"
            ) json;
          name =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "name"
            ) json;
          typeParameters =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "typeParameters"
            ) json;
          parameters =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.field "parameters"
            ) json;
          type_ =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "type"
            ) json;
          typeArguments =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "typeArguments"
            ) json;
      } : node_SignatureDeclarationBase)
    )
  )
)
let write_node_NodeWithTypeArguments = (
  Atdgen_codec_runtime.Encode.make (fun (t : node_NodeWithTypeArguments) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"pos"
          t.pos
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"end"
          t.end_
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"kind"
          t.kind
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"modifiers"
          t.modifiers
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"decorators"
          t.decorators
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_symbol
            )
          ~name:"resolvedSymbol"
          t.resolvedSymbol
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"resolvedType"
          t.resolvedType
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"typeArguments"
          t.typeArguments
      ]
    )
  )
)
let read_node_NodeWithTypeArguments = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          pos =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "pos"
            ) json;
          end_ =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "end"
            ) json;
          kind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "kind"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          modifiers =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "modifiers"
            ) json;
          decorators =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "decorators"
            ) json;
          resolvedSymbol =
            Atdgen_codec_runtime.Decode.decode
            (
              read_symbol
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedSymbol"
            ) json;
          resolvedType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedType"
            ) json;
          typeArguments =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "typeArguments"
            ) json;
      } : node_NodeWithTypeArguments)
    )
  )
)
let write_node_NamedDeclaration = (
  Atdgen_codec_runtime.Encode.make (fun (t : node_NamedDeclaration) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"pos"
          t.pos
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"end"
          t.end_
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"kind"
          t.kind
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"modifiers"
          t.modifiers
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"decorators"
          t.decorators
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_symbol
            )
          ~name:"resolvedSymbol"
          t.resolvedSymbol
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"resolvedType"
          t.resolvedType
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"name"
          t.name
      ]
    )
  )
)
let read_node_NamedDeclaration = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          pos =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "pos"
            ) json;
          end_ =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "end"
            ) json;
          kind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "kind"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          modifiers =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "modifiers"
            ) json;
          decorators =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "decorators"
            ) json;
          resolvedSymbol =
            Atdgen_codec_runtime.Decode.decode
            (
              read_symbol
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedSymbol"
            ) json;
          resolvedType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedType"
            ) json;
          name =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "name"
            ) json;
      } : node_NamedDeclaration)
    )
  )
)
let write_node_FunctionLikeDeclarationBase = (
  Atdgen_codec_runtime.Encode.make (fun (t : node_FunctionLikeDeclarationBase) ->
    (
    Atdgen_codec_runtime.Encode.obj
      [
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"pos"
          t.pos
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"end"
          t.end_
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"kind"
          t.kind
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.string
            )
          ~name:"kindName"
          t.kindName
        ;
          Atdgen_codec_runtime.Encode.field
            (
            Atdgen_codec_runtime.Encode.int
            )
          ~name:"flags"
          t.flags
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"modifiers"
          t.modifiers
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"decorators"
          t.decorators
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_symbol
            )
          ~name:"resolvedSymbol"
          t.resolvedSymbol
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_type_
            )
          ~name:"resolvedType"
          t.resolvedType
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"name"
          t.name
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"typeParameters"
          t.typeParameters
        ;
          Atdgen_codec_runtime.Encode.field
            (
            write__1
            )
          ~name:"parameters"
          t.parameters
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"type"
          t.type_
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write__1
            )
          ~name:"typeArguments"
          t.typeArguments
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"asteriskToken"
          t.asteriskToken
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"questionToken"
          t.questionToken
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"exclamationToken"
          t.exclamationToken
        ;
          Atdgen_codec_runtime.Encode.field_o
            (
            write_node
            )
          ~name:"body"
          t.body
      ]
    )
  )
)
let read_node_FunctionLikeDeclarationBase = (
  Atdgen_codec_runtime.Decode.make (fun json ->
    (
      ({
          pos =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "pos"
            ) json;
          end_ =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "end"
            ) json;
          kind =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "kind"
            ) json;
          kindName =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.string
              |> Atdgen_codec_runtime.Decode.field "kindName"
            ) json;
          flags =
            Atdgen_codec_runtime.Decode.decode
            (
              Atdgen_codec_runtime.Decode.int
              |> Atdgen_codec_runtime.Decode.field "flags"
            ) json;
          modifiers =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "modifiers"
            ) json;
          decorators =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "decorators"
            ) json;
          resolvedSymbol =
            Atdgen_codec_runtime.Decode.decode
            (
              read_symbol
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedSymbol"
            ) json;
          resolvedType =
            Atdgen_codec_runtime.Decode.decode
            (
              read_type_
              |> Atdgen_codec_runtime.Decode.fieldOptional "resolvedType"
            ) json;
          name =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "name"
            ) json;
          typeParameters =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "typeParameters"
            ) json;
          parameters =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.field "parameters"
            ) json;
          type_ =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "type"
            ) json;
          typeArguments =
            Atdgen_codec_runtime.Decode.decode
            (
              read__1
              |> Atdgen_codec_runtime.Decode.fieldOptional "typeArguments"
            ) json;
          asteriskToken =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "asteriskToken"
            ) json;
          questionToken =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "questionToken"
            ) json;
          exclamationToken =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "exclamationToken"
            ) json;
          body =
            Atdgen_codec_runtime.Decode.decode
            (
              read_node
              |> Atdgen_codec_runtime.Decode.fieldOptional "body"
            ) json;
      } : node_FunctionLikeDeclarationBase)
    )
  )
)

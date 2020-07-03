(* Auto-generated from "typescript.atd" *)
              [@@@ocaml.warning "-27-32-35-39"]

type node = [
    `SourceFile of node_SourceFile
  | `TypeAliasDeclaration of node_TypeAliasDeclaration
  | `TypeParameter of node_TypeParameter
  | `InterfaceDeclaration of node_InterfaceDeclaration
  | `HeritageClause of node_HeritageClause
  | `Identifier of node_Identifier
  | `PrivateIdentifier of node_Generic
  | `StringLiteral of node_Generic
  | `NumericLiteral of node_Generic
  | `Unknown of node_Generic
  | `EndOfFileToken of node_Generic
  | `SingleLineCommentTrivia of node_Generic
  | `MultiLineCommentTrivia of node_Generic
  | `NewLineTrivia of node_Generic
  | `WhitespaceTrivia of node_Generic
  | `ShebangTrivia of node_Generic
  | `ConflictMarkerTrivia of node_Generic
  | `BigIntLiteral of node_Generic
  | `JsxText of node_Generic
  | `JsxTextAllWhiteSpaces of node_Generic
  | `RegularExpressionLiteral of node_Generic
  | `NoSubstitutionTemplateLiteral of node_Generic
  | `TemplateHead of node_Generic
  | `TemplateMiddle of node_Generic
  | `TemplateTail of node_Generic
  | `OpenBraceToken of node_Generic
  | `CloseBraceToken of node_Generic
  | `OpenParenToken of node_Generic
  | `CloseParenToken of node_Generic
  | `OpenBracketToken of node_Generic
  | `CloseBracketToken of node_Generic
  | `DotToken of node_Generic
  | `DotDotDotToken of node_Generic
  | `SemicolonToken of node_Generic
  | `CommaToken of node_Generic
  | `QuestionDotToken of node_Generic
  | `LessThanToken of node_Generic
  | `LessThanSlashToken of node_Generic
  | `GreaterThanToken of node_Generic
  | `LessThanEqualsToken of node_Generic
  | `GreaterThanEqualsToken of node_Generic
  | `EqualsEqualsToken of node_Generic
  | `ExclamationEqualsToken of node_Generic
  | `EqualsEqualsEqualsToken of node_Generic
  | `ExclamationEqualsEqualsToken of node_Generic
  | `EqualsGreaterThanToken of node_Generic
  | `PlusToken of node_Generic
  | `MinusToken of node_Generic
  | `AsteriskToken of node_Generic
  | `AsteriskAsteriskToken of node_Generic
  | `SlashToken of node_Generic
  | `PercentToken of node_Generic
  | `PlusPlusToken of node_Generic
  | `MinusMinusToken of node_Generic
  | `LessThanLessThanToken of node_Generic
  | `GreaterThanGreaterThanToken of node_Generic
  | `GreaterThanGreaterThanGreaterThanToken of node_Generic
  | `AmpersandToken of node_Generic
  | `BarToken of node_Generic
  | `CaretToken of node_Generic
  | `ExclamationToken of node_Generic
  | `TildeToken of node_Generic
  | `AmpersandAmpersandToken of node_Generic
  | `BarBarToken of node_Generic
  | `QuestionToken of node_Generic
  | `ColonToken of node_Generic
  | `AtToken of node_Generic
  | `QuestionQuestionToken of node_Generic
  | `BacktickToken of node_Generic
  | `EqualsToken of node_Generic
  | `PlusEqualsToken of node_Generic
  | `MinusEqualsToken of node_Generic
  | `AsteriskEqualsToken of node_Generic
  | `AsteriskAsteriskEqualsToken of node_Generic
  | `SlashEqualsToken of node_Generic
  | `PercentEqualsToken of node_Generic
  | `LessThanLessThanEqualsToken of node_Generic
  | `GreaterThanGreaterThanEqualsToken of node_Generic
  | `GreaterThanGreaterThanGreaterThanEqualsToken of node_Generic
  | `AmpersandEqualsToken of node_Generic
  | `BarEqualsToken of node_Generic
  | `BarBarEqualsToken of node_Generic
  | `AmpersandAmpersandEqualsToken of node_Generic
  | `QuestionQuestionEqualsToken of node_Generic
  | `CaretEqualsToken of node_Generic
  | `BreakKeyword of node_Generic
  | `CaseKeyword of node_Generic
  | `CatchKeyword of node_Generic
  | `ClassKeyword of node_Generic
  | `ConstKeyword of node_Generic
  | `ContinueKeyword of node_Generic
  | `DebuggerKeyword of node_Generic
  | `DefaultKeyword of node_Generic
  | `DeleteKeyword of node_Generic
  | `DoKeyword of node_Generic
  | `ElseKeyword of node_Generic
  | `EnumKeyword of node_Generic
  | `ExportKeyword of node_Generic
  | `ExtendsKeyword of node_Generic
  | `FalseKeyword of node_Generic
  | `FinallyKeyword of node_Generic
  | `ForKeyword of node_Generic
  | `FunctionKeyword of node_Generic
  | `IfKeyword of node_Generic
  | `ImportKeyword of node_Generic
  | `InKeyword of node_Generic
  | `InstanceOfKeyword of node_Generic
  | `NewKeyword of node_Generic
  | `NullKeyword of node_Generic
  | `ReturnKeyword of node_Generic
  | `SuperKeyword of node_Generic
  | `SwitchKeyword of node_Generic
  | `ThisKeyword of node_Generic
  | `ThrowKeyword of node_Generic
  | `TrueKeyword of node_Generic
  | `TryKeyword of node_Generic
  | `TypeOfKeyword of node_Generic
  | `VarKeyword of node_Generic
  | `VoidKeyword of node_Generic
  | `WhileKeyword of node_Generic
  | `WithKeyword of node_Generic
  | `ImplementsKeyword of node_Generic
  | `InterfaceKeyword of node_Generic
  | `LetKeyword of node_Generic
  | `PackageKeyword of node_Generic
  | `PrivateKeyword of node_Generic
  | `ProtectedKeyword of node_Generic
  | `PublicKeyword of node_Generic
  | `StaticKeyword of node_Generic
  | `YieldKeyword of node_Generic
  | `AbstractKeyword of node_Generic
  | `AsKeyword of node_Generic
  | `AssertsKeyword of node_Generic
  | `AnyKeyword of node_Generic
  | `AsyncKeyword of node_Generic
  | `AwaitKeyword of node_Generic
  | `BooleanKeyword of node_Generic
  | `ConstructorKeyword of node_Generic
  | `DeclareKeyword of node_Generic
  | `GetKeyword of node_Generic
  | `InferKeyword of node_Generic
  | `IsKeyword of node_Generic
  | `KeyOfKeyword of node_Generic
  | `ModuleKeyword of node_Generic
  | `NamespaceKeyword of node_Generic
  | `NeverKeyword of node_Generic
  | `ReadonlyKeyword of node_Generic
  | `RequireKeyword of node_Generic
  | `NumberKeyword of node_Generic
  | `ObjectKeyword of node_Generic
  | `SetKeyword of node_Generic
  | `StringKeyword of node_Generic
  | `SymbolKeyword of node_Generic
  | `TypeKeyword of node_Generic
  | `UndefinedKeyword of node_Generic
  | `UniqueKeyword of node_Generic
  | `UnknownKeyword of node_Generic
  | `FromKeyword of node_Generic
  | `GlobalKeyword of node_Generic
  | `BigIntKeyword of node_Generic
  | `OfKeyword of node_Generic
  | `QualifiedName of node_QualifiedName
  | `ComputedPropertyName of node_Generic
  | `Parameter of node_Parameter
  | `Decorator of node_Generic
  | `PropertySignature of node_PropertySignature
  | `PropertyDeclaration of node_PropertyDeclaration
  | `MethodSignature of node_MethodSignature
  | `MethodDeclaration of node_Generic
  | `Constructor of node_Generic
  | `GetAccessor of node_Generic
  | `SetAccessor of node_Generic
  | `CallSignature of node_CallSignature
  | `ConstructSignature of node_Generic
  | `IndexSignature of node_IndexSignature
  | `TypePredicate of node_Generic
  | `TypeReference of node_TypeReference
  | `FunctionType of node_FunctionType
  | `ConstructorType of node_Generic
  | `TypeQuery of node_Generic
  | `TypeLiteral of node_TypeLiteral
  | `ArrayType of node_ArrayType
  | `TupleType of node_Generic
  | `OptionalType of node_Generic
  | `RestType of node_Generic
  | `UnionType of node_UnionType
  | `IntersectionType of node_Generic
  | `ConditionalType of node_Generic
  | `InferType of node_Generic
  | `ParenthesizedType of node_Generic
  | `ThisType of node_Generic
  | `TypeOperator of node_TypeOperator
  | `IndexedAccessType of node_Generic
  | `MappedType of node_Generic
  | `LiteralType of node_Generic
  | `NamedTupleMember of node_Generic
  | `ImportType of node_Generic
  | `ObjectBindingPattern of node_Generic
  | `ArrayBindingPattern of node_Generic
  | `BindingElement of node_BindingElement
  | `ArrayLiteralExpression of node_Generic
  | `ObjectLiteralExpression of node_Generic
  | `PropertyAccessExpression of node_Generic
  | `ElementAccessExpression of node_Generic
  | `CallExpression of node_Generic
  | `NewExpression of node_Generic
  | `TaggedTemplateExpression of node_Generic
  | `TypeAssertionExpression of node_Generic
  | `ParenthesizedExpression of node_Generic
  | `FunctionExpression of node_Generic
  | `ArrowFunction of node_Generic
  | `DeleteExpression of node_Generic
  | `TypeOfExpression of node_Generic
  | `VoidExpression of node_Generic
  | `AwaitExpression of node_Generic
  | `PrefixUnaryExpression of node_Generic
  | `PostfixUnaryExpression of node_Generic
  | `BinaryExpression of node_Generic
  | `ConditionalExpression of node_Generic
  | `TemplateExpression of node_Generic
  | `YieldExpression of node_Generic
  | `SpreadElement of node_Generic
  | `ClassExpression of node_Generic
  | `OmittedExpression of node_Generic
  | `ExpressionWithTypeArguments of node_Generic
  | `AsExpression of node_Generic
  | `NonNullExpression of node_Generic
  | `MetaProperty of node_Generic
  | `SyntheticExpression of node_Generic
  | `TemplateSpan of node_Generic
  | `SemicolonClassElement of node_Generic
  | `Block of node_Generic
  | `EmptyStatement of node_Generic
  | `VariableStatement of node_Generic
  | `ExpressionStatement of node_Generic
  | `IfStatement of node_Generic
  | `DoStatement of node_Generic
  | `WhileStatement of node_Generic
  | `ForStatement of node_Generic
  | `ForInStatement of node_Generic
  | `ForOfStatement of node_Generic
  | `ContinueStatement of node_Generic
  | `BreakStatement of node_Generic
  | `ReturnStatement of node_Generic
  | `WithStatement of node_Generic
  | `SwitchStatement of node_Generic
  | `LabeledStatement of node_Generic
  | `ThrowStatement of node_Generic
  | `TryStatement of node_Generic
  | `DebuggerStatement of node_Generic
  | `VariableDeclaration of node_VariableDeclaration
  | `VariableDeclarationList of node_VariableDeclarationList
  | `FunctionDeclaration of node_FunctionDeclaration
  | `ClassDeclaration of node_Generic
  | `EnumDeclaration of node_EnumDeclaration
  | `ModuleDeclaration of node_Generic
  | `ModuleBlock of node_Generic
  | `CaseBlock of node_Generic
  | `NamespaceExportDeclaration of node_Generic
  | `ImportEqualsDeclaration of node_Generic
  | `ImportDeclaration of node_Generic
  | `ImportClause of node_Generic
  | `NamespaceImport of node_Generic
  | `NamedImports of node_Generic
  | `ImportSpecifier of node_Generic
  | `ExportAssignment of node_Generic
  | `ExportDeclaration of node_Generic
  | `NamedExports of node_Generic
  | `NamespaceExport of node_Generic
  | `ExportSpecifier of node_Generic
  | `MissingDeclaration of node_Generic
  | `ExternalModuleReference of node_Generic
  | `JsxElement of node_Generic
  | `JsxSelfClosingElement of node_Generic
  | `JsxOpeningElement of node_Generic
  | `JsxClosingElement of node_Generic
  | `JsxFragment of node_Generic
  | `JsxOpeningFragment of node_Generic
  | `JsxClosingFragment of node_Generic
  | `JsxAttribute of node_Generic
  | `JsxAttributes of node_Generic
  | `JsxSpreadAttribute of node_Generic
  | `JsxExpression of node_Generic
  | `CaseClause of node_Generic
  | `DefaultClause of node_Generic
  | `CatchClause of node_Generic
  | `PropertyAssignment of node_Generic
  | `ShorthandPropertyAssignment of node_Generic
  | `SpreadAssignment of node_Generic
  | `EnumMember of node_EnumMember
  | `UnparsedPrologue of node_Generic
  | `UnparsedPrepend of node_Generic
  | `UnparsedText of node_Generic
  | `UnparsedInternalText of node_Generic
  | `UnparsedSyntheticReference of node_Generic
  | `Bundle of node_Generic
  | `UnparsedSource of node_Generic
  | `InputFiles of node_Generic
  | `JSDocTypeExpression of node_Generic
  | `JSDocAllType of node_Generic
  | `JSDocUnknownType of node_Generic
  | `JSDocNullableType of node_Generic
  | `JSDocNonNullableType of node_Generic
  | `JSDocOptionalType of node_Generic
  | `JSDocFunctionType of node_Generic
  | `JSDocVariadicType of node_Generic
  | `JSDocNamepathType of node_Generic
  | `JSDocComment of node_Generic
  | `JSDocTypeLiteral of node_Generic
  | `JSDocSignature of node_Generic
  | `JSDocTag of node_Generic
  | `JSDocAugmentsTag of node_Generic
  | `JSDocImplementsTag of node_Generic
  | `JSDocAuthorTag of node_Generic
  | `JSDocDeprecatedTag of node_Generic
  | `JSDocClassTag of node_Generic
  | `JSDocPublicTag of node_Generic
  | `JSDocPrivateTag of node_Generic
  | `JSDocProtectedTag of node_Generic
  | `JSDocReadonlyTag of node_Generic
  | `JSDocCallbackTag of node_Generic
  | `JSDocEnumTag of node_Generic
  | `JSDocParameterTag of node_Generic
  | `JSDocReturnTag of node_Generic
  | `JSDocThisTag of node_Generic
  | `JSDocTypeTag of node_Generic
  | `JSDocTemplateTag of node_Generic
  | `JSDocTypedefTag of node_Generic
  | `JSDocPropertyTag of node_Generic
  | `SyntaxList of node_Generic
  | `NotEmittedStatement of node_Generic
  | `PartiallyEmittedExpression of node_Generic
  | `CommaListExpression of node_Generic
  | `MergeDeclarationMarker of node_Generic
  | `EndOfDeclarationMarker of node_Generic
  | `SyntheticReferenceExpression of node_Generic
  | `Count of node_Generic
]

and node_ArrayType = {
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

and node_BindingElement = {
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

and node_CallSignature = {
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

and node_EnumDeclaration = {
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

and node_EnumMember = {
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

and node_FunctionDeclaration = {
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

and node_FunctionType = {
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

and node_Generic = {
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

and node_HeritageClause = {
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

and node_Identifier = {
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

and node_IndexSignature = {
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

and node_InterfaceDeclaration = {
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

and node_MethodSignature = {
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

and node_Parameter = {
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

and node_PropertyDeclaration = {
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

and node_PropertySignature = {
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

and node_QualifiedName = {
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

and node_SourceFile = {
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

and node_TypeAliasDeclaration = {
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

and node_TypeLiteral = {
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

and node_TypeOperator = {
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

and node_TypeParameter = {
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

and node_TypeReference = {
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

and node_UnionType = {
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

and node_VariableDeclaration = {
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

and node_VariableDeclarationList = {
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

and symbol = {
  flags: int;
  id: int option;
  resolvedType: type_ option;
  fullyQualifiedName: string;
  name: string
}

and type_ = [
    `ObjectKeyword of type_Generic
  | `Void of type_Generic
  | `Any of type_Generic
  | `Array of type_Generic
  | `Boolean of type_Generic
  | `String of type_Generic
  | `Number of type_Generic
  | `Literal of type_Generic
  | `BooleanLiteral of type_Generic
  | `Union of type_UnionType
  | `EnumLiteral of type_StringLiteralType
  | `NumberLiteral of type_NumberLiteralType
  | `StringLiteral of type_StringLiteralType
  | `Class of type_Generic
  | `ClassOrInterface of type_Generic
  | `Interface of type_InterfaceType
  | `Object of type_ObjectType
  | `Enum of type_Generic
  | `TypeParameter of type_TypeParameter
  | `Tuple of type_TupleType
  | `Intersection of type_IntersectionType
  | `UnionOrIntersection of type_UnionOrIntersectionType
  | `Unknown of type_Generic
  | `Null of type_Generic
  | `Undefined of type_Generic
  | `TypeReference of type_TypeReference
  | `Anonymous of type_AnonymousType
  | `Unidentified of type_Unidentified
]

and type_AnonymousType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string;
  objectFlags: int;
  target: type_ option
}

and type_Generic = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string
}

and type_InterfaceType = {
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

and type_IntersectionType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string;
  types: type_ list
}

and type_NumberLiteralType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string;
  value: float
}

and type_ObjectType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string;
  objectFlags: int
}

and type_StringLiteralType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string;
  value: string
}

and type_TupleType = {
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

and type_TypeParameter = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string;
  constraint_: type_ option
}

and type_TypeReference = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string;
  target: type_ option
}

and type_Unidentified = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string;
  nodeKind: string option
}

and type_UnionOrIntersectionType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string;
  types: type_ list
}

and type_UnionType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string;
  types: type_ list
}

type conditionalRoot = {
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

type type_conditionalType = {
  root: conditionalRoot;
  checkType: type_;
  extendsType: type_;
  resolvedTrueType: type_;
  resolvedFalseType: type_
}

type type_LiteralType = {
  flags: int;
  id: int option;
  pattern: node option;
  aliasTypeArguments: type_ list option;
  kindName: string
}

type readonlyTextRange = { pos: int; end_: int }

type node_TypeElement = {
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

type node_SignatureDeclarationBase = {
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

type node_NodeWithTypeArguments = {
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

type node_NamedDeclaration = {
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

type node_FunctionLikeDeclarationBase = {
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

import { ts, ScriptKind as mScriptKind } from 'ts-morph';

export type SymbolId = number & { __symbolIdentifier: void };
export type NodeId<N extends Node = Node> = number & {
  __nodeIdentifier: void;
} & N;
export type TypeId = number & { __typeIdentifier: void };

export interface ReadonlyTextRange {
  readonly pos: number;
  readonly end: number;
}

type NodeArray<T> = Array<T>;
export interface Node extends ReadonlyTextRange {
  kind: ts.SyntaxKind;
  kindName: string;
  flags: ts.NodeFlags;
  parent?: NodeId;
  id: NodeId;
  symbol?: SymbolId;
}

export interface ModifierableNode extends Node {}

export interface Symbol {
  flags: ts.SymbolFlags;
  name: string;
  escapedName: string;
  fullyQualifiedName: string;
  aliasedSymbol: SymbolId | undefined;
  exportSymbol: SymbolId | undefined;
  valueDeclaration: NodeId | undefined;
  declarations: NodeId[];
  members: SymbolId[];
  declaredType: TypeId;
  id: SymbolId;
}

export interface Type {
  flags: ts.TypeFlags;
  symbol?: SymbolId;
  aliasSymbol?: SymbolId;
  aliasTypeArguments?: TypeId[] | undefined;
  id: TypeId;
}
export interface IntrinsicType extends Type {
  intrinsicName: string;

  objectFlags: ts.ObjectFlags;
}

export interface Declaration extends Node {}

export interface Identifier extends Node {
  readonly originalKeywordKind?: ts.SyntaxKind;
  readonly escapedText: string;
}

export interface LiteralLikeNode extends Node {
  readonly kind: ts.SyntaxKind;
  isUnterminated?: boolean;
  hasExtendedUnicodeEscape?: boolean;
}

export interface StringLiteral extends LiteralLikeNode, Declaration {
  readonly kind: ts.SyntaxKind.StringLiteral;
  value: string;
}
export interface NumericLiteral extends LiteralLikeNode, Declaration {
  readonly kind: ts.SyntaxKind.NumericLiteral;
  value: number;
}
export interface TemplateLiteralLikeNode extends LiteralLikeNode {
  rawText?: string;
}
export interface NoSubstitutionTemplateLiteral
  extends LiteralLikeNode,
    TemplateLiteralLikeNode,
    Declaration {
  readonly kind: ts.SyntaxKind.NoSubstitutionTemplateLiteral;
}

export type StringLiteralLike = StringLiteral | NoSubstitutionTemplateLiteral;
export type PropertyNameLiteral =
  | Identifier
  | StringLiteralLike
  | NumericLiteral;

export interface NamedDeclaration extends Declaration {
  symbol?: SymbolId;
  name?: NodeId;
}

export interface DeclarationStatement extends NamedDeclaration {}

export interface Statement extends Node {}

export interface ModuleBlock extends Node {
  readonly parent?: NodeId<NamespaceDeclaration>;
  readonly statements: NodeId[];
}

export type NamespaceBody = ModuleBlock | NamespaceDeclaration;

export interface ModuleDeclaration extends DeclarationStatement {
  readonly kind: ts.SyntaxKind.ModuleDeclaration;
  readonly parent?: NodeId;
  readonly name: NodeId<Identifier | StringLiteral>;
  readonly body?: NodeId;
}

export interface NamespaceDeclaration extends ModuleDeclaration {
  readonly name: NodeId<Identifier>;
  readonly body: NodeId;
  readonly symbols: SymbolId[];
}

// | SignatureDeclaration
// | ClassLikeDeclaration
// | TypeLiteralNode

export type SignatureDeclaration = Node;
// =
// | CallSignatureDeclaration
// | ConstructSignatureDeclaration
// | MethodSignature
// | IndexSignatureDeclaration
// | FunctionTypeNode
// | ConstructorTypeNode
// | JSDocFunctionType
// | FunctionDeclaration
// | MethodDeclaration
// | ConstructorDeclaration
// | AccessorDeclaration
// | FunctionExpression
// | ArrowFunction;

export interface InterfaceDeclaration extends DeclarationStatement {
  readonly kind: ts.SyntaxKind.InterfaceDeclaration;
  readonly name: NodeId<Identifier>;
  readonly typeParameters?: NodeArray<NodeId<TypeParameterDeclaration>>;
  readonly heritageClauses?: NodeArray<NodeId<HeritageClause>>;
  readonly members: NodeArray<NodeId<TypeElement>>;
}

export interface ClassLikeDeclarationBase extends NamedDeclaration {
  readonly kind: ts.SyntaxKind.ClassDeclaration | ts.SyntaxKind.ClassExpression;
  readonly name?: NodeId;
  readonly typeParameters?: NodeArray<TypeParameterDeclaration>;
  readonly heritageClauses?: NodeArray<HeritageClause>;
  readonly members: NodeArray<ClassElement>;
}

export interface ClassElement extends NamedDeclaration {
  _classElementBrand: any;
  readonly name?: NodeId<PropertyName>;
}

export interface ClassDeclaration
  extends ClassLikeDeclarationBase,
    DeclarationStatement {
  readonly kind: ts.SyntaxKind.ClassDeclaration;
  /** May be undefined in `export default class { ... }`. */
  readonly name?: NodeId;
}

export type ClassLikeDeclaration = ClassDeclaration | ClassExpression;

export interface ClassExpression extends ClassLikeDeclarationBase {
  readonly kind: ts.SyntaxKind.ClassExpression;
}

export interface HeritageClause extends Node {
  readonly kind: ts.SyntaxKind.HeritageClause;
  readonly parent: NodeId<InterfaceDeclaration | ClassLikeDeclaration>;
  readonly token:
    | ts.SyntaxKind.ExtendsKeyword
    | ts.SyntaxKind.ImplementsKeyword;
  readonly types: NodeArray<NodeId<ExpressionWithTypeArguments>>;
}

export interface TypeNode extends Node {}

export type TypeReferenceType = TypeReferenceNode | ExpressionWithTypeArguments;

export interface NodeWithTypeArguments extends TypeNode {
  readonly typeArguments?: NodeArray<TypeNode>;
}

export interface ExpressionWithTypeArguments extends NodeWithTypeArguments {
  readonly kind: ts.SyntaxKind.ExpressionWithTypeArguments;
  readonly parent: NodeId<HeritageClause>;
  readonly expression: NodeId<Node>;
}

export interface QualifiedName extends Node {
  readonly kind: ts.SyntaxKind.QualifiedName;
  readonly left: NodeId<EntityName>;
  readonly right: NodeId<Identifier>;
}

export interface Token<TKind extends ts.SyntaxKind> extends Node {
  readonly kind: TKind;
}
export interface KeywordToken<TKind extends ts.KeywordSyntaxKind>
  extends Token<TKind> {}
export interface PunctuationToken<TKind extends ts.SyntaxKind>
  extends Token<TKind> {}
export type QuestionToken = PunctuationToken<ts.SyntaxKind.QuestionToken>;
export type AssertsKeyword = KeywordToken<ts.SyntaxKind.AssertsKeyword>;
export type AwaitKeyword = KeywordToken<ts.SyntaxKind.AwaitKeyword>;

export type EntityName = Identifier | QualifiedName;

export type PropertyName =
  | Identifier
  | StringLiteral
  | NumericLiteral
  | ComputedPropertyName
  | PrivateIdentifier;

export interface PrivateIdentifier extends Node {
  readonly kind: ts.SyntaxKind.PrivateIdentifier;
  readonly escapedText: string;
}

export interface Expression extends Node {
  _expressionBrand: any;
}

export interface ComputedPropertyName extends Node {
  readonly kind: ts.SyntaxKind.ComputedPropertyName;
  readonly parent: NodeId;
  readonly expression: NodeId<Expression>;
}

export interface ThisTypeNode extends TypeNode {
  readonly kind: ts.SyntaxKind.ThisType;
}

export interface TypeReferenceNode extends NodeWithTypeArguments {
  readonly kind: ts.SyntaxKind.TypeReference;
  readonly typeName: NodeId<EntityName>;
}

export interface TypePredicateNode extends TypeNode {
  readonly kind: ts.SyntaxKind.TypePredicate;
  readonly parent: NodeId<SignatureDeclaration>;
  readonly assertsModifier?: NodeId<AssertsKeyword>;
  readonly parameterName: NodeId<Identifier | ThisTypeNode>;
  readonly type?: NodeId<TypeNode>;
}

// TODO: N/A in morph
export interface TypeQueryNode extends TypeNode {
  readonly kind: ts.SyntaxKind.TypeQuery;
  readonly exprName: EntityName;
}

export interface TypeElement extends NamedDeclaration {
  _typeElementBrand: any;
  readonly name?: NodeId<PropertyName>;
  readonly questionToken?: NodeId<QuestionToken>;
}

// A TypeLiteral is the declaration node for an anonymous symbol.
export interface TypeLiteralNode extends TypeNode, Declaration {
  readonly kind: ts.SyntaxKind.TypeLiteral;
  readonly members: NodeArray<TypeElement>;
}

export interface ArrayTypeNode extends TypeNode {
  readonly kind: ts.SyntaxKind.ArrayType;
  readonly elementType: NodeId<TypeNode>;
}

// TODO: Not found in morph
export interface OptionalTypeNode extends TypeNode {
  readonly kind: ts.SyntaxKind.OptionalType;
  readonly type: NodeId<TypeNode>;
}

export interface TupleTypeNode extends TypeNode {
  readonly kind: ts.SyntaxKind.TupleType;
  readonly elements: NodeArray<NodeId<TypeNode | NamedTupleMember>>;
}

export interface NamedTupleMember extends TypeNode, Declaration {
  readonly kind: ts.SyntaxKind;
  readonly dotDotDotToken?: Token<ts.SyntaxKind.DotDotDotToken>;
  readonly name: NodeId<Identifier>;
  readonly questionToken?: Token<ts.SyntaxKind.QuestionToken>;
  readonly type: NodeId<TypeNode>;
}

export interface TypeAliasDeclaration extends NamedDeclaration {
  parameters: NodeId[];
  type?: TypeId;
  modifiers: NodeId[];
}

export interface TypeParameterDeclaration extends NamedDeclaration {
  parent?: NodeId;
  constraint?: NodeId;
  default?: NodeId;
}

export interface SourceFile extends Node {
  filePath: string;
  baseName: string;
  baseNameWithoutExtension: string;
  extension: string;
  scriptKind: mScriptKind;
  isDeclarationFile: boolean;
  isFromExternalLibrary: boolean;
  isInNodeModules: boolean;
  fullSource: string;
  symbols: SymbolId[];
  statements: NodeId[];
}

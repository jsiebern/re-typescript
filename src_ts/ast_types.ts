import { ts, ScriptKind as mScriptKind } from 'ts-morph';

export type SymbolId = number & { __symbolIdentifier: void };
export type NodeId<N extends Node = Node> = number & {
  __nodeIdentifier: void;
} & N;
export type TypeId<T extends Type = Type> = number & {
  __typeIdentifier: void;
} & T;

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
  readonly name?: NodeId<Identifier>;
  readonly typeParameters?: NodeArray<NodeId<TypeParameterDeclaration>>;
  readonly heritageClauses?: NodeArray<NodeId<HeritageClause>>;
  readonly members: NodeArray<NodeId<ClassElement>>;
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
  readonly name?: NodeId<Identifier>;
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
  readonly parent: NodeId<Declaration>;
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

// export interface MappedTypeNode extends TypeNode, Declaration {
//   readonly kind: SyntaxKind.MappedType;
//   readonly readonlyToken?: ReadonlyToken | PlusToken | MinusToken;
//   readonly typeParameter: TypeParameterDeclaration;
//   readonly questionToken?: QuestionToken | PlusToken | MinusToken;
//   readonly type?: TypeNode;
// }

////////////////////////////////////////////////////////////////////////////////
// --- Types
////////////////////////////////////////////////////////////////////////////////
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
export interface NullableType extends IntrinsicType {
  objectFlags: ts.ObjectFlags;
}
export interface PseudoBigInt {
  negative: boolean;
  base10Value: string;
}
// String literal types (TypeFlags.StringLiteral)
// Numeric literal types (TypeFlags.NumberLiteral)
// BigInt literal types (TypeFlags.BigIntLiteral)
export interface LiteralType extends Type {
  value: string | number | PseudoBigInt; // Value of literal
}

// Unique symbol types (TypeFlags.UniqueESSymbol)
export interface UniqueESSymbolType extends Type {
  symbol: SymbolId;
  escapedName: string;
}

export interface StringLiteralType extends LiteralType {
  value: string;
}

export interface NumberLiteralType extends LiteralType {
  value: number;
}

export interface BigIntLiteralType extends LiteralType {
  value: PseudoBigInt;
}

// Enum types (TypeFlags.Enum)
export interface EnumType extends Type {}

export type ObjectFlagsType =
  | NullableType
  | ObjectType
  | UnionType
  | IntersectionType;

// Object types (TypeFlags.ObjectType)
export interface ObjectType extends Type {
  objectFlags: ts.ObjectFlags;
}

export interface UnionType extends UnionOrIntersectionType {
  resolvedReducedType: Type;
  regularType: UnionType;
}
export interface UnionOrIntersectionType extends Type {
  types: Type[];
}
export interface IntersectionType extends UnionOrIntersectionType {
  resolvedApparentType: Type;
}

// keyof T types (TypeFlags.Index)
export interface IndexType extends InstantiableType {
  type: InstantiableType | UnionOrIntersectionType;
  stringsOnly: boolean;
}

export interface InstantiableType extends Type {
  /* @internal */
  resolvedBaseConstraint?: Type;
  /* @internal */
  resolvedIndexType?: IndexType;
  /* @internal */
  resolvedStringIndexType?: IndexType;
}

// Type parameters (TypeFlags.TypeParameter)
export interface TypeParameter extends InstantiableType {
  /** Retrieve using getConstraintFromTypeParameter */
  /* @internal */
  constraint?: Type; // Constraint
  /* @internal */
  default?: Type;
}

/** Class and interface types (ObjectFlags.Class and ObjectFlags.Interface). */
export interface InterfaceType extends ObjectType {
  typeParameters: TypeParameter[] | undefined; // Type parameters (undefined if non-generic)
  outerTypeParameters: TypeParameter[] | undefined; // Outer type parameters (undefined if none)
  localTypeParameters: TypeParameter[] | undefined; // Local type parameters (undefined if none)
  thisType: TypeParameter | undefined; // The "this" type (undefined if none)
  resolvedBaseConstructorType?: Type; // Resolved base constructor type of class
  resolvedBaseTypes: BaseType[]; // Resolved base types
}

// Indexed access types (TypeFlags.IndexedAccess)
// Possible forms are T[xxx], xxx[T], or xxx[keyof T], where T is a type variable
export interface IndexedAccessType extends InstantiableType {
  objectType: Type;
  indexType: Type;
  constraint?: Type;
  simplifiedForReading?: Type;
  simplifiedForWriting?: Type;
}

type BindingPattern = never;
export type TypeVariable = TypeParameter | IndexedAccessType;
export type BindingName = Identifier | BindingPattern;

export interface ParameterDeclaration extends NamedDeclaration {
  readonly kind: ts.SyntaxKind.Parameter;
  readonly parent: NodeId<SignatureDeclaration>;
  readonly dotDotDotToken?: PunctuationToken<ts.SyntaxKind.DotDotDotToken>; // Present on rest parameter
  readonly name: NodeId<BindingName>; // Declared parameter name.
  readonly questionToken?: QuestionToken; // Present on optional parameter
  readonly type?: TypeNode; // Optional type annotation
  readonly initializer?: Expression; // Optional initializer
}

export interface SignatureDeclarationBase extends NamedDeclaration {
  readonly kind: SignatureDeclaration['kind'];
  readonly name?: NodeId<PropertyName>;
  readonly typeParameters?: NodeArray<NodeId<TypeParameterDeclaration>>;
  readonly parameters: NodeArray<NodeId<ParameterDeclaration>>;
  readonly type?: NodeId<TypeNode>;
}

type ObjectTypeDeclaration = never;
export interface IndexSignatureDeclaration
  extends SignatureDeclarationBase,
    ClassElement,
    TypeElement {
  readonly kind: ts.SyntaxKind.IndexSignature;
  readonly parent: NodeId<ObjectTypeDeclaration>;
  readonly type: NodeId<TypeNode>;
}

export interface IndexInfo {
  type: Type;
  isReadonly: boolean;
  declaration?: IndexSignatureDeclaration;
}
export interface InterfaceTypeWithDeclaredMembers extends InterfaceType {
  declaredProperties: Symbol[]; // Declared members
  declaredCallSignatures: Signature[]; // Declared call signatures
  declaredConstructSignatures: Signature[]; // Declared construct signatures
  declaredStringIndexInfo?: IndexInfo; // Declared string indexing info
  declaredNumberIndexInfo?: IndexInfo; // Declared numeric indexing info
}

export type BaseType = ObjectType | IntersectionType | TypeVariable; // Also `any` and `object`

type MappedTypeNode = never;
export interface MappedType extends ObjectType {
  declaration: MappedTypeNode;
  typeParameter?: TypeParameter;
  constraintType?: Type;
  templateType?: Type;
  modifiersType?: Type;
  resolvedApparentType?: Type;
}

////////////////////////////////////////////////////////////////////////////////
// --- Signatures
////////////////////////////////////////////////////////////////////////////////

type JSDocSignature = never;
export interface Signature {
  declaration?: SignatureDeclaration | JSDocSignature; // Originating declaration
  typeParameters?: readonly TypeParameter[]; // Type parameters (undefined if non-generic)
  parameters: readonly Symbol[]; // Parameters
  /* @internal */
  thisParameter?: Symbol; // symbol of this-type parameter
  /* @internal */
  // See comment in `instantiateSignature` for why these are set lazily.
  resolvedReturnType?: Type; // Lazily set by `getReturnTypeOfSignature`.
  /* @internal */
  // Lazily set by `getTypePredicateOfSignature`.
  // `undefined` indicates a type predicate that has not yet been computed.
  // Uses a special `noTypePredicate` sentinel value to indicate that there is no type predicate. This looks like a TypePredicate at runtime to avoid polymorphism.
  resolvedTypePredicate?: TypePredicate;
}

export type TypePredicate =
  | ThisTypePredicate
  | IdentifierTypePredicate
  | AssertsThisTypePredicate
  | AssertsIdentifierTypePredicate;

export const enum TypePredicateKind {
  This,
  Identifier,
  AssertsThis,
  AssertsIdentifier,
}

export interface TypePredicateBase {
  kind: TypePredicateKind;
  type: TypeId | undefined;
}

export interface ThisTypePredicate extends TypePredicateBase {
  kind: TypePredicateKind.This;
  parameterName: undefined;
  parameterIndex: undefined;
  type: TypeId;
}

export interface IdentifierTypePredicate extends TypePredicateBase {
  kind: TypePredicateKind.Identifier;
  parameterName: string;
  parameterIndex: number;
  type: TypeId;
}

export interface AssertsThisTypePredicate extends TypePredicateBase {
  kind: TypePredicateKind.AssertsThis;
  parameterName: undefined;
  parameterIndex: undefined;
  type: TypeId | undefined;
}

export interface AssertsIdentifierTypePredicate extends TypePredicateBase {
  kind: TypePredicateKind.AssertsIdentifier;
  parameterName: string;
  parameterIndex: number;
  type: TypeId | undefined;
}

import { ts, ScriptKind as mScriptKind } from 'ts-morph';

export interface Node {
  kind: number;
  kindName: string;
  flags: ts.NodeFlags;
  parent?: Node;
}

export interface ModifierableNode extends Node {}

export interface ISymbol {
  flags: ts.SymbolFlags;
  name: string;
  escapedName: string;
  fullyQualifiedName: string;
  aliasedSymbol: Symbol | undefined;
  exportSymbol: Symbol | undefined;
  valueDeclaration: Node | undefined;
  declarations: Node[];
  members: Symbol[];
  declaredType: Type;
  id: number;
}
export type Symbol = ISymbol | { id: number };

export interface Type {
  flags: ts.TypeFlags;
  symbol?: Symbol;
  aliasSymbol?: Symbol;
  aliasTypeArguments?: Type[];
}
export interface IntrinsicType extends Type {
  intrinsicName: string;
  objectFlags: ts.ObjectFlags;
}

export interface Declaration extends Node {
  readonly kind: ts.SyntaxKind;
}

export interface Identifier extends Node {
  readonly kind: ts.SyntaxKind.Identifier;
  readonly originalKeywordKind?: ts.SyntaxKind;
  readonly escapedText: string;
}

export interface LiteralLikeNode extends Node {
  readonly kind: ts.SyntaxKind;
  text: string;
  isUnterminated?: boolean;
  hasExtendedUnicodeEscape?: boolean;
}

export interface StringLiteral extends LiteralLikeNode, Declaration {
  readonly kind: ts.SyntaxKind.StringLiteral;
}
export interface NumericLiteral extends LiteralLikeNode, Declaration {
  readonly kind: ts.SyntaxKind.NumericLiteral;
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

export type DeclarationName =
  | Identifier
  // | PrivateIdentifier
  | StringLiteralLike
  | NumericLiteral;
// | ComputedPropertyName
// | ElementAccessExpression
// | BindingPattern
// | EntityNameExpression;

export interface NamedDeclaration extends Declaration {
  symbol: Symbol | undefined;
  kind: ts.SyntaxKind;
  kindName: string;
  name?: DeclarationName;
}

export interface DeclarationStatement extends NamedDeclaration {
  readonly name?: Identifier | StringLiteral | NumericLiteral;
}

export interface Statement extends Node {}

export interface ModuleBlock extends Node {
  readonly kind: ts.SyntaxKind.ModuleBlock;
  readonly kindName: string;
  readonly parent?: ModuleDeclaration;
  readonly statements: Statement[];
}

export type NamespaceBody = ModuleBlock | NamespaceDeclaration;

export interface ModuleDeclaration extends DeclarationStatement {
  readonly kind: ts.SyntaxKind.ModuleDeclaration;
  readonly parent?: NamespaceBody | SourceFile;
  readonly name: Identifier | StringLiteral;
  readonly body?: NamespaceBody;
}

export interface NamespaceDeclaration extends ModuleDeclaration {
  readonly name: Identifier;
  readonly body: NamespaceBody;
  readonly symbols: Symbol[];
}

export type DeclarationWithTypeParameterChildren = Node;
// | SignatureDeclaration
// | ClassLikeDeclaration
// | InterfaceDeclaration
// | TypeAliasDeclaration

export interface TypeParameterDeclaration extends NamedDeclaration {
  parent?: DeclarationWithTypeParameterChildren;
  constraint: Node | undefined;
  default: Node | undefined;
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
  symbols: Symbol[];
}

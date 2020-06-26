import * as Debug from './debug';
import {
  ts,
  Symbol as mSymbol,
  Type as mType,
  Node as mNode,
  TypeAliasDeclaration as mTypeAliasDeclaration,
  TypeParameterDeclaration as mTypeParameterDeclaration,
  SourceFile as mSourceFile,
  Identifier as mIdentifier,
  NamespaceDeclaration as mNamespaceDeclaration,
  ModuleBlock as mModuleBlock,
  Statement as mStatement,
  StringLiteralLike as mStringLiteralLike,
  NumericLiteral as mNumericLiteral,
  HeritageClause as mHeritageClause,
  InterfaceDeclaration as mInterfaceDeclaration,
  TypePredicateNode as mTypePredicateNode,
  TupleTypeNode as mTupleTypeNode,
  ArrayTypeNode as mArrayTypeNode,
  TypeLiteralNode as mTypeLiteralNode,
  TypeElement as mTypeElement,
  TypeReferenceNode as mTypeReferenceNode,
  ThisTypeNode as mThisTypeNode,
  ComputedPropertyName as mComputedPropertyName,
  Expression as mExpression,
  PrivateIdentifier as mPrivateIdentifier,
  EntityName as mEntityName,
  QualifiedName as mQualifiedName,
  ExpressionWithTypeArguments as mExpressionWithTypeArguments,
  ClassExpression as mClassExpression,
  ClassDeclaration as mClassDeclaration,
  ClassElement as mClassElement,
  ClassLikeDeclarationBase as mClassLikeDeclarationBase,
} from 'ts-morph';
import {
  SourceFile,
  Statement,
  HeritageClause,
  InterfaceDeclaration,
  TypePredicateNode,
  TupleTypeNode,
  ArrayTypeNode,
  TypeLiteralNode,
  TypeElement,
  TypeReferenceNode,
  ThisTypeNode,
  ComputedPropertyName,
  Expression,
  PrivateIdentifier,
  EntityName,
  QualifiedName,
  ExpressionWithTypeArguments,
  ClassExpression,
  ClassDeclaration,
  ClassElement,
  ClassLikeDeclarationBase,
  // DeclarationName,
  // DeclarationStatement,
  TypeAliasDeclaration,
  Identifier,
  IntrinsicType,
  TypeParameterDeclaration,
  // LiteralLikeNode,
  // NamedDeclaration,
  Symbol,
  Node,
  Type,
  // ModifierableNode,
  ModuleBlock,
  ModuleDeclaration,
  NamespaceBody,
  NamespaceDeclaration,
  // NumericLiteral,
  // PropertyNameLiteral,
  StringLiteral,
  NumericLiteral,
  NodeId,
  SymbolId,
  TypeId,
} from './ast_types';

Debug.setLogLevel(Debug.LogLevel.Warning);

const maybe = (a: any, b: any) => (b == null ? undefined : a(b));

export function createNode(node: mNode): Node {
  return {
    pos: node.getPos(),
    end: node.getEnd(),
    flags: node.compilerNode.flags,
    kind: node.getKind(),
    kindName: node.getKindName(),
    symbol: maybe(visitSymbol, node.getSymbol()),
    id: getNodeId(node),
  };
}

export function createSymbol(symbol: mSymbol): Symbol {
  return {
    flags: symbol.getFlags(),
    name: symbol.getName(),
    escapedName: symbol.getEscapedName(),
    fullyQualifiedName: symbol.getFullyQualifiedName(),
    aliasedSymbol: maybe(visitSymbol, symbol.getAliasedSymbol()),
    exportSymbol: maybe(visitSymbol, symbol.getExportSymbol()),
    valueDeclaration: maybe(visitNode, symbol.getValueDeclaration()),
    declarations: symbol.getDeclarations().map((d) => visitNode(d)),
    members: symbol.getMembers().map(visitSymbol),
    declaredType: visitType(symbol.getDeclaredType()),
    id: getSymbolId(symbol),
  };
}

function maybeAddIntrinsicProps(type: mType) {
  let intrinsic = type.isAny()
    ? 'any'
    : type.isBoolean()
    ? 'boolean'
    : type.isNull()
    ? 'null'
    : type.isNumber()
    ? 'number'
    : type.isString()
    ? 'string'
    : type.isUnknown()
    ? 'unknown'
    : type.getText() == 'never'
    ? 'never'
    : undefined;
  return intrinsic
    ? {
        objectFlags: type.getObjectFlags(),
        intrinsicName: intrinsic,
      }
    : {};
}
export function createType(type: mType): Type | IntrinsicType {
  const base = {
    id: getTypeId(type),
    flags: type.getFlags(),
    symbol: maybe(visitSymbol, type.getSymbol()),
    aliasSymbol: maybe(visitSymbol, type.getAliasSymbol()),
    aliasTypeArguments: type.getAliasTypeArguments().map((t) => visitType(t)),
  };

  return {
    ...base,
    ...maybeAddIntrinsicProps(type),
  };
}

export function createIdentifier(ident: mIdentifier): Identifier {
  return {
    ...createNode(ident),
    escapedText: ident.compilerNode.escapedText.toString(),
    originalKeywordKind: ident.compilerNode.originalKeywordKind,
  };
}

export function createTypeParameterDeclaration(
  typeParam: mTypeParameterDeclaration
): TypeParameterDeclaration {
  return {
    ...createNode(typeParam),
    flags: typeParam.compilerNode.flags,
    symbol: maybe(visitSymbol, typeParam.getSymbol()),
    name: visitNode(typeParam.getNameNode()),
    parent: maybe(visitNode, typeParam.getParent()),
    constraint: maybe(visitNode, typeParam.getConstraint()),
    default: maybe(visitNode, typeParam.getDefault()),
  };
}

export function createTypeAliasDeclaration(
  typeAlias: mTypeAliasDeclaration
): TypeAliasDeclaration {
  return {
    ...createNode(typeAlias),
    flags: typeAlias.compilerNode.flags,
    symbol: maybe(visitSymbol, typeAlias.getSymbol()),
    name: visitNode(typeAlias.getNameNode()),
    parameters: typeAlias.getTypeParameters().map((tp) => visitNode(typeAlias)),
    type: maybe(visitType, typeAlias.getSymbol()?.getDeclaredType()),
    modifiers: typeAlias.getModifiers().map((m) => visitNode(m)),
  };
}

export function createStatement(st: mStatement): Statement {
  return createNode(st);
}

export function createModuleBlock(block: mModuleBlock): ModuleBlock {
  return {
    ...createNode(block),
    parent: maybe(visitNode, block.getParent()),
    statements: block.forEachChildAsArray().map((s) => visitNode(s)),
  };
}

export function createStringLiteral(str: mStringLiteralLike): StringLiteral {
  return {
    ...createNode(str),
    kind: ts.SyntaxKind.StringLiteral,
    value: str.getText(),
    isUnterminated: str.compilerNode.isUnterminated,
    hasExtendedUnicodeEscape: str.compilerNode.hasExtendedUnicodeEscape,
  };
}
export function createNumericLiteral(num: mNumericLiteral): NumericLiteral {
  return {
    ...createNode(num),
    kind: ts.SyntaxKind.NumericLiteral,
    value: num.getLiteralValue(),
    isUnterminated: num.compilerNode.isUnterminated,
    hasExtendedUnicodeEscape: num.compilerNode.hasExtendedUnicodeEscape,
  };
}

export function createNamespaceBody(body: mNode) {
  if (mNode.isModuleBlock(body)) {
    return createModuleBlock(body);
  }
  if (mNode.isNamespaceDeclaration(body)) {
    return createNamespaceDeclaration(body);
  }
  throw new Error('Invalid body type');
}

export function createNameOfIdentifierOrLiteral(
  node: mNode
): Identifier | StringLiteral {
  if (mNode.isIdentifier(node)) {
    return createIdentifier(node);
  }
  if (mNode.isStringLiteral(node)) {
    return createStringLiteral(node);
  }
  throw new Error('Not an identifier or string literal');
}

export function createModuleBlockOrSourceFile(node: mNode) {
  if (mNode.isModuleBlock(node)) {
    return createModuleBlock(node);
  }
  if (mNode.isSourceFile(node)) {
    return createSourceFile(node);
  }
  throw new Error('not a module block or sourc file');
}

export function createModuleDeclaration(
  namespace: mNamespaceDeclaration
): ModuleDeclaration {
  return {
    ...createNode(namespace),
    kind: ts.SyntaxKind.ModuleDeclaration,
    symbol: maybe(visitSymbol, namespace.getSymbol()),
    parent: visitNode(namespace.getParent()),
    name: visitNode(namespace.getNameNode()),
    body: visitNode(namespace.getBody()),
  };
}

export function createNamespaceDeclaration(
  namespace: mNamespaceDeclaration
): NamespaceDeclaration {
  return {
    ...createModuleDeclaration(namespace),
    name: visitNode(namespace.getNameNode()),
    body: visitNode(namespace.getBody()),
    symbols: namespace.getLocals().map(visitSymbol),
  };
}

export function createHeritageClause(node: mHeritageClause): HeritageClause {
  return {
    ...createNode(node),
    kind: ts.SyntaxKind.HeritageClause,
    parent: visitNode(node.getParent()),
    token: node.getToken(),
    types: node
      .getTypeNodes()
      .map((d) => visitNode<ExpressionWithTypeArguments>(d)),
  };
}
export function createInterfaceDeclaration(
  node: mInterfaceDeclaration
): InterfaceDeclaration {
  return {
    ...createNode(node),
    kind: ts.SyntaxKind.InterfaceDeclaration,
    name: visitNode(node.getNameNode()),
    typeParameters: node.getTypeParameters().map(visitNode),
    heritageClauses: node
      .getHeritageClauses()
      .map((d) => visitNode<HeritageClause>(d)),
    members: node.getMembers().map((d) => visitNode<TypeElement>(d)),
  };
}

export function createTypePredicateNode(
  node: mTypePredicateNode
): TypePredicateNode {
  return {
    ...createNode(node),
    kind: ts.SyntaxKind.TypePredicate,
    parent: visitNode(node.getParent()),
    assertsModifier: maybe(visitNode, node.getAssertsModifier()),
    parameterName: visitNode(node.getParameterNameNode()),
    type: maybe(visitNode, node.getTypeNode()),
  };
}
// export function createTupleTypeNode(node: mTupleTypeNode): TupleTypeNode {
//   return {};
// }
// export function createArrayTypeNode(node: mArrayTypeNode): ArrayTypeNode {
//   return {};
// }
// export function createTypeLiteralNode(node: mTypeLiteralNode): TypeLiteralNode {
//   return {};
// }
// export function createTypeElement(node: mTypeElement): TypeElement {
//   return {};
// }
export function createTypeReferenceNode(
  node: mTypeReferenceNode
): TypeReferenceNode {
  return {
    ...createNode(node),
    kind: ts.SyntaxKind.TypeReference,
    typeName: visitNode(node.getTypeName()),
  };
}
// export function createThisTypeNode(node: mThisTypeNode): ThisTypeNode {
//   return {};
// }
// export function createComputedPropertyName(
//   node: mComputedPropertyName
// ): ComputedPropertyName {
//   return {};
// }
// export function createExpression(node: mExpression): Expression {
//   return {};
// }
// export function createPrivateIdentifier(
//   node: mPrivateIdentifier
// ): PrivateIdentifier {
//   return {};
// }
export function createQualifiedName(node: mQualifiedName): QualifiedName {
  return {
    ...createNode(node),
    kind: ts.SyntaxKind.QualifiedName,
    left: visitNode(node.getLeft()),
    right: visitNode(node.getRight()),
  };
}
export function createExpressionWithTypeArguments(
  node: mExpressionWithTypeArguments
): ExpressionWithTypeArguments {
  return {
    ...createNode(node),
    kind: ts.SyntaxKind.ExpressionWithTypeArguments,
    parent: visitNode(node.getParent()),
    expression: visitNode(node.getExpression()),
  };
}
// export function createClassExpression(node: mClassExpression): ClassExpression {
//   return {};
// }
// export function createClassDeclaration(
//   node: mClassDeclaration
// ): ClassDeclaration {
//   return {};
// }
// export function createClassElement(node: mClassElement): ClassElement {
//   return {};
// }
// export function createClassLikeDeclarationBase(
//   node: mClassLikeDeclarationBase
// ): ClassLikeDeclarationBase {
//   return {};
// }

export function createSourceFile(sourceFile: mSourceFile): SourceFile {
  return {
    ...createNode(sourceFile),
    flags: sourceFile.compilerNode.flags,
    filePath: sourceFile.getFilePath(),
    baseName: sourceFile.getBaseName(),
    baseNameWithoutExtension: sourceFile.getBaseNameWithoutExtension(),
    extension: sourceFile.getExtension(),
    scriptKind: sourceFile.getScriptKind(),
    isDeclarationFile: sourceFile.isDeclarationFile(),
    isFromExternalLibrary: sourceFile.isFromExternalLibrary(),
    isInNodeModules: sourceFile.isInNodeModules(),
    fullSource: sourceFile.getFullText(),
    symbols: sourceFile.getLocals().map(visitSymbol),
    statements: sourceFile.getStatements().map(visitNode),
  };
}

export interface Result {
  symbols: Map<SymbolId, Symbol>;
  nodes: Map<NodeId, Node>;
  sourceFiles: Map<string, NodeId>;
  types: Map<TypeId, Type>;
  errors?: string;
}

////////////////////////////////////////////////////////////////////////////////
// --- Traversal functions below
////////////////////////////////////////////////////////////////////////////////
const nodesVisited: Set<NodeId> = new Set();
function resolveNode<A extends Node = Node>(node: mNode): NodeId<A> {
  const nodeId = getNodeId(node);
  if (nodesVisited.has(nodeId)) {
    Debug.log('hasBeenVisited', node, Debug.LogLevel.Debug);
    return nodeId as NodeId<A>;
  }
  nodesVisited.add(nodeId);
  let resolved: Node;
  ///////////////////////////////// //////////
  if (mNode.isSourceFile(node)) {
    resolved = createSourceFile(node);
    result.sourceFiles.set(node.getFilePath() as string, nodeId);
  } else if (mNode.isNamespaceDeclaration(node)) {
    resolved = createNamespaceDeclaration(node);
  } else if (mNode.isTypeAliasDeclaration(node)) {
    resolved = createTypeAliasDeclaration(node);
  } else if (mNode.isIdentifier(node)) {
    resolved = createIdentifier(node);
  } else if (mNode.isStringLiteral(node)) {
    resolved = createStringLiteral(node);
  } else if (mNode.isNumericLiteral(node)) {
    resolved = createNumericLiteral(node);
  } else if (mNode.isModuleBlock(node)) {
    resolved = createModuleBlock(node);
  } else if (mNode.isTypeParameterDeclaration(node)) {
    resolved = createTypeParameterDeclaration(node);
  } else if (mNode.isHeritageClause(node)) {
    resolved = createHeritageClause(node);
  } else if (mNode.isInterfaceDeclaration(node)) {
    resolved = createInterfaceDeclaration(node);
  } else if (mNode.isTypePredicateNode(node)) {
    resolved = createTypePredicateNode(node);
  } else if (mNode.isTypeReferenceNode(node)) {
    resolved = createTypeReferenceNode(node);
  } else if (mNode.isQualifiedName(node)) {
    resolved = createQualifiedName(node);
  } else if (mNode.isExpressionWithTypeArguments(node)) {
    resolved = createExpressionWithTypeArguments(node);

    // TupleTypeNode as mTupleTypeNode,
    // ArrayTypeNode as mArrayTypeNode,
    // TypeLiteralNode as mTypeLiteralNode,
    // TypeElement as mTypeElement,
    // ThisTypeNode as mThisTypeNode,
    // ComputedPropertyName as mComputedPropertyName,
    // Expression as mExpression,
    // PrivateIdentifier as mPrivateIdentifier,
    // ClassExpression as mClassExpression,
    // ClassDeclaration as mClassDeclaration,
    // ClassElement as mClassElement,
    // ClassLikeDeclarationBase as mClassLikeDeclarationBase,
  } else {
    Debug.log(
      `Warning: Type ${node.getKindName()} is not implemented yet`,
      node,
      Debug.LogLevel.Warning
    );
    return nodeId as NodeId<A>;
  }
  ///////////////////////////////////////////
  result.nodes.set(nodeId, resolved);
  return nodeId as NodeId<A>;
}
const typesVisited = new Set();
function resolveType(type_: mType) {
  const typeId = getTypeId(type_);
  if (typesVisited.has(typeId)) {
    return typeId;
  }
  typesVisited.add(typeId);
  ///////////////////////////////////////////
  const resolved = createType(type_);
  ///////////////////////////////////////////
  typesVisited.add(typeId);
  result.types.set(typeId, resolved);
  return typeId;
}
const symbolsVisited = new Set();
function resolveSymbol(symbol: mSymbol) {
  const symbolId = getSymbolId(symbol);
  if (symbolsVisited.has(symbolId)) {
    return symbolId;
  }
  symbolsVisited.add(symbolId);
  ///////////////////////////////////////////
  const resolved = createSymbol(symbol);
  ///////////////////////////////////////////
  result.symbols.set(symbolId, resolved);
  symbolsVisited.add(symbolId);
  return symbolId;
}

let nextSymbolId = 1;
let nextNodeId = 1;
let nextTypeId = 1;
export function getSymbolId(mSymbol: mSymbol): SymbolId {
  const symbol = mSymbol.compilerSymbol;
  // @ts-expect-error
  if (!symbol.id) {
    // @ts-expect-error
    symbol.id = nextSymbolId;
    nextSymbolId++;
  } else {
    // @ts-expect-error
    if (symbol.id > nextSymbolId) {
      // @ts-expect-error
      nextSymbolId = symbol.id + 1;
    }
  }
  // @ts-expect-error
  return symbol.id;
}
export function getNodeId(mNode: mNode): NodeId {
  const node = mNode.compilerNode;
  // @ts-expect-error
  if (!node.id) {
    // @ts-expect-error
    node.id = nextNodeId;
    nextNodeId++;
  } else {
    // @ts-expect-error
    if (node.id > nextNodeId) {
      // @ts-expect-error
      nextNodeId = node.id + 1;
    }
  }
  // @ts-expect-error
  return node.id;
}
export function getTypeId(mType: mType): TypeId {
  const type_ = mType.compilerType;
  // @ts-expect-error
  if (!type_.id) {
    // @ts-expect-error
    type_.id = nextTypeId;
    nextTypeId++;
  } else {
    // @ts-expect-error
    if (type_.id > nextTypeId) {
      // @ts-expect-error
      nextTypeId = type_.id + 1;
    }
  }
  // @ts-expect-error
  return type_.id;
}

const result: Result = {
  symbols: new Map(),
  nodes: new Map(),
  sourceFiles: new Map(),
  types: new Map(),
};
export function resetResult() {
  result.symbols.clear();
  result.nodes.clear();
  result.sourceFiles.clear();
  delete result.errors;
}

function visitNode<A extends Node = Node>(node: mNode): NodeId<A> {
  Debug.log('visit', node, Debug.LogLevel.Debug);
  return resolveNode<A>(node);
}

function visitSymbol(symbol: mSymbol) {
  Debug.log('visit', symbol, Debug.LogLevel.Debug);
  return resolveSymbol(symbol);
}

function visitType(type_: mType) {
  Debug.log('visit', type_, Debug.LogLevel.Debug);
  return resolveType(type_);
}

export function walkNodes(nodes: mNode[]) {
  resetResult();
  symbolsVisited.clear();
  nodesVisited.clear();
  typesVisited.clear();
  nodes.forEach((f) => visitNode(f));
  return result;
}

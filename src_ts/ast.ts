import {
  createWrappedNode,
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
} from 'ts-morph';
import {
  SourceFile,
  Statement,
  Declaration,
  // DeclarationName,
  // DeclarationStatement,
  Identifier,
  IntrinsicType,
  TypeParameterDeclaration,
  // LiteralLikeNode,
  NamedDeclaration,
  Symbol,
  Node,
  Type,
  ModifierableNode,
  ModuleBlock,
  ModuleDeclaration,
  NamespaceBody,
  NamespaceDeclaration,
  // NumericLiteral,
  // PropertyNameLiteral,
  StringLiteral,
  // StringLiteralLike,
} from './ast_types';

let result: Result;

function symbolId(symbol: mSymbol): number {
  // @ts-expect-error
  return symbol.compilerSymbol.id;
}
function maybe<A, B>(map: (v: A | undefined) => B, value: A | undefined) {
  return value != null ? map(value) : undefined;
}
const visitors = new Set();
function hasBeenVisited(id: number) {
  if (visitors.has(id)) {
    return true;
  } else {
    visitors.add(id);
    return false;
  }
}

export function createNode(node: mNode): Node {
  return {
    flags: node.compilerNode.flags,
    kind: node.getKind(),
    kindName: node.getKindName(),
  };
}

export function createSymbol(symbol: mSymbol): Symbol {
  const id = symbolId(symbol);
  if (hasBeenVisited(id)) {
    return { id: id };
  } else {
    const symbol_parsed = {
      flags: symbol.getFlags(),
      name: symbol.getName(),
      escapedName: symbol.getEscapedName(),
      fullyQualifiedName: symbol.getFullyQualifiedName(),
      aliasedSymbol: maybe(createSymbol, symbol.getAliasedSymbol()),
      exportSymbol: maybe(createSymbol, symbol.getExportSymbol()),
      valueDeclaration: maybe(createNode, symbol.getValueDeclaration()),
      declarations: symbol.getDeclarations().map(createNode),
      members: symbol.getMembers().map(createSymbol),
      declaredType: createType(symbol.getDeclaredType()),
      id: symbolId(symbol),
    };
    result.symbols.set(id, symbol_parsed);
    return symbol_parsed;
  }
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
        objectFlags: ts.ObjectFlags,
        intrinsicName: intrinsic,
      }
    : {};
}
export function createType(type: mType): Type | IntrinsicType {
  const base = {
    flags: type.getFlags(),
    symbol: maybe(createSymbol, type.getSymbol()),
    aliasSymbol: maybe(createSymbol, type.getAliasSymbol()),
    aliasTypeArguments: type.getAliasTypeArguments().map(createType),
  };

  return {
    ...base,
    ...maybeAddIntrinsicProps(type),
  };
}

export function createIdentifier(ident: mIdentifier): Identifier {
  return {
    ...createNode(ident),
    kindName: ident.getKindName(),
    escapedText: ident.compilerNode.escapedText.toString(),
    originalKeywordKind: ident.compilerNode.originalKeywordKind,
  };
}

export function createTypeParameterDeclaration(
  typeParam: mTypeParameterDeclaration
): TypeParameterDeclaration {
  return {
    flags: typeParam.compilerNode.flags,
    symbol: maybe(createSymbol, typeParam.getSymbol()),
    kind: typeParam.getKind(),
    kindName: typeParam.getKindName(),
    name: createIdentifier(typeParam.getNameNode()),
    parent: maybe(createNode, typeParam.getParent()),
    constraint: maybe(createNode, typeParam.getConstraint()),
    default: maybe(createNode, typeParam.getDefault()),
  };
}

interface TypeAliasDeclaration extends NamedDeclaration {
  parameters: TypeParameterDeclaration[];
  type: Type | undefined;
  modifiers: Array<ModifierableNode>;
}
export function createTypeAliasDeclaration(
  typeAlias: mTypeAliasDeclaration
): TypeAliasDeclaration {
  return {
    flags: typeAlias.compilerNode.flags,
    kind: typeAlias.getKind(),
    kindName: typeAlias.getKindName(),
    symbol: maybe(createSymbol, typeAlias.getSymbol()),
    name: createIdentifier(typeAlias.getNameNode()),
    parameters: typeAlias
      .getTypeParameters()
      .map(createTypeParameterDeclaration),
    type: maybe(createType, typeAlias.getSymbol()?.getDeclaredType()),
    modifiers: typeAlias.getModifiers().map(createNode),
  };
}

export function createStatement(st: mStatement): Statement {
  return createNode(st);
}

export function createModuleBlock(block: mModuleBlock): ModuleBlock {
  return {
    ...createNode(block),
    parent: maybe(
      createModuleDeclaration,
      block.getParentIfKindOrThrow(ts.SyntaxKind.ModuleDeclaration)
    ),
    statements: block.getStatements().map(createStatement),
  };
}

export function createStringLiteral(str: mStringLiteralLike): StringLiteral {
  return {
    ...createNode(str),
    kindName: str.getKindName(),
    text: str.getText(),
    isUnterminated: str.compilerNode.isUnterminated,
    hasExtendedUnicodeEscape: str.compilerNode.hasExtendedUnicodeEscape,
  };
}

export function createNamespaceBody(body: ts.NamespaceBody) {
  let body_res: NamespaceBody;
  if (body.kind === ts.SyntaxKind.ModuleBlock) {
    body_res = createModuleBlock(createWrappedNode(body));
  } else {
    body_res = createNamespaceDeclaration(createWrappedNode(body));
  }
  return body_res;
}

export function createNameOfIdentifierOrLiteral(
  node: mNode
): Identifier | StringLiteral {
  const node_raw = node.compilerNode;
  if (ts.isIdentifier(node_raw)) {
    createIdentifier(createWrappedNode(node_raw));
  } else if (ts.isStringLiteral(node_raw)) {
    createStringLiteral(createWrappedNode(node_raw));
  }
  throw new Error('Fuck this');
}

export function createModuleDeclaration(
  namespace: mNamespaceDeclaration
): ModuleDeclaration {
  let name = createIdentifier(namespace.getNameNode());
  const body_res = createNamespaceBody(namespace.compilerNode.body);
  let parent = namespace.getParent().compilerNode;
  let parent_res =
    parent.kind === ts.SyntaxKind.ModuleBlock
      ? createModuleBlock(createWrappedNode(parent))
      : parent.kind === ts.SyntaxKind.SourceFile
      ? createSourceFile(createWrappedNode(parent))
      : undefined;

  return {
    ...createNode(namespace),
    symbol: maybe(createSymbol, namespace.getSymbol()),
    parent: parent_res,
    name: name,
    body: body_res,
  };
}

export function createNamespaceDeclaration(
  namespace: mNamespaceDeclaration
): NamespaceDeclaration {
  return {
    ...createModuleDeclaration(namespace),
    name: createIdentifier(namespace.getNameNode()),
    body: createNamespaceBody(namespace.compilerNode.body),
    symbols: namespace.getSymbolsInScope(ts.SymbolFlags.None).map(createSymbol),
  };
}

export function createSourceFile(sourceFile: mSourceFile): SourceFile {
  return {
    flags: sourceFile.compilerNode.flags,
    kind: sourceFile.getKind(),
    kindName: sourceFile.getKindName(),
    filePath: sourceFile.getFilePath(),
    baseName: sourceFile.getBaseName(),
    baseNameWithoutExtension: sourceFile.getBaseNameWithoutExtension(),
    extension: sourceFile.getExtension(),
    scriptKind: sourceFile.getScriptKind(),
    isDeclarationFile: sourceFile.isDeclarationFile(),
    isFromExternalLibrary: sourceFile.isFromExternalLibrary(),
    isInNodeModules: sourceFile.isInNodeModules(),
    fullSource: sourceFile.getFullText(),
    symbols: [], //sourceFile.getLocals().map(createSymbol),
  };
}

export interface Result {
  symbols: Map<number, Symbol>;
  declarations: Map<string, Declaration>;
  sourceFiles: Map<string, SourceFile>;
  errors?: string;
}

export function walkNodes(nodes: mNode[]) {
  result = {
    symbols: new Map(),
    declarations: new Map(),
    sourceFiles: new Map(),
  };
  function walk(node: mNode) {
    if (mNode.isSourceFile(node)) {
      result.sourceFiles.set(
        node.getFilePath() as string,
        createSourceFile(node)
      );
    } else if (mNode.isNamespaceDeclaration(node)) {
      // const path = node.getSymbol()?.getFullyQualifiedName();
      // result.declarations.set(path || '', createNamespaceDeclaration(node));
      // node.forEachChild(walkNode);
    } else if (mNode.isTypeAliasDeclaration(node)) {
      const path = node.getSymbol()?.getFullyQualifiedName();
      result.declarations.set(path || '', createTypeAliasDeclaration(node));
    }
    node.forEachChild(walk);
  }
  nodes.forEach(walk);
  return result;
}

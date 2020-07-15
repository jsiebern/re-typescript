import * as fs from 'fs';
import {
  Type,
  Project,
  Node,
  ts,
  createWrappedNode,
  SourceFile,
  MemberExpression,
} from 'ts-morph';
import * as common from '@ts-morph/common';
import { walkNodes } from './ast';
import { strict, notEqual } from 'assert';
import { SCHED_RR } from 'cluster';

const project = new Project({
  useInMemoryFileSystem: true,
  compilerOptions: {
    noEmitOnError: false,
  },
});
project.createSourceFile;
project.saveSync;
// const created = project.createSourceFile(
//   'foo.d.ts',
//   `
// interface A<Param> {
// x: string;
// y: number;
// z: Param;
// }
// declare namespace XY {
//   type keys = keyof A<boolean> extends "aa" ? string : number;
//   type bla = string;
// }
// export type xx = XY.bla;
// `
// );
// created.saveSync();
ts.createSourceFile;
const created2 = project.createSourceFile(
  'simple.d.ts',
  `
 type opt_in_arr = (string | undefined)[];
`
);
created2.getFilePath();
created2.saveSync();

// type

const diagnostics = project.getPreEmitDiagnostics();
const errors = diagnostics.length
  ? project.formatDiagnosticsWithColorAndContext(diagnostics)
  : undefined;

const c = project.getTypeChecker().compilerObject;
const tc = project.getTypeChecker();

const recurseType = (type: Type | undefined, level = 0) => {
  if (!type /*|| level > 5*/) {
    return;
  }
  console.log(
    '  '.repeat(level),
    't: ' + type.getText() + ' ' + type.getFlags()
  );
  [].forEach((t) => recurseType(t, level + 1));
  type.getTupleElements().forEach((t) => recurseType(t, level + 1));
};
const recurse = (node: Node, level = 0) => {
  if (Node.isTypeAliasDeclaration(node)) {
    node.getName();
    node.getTypeNode();
  }
  if (Node.isEnumDeclaration(node)) {
    const member = node.getMembers()[0];
    member.getName;
    // const x = member.getInitializer()
  }
  if (Node.isUnionTypeNode(node)) {
    node.getTypeNodes;
  }
  if (Node.isTypeReferenceNode(node)) {
    node.getTypeArguments;
    const name = node.getTypeName();
    const typ = node.getType();
  }

  if (node.getKindName() === 'ParenthesizedType') {
    // @ts-ignore^
    // console.log(node.compilerNode.type);
  }

  //  if (Node.isIndexedAccessTypeNode(node)) {
  // console.log(node.getKindName())
  // node.getType().getCallSignatures().forEach(cs => {
  //   console.log("Y")
  //   console.log(cs.getDeclaration().getKindName())
  // })

  // console.log(tc.compilerObject.typeToTypeNode(node.getType().compilerType));
  // @ts-ignore
  //tc.compilerObject.runWithCancellationToken;
  //const cNode = tc.compilerObject.typeToTypeNode(node.getType().compilerType);
  //if (cNode) {
  //const node = createWrappedNode(cNode, {
  //typeChecker: tc.compilerObject,
  //sourceFile: created2.compilerNode,
  //});
  //console.log(node.getKindName());
  //}
  // node.getType().getBaseTypes().forEach(bt => console.log(bt.isString() ? 'string' : 'other'))
  //

  // if (Node.isTypeParameteredNode(node)) {
  //   // console.log(node.getText());
  //   console.log(node.getTypeParameters().map(n => n.getName()));
  // }
  // if (Node.isTupleTypeNode(node)) {
  //   console.log(node);
  // }

  // const type = node.getSymbol()?.getDeclaredType();
  // console.log('  '.repeat(level), node.getKindName() + ':');
  // recurseType(type, level);

  node.getChildren().forEach((n) => recurse(n, level + 1));
};

project.getSourceFiles().forEach((src) => {
  recurse(src);
});

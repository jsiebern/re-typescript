import * as fs from 'fs';
import { Type, Project, Node, ts } from 'ts-morph';
import common from '@ts-morph/common';
import { walkNodes } from './ast';
import { strict } from 'assert';
import { SCHED_RR } from 'cluster';

const project = new Project({
  useInMemoryFileSystem: true,
  compilerOptions: {
    noEmitOnError: false,
  },
});
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
declare function some_other_function(): { inline: string };
`
);
created2.saveSync();

// type

const diagnostics = project.getPreEmitDiagnostics();
const errors = diagnostics.length
  ? project.formatDiagnosticsWithColorAndContext(diagnostics)
  : undefined;

// project.getSourceFiles().forEach((node) => {
//   // @ts-ignore
//   console.log(node.compilerNode.texts);
//   console.log(node.getKind(), Node.isSourceFile(node));
//   console.log(ts.isSourceFile(node.compilerNode));
// });

const c = project.getTypeChecker().compilerObject;

const recurseType = (type: Type | undefined, level = 0) => {
  if (!type /*|| level > 5*/) {
    return;
  }
  console.log(
    '  '.repeat(level),
    't: ' + type.getText() + ' ' + type.getFlags()
  );
  // type.getAliasTypeArguments().forEach((t) => recurseType(t, level + 1));
  [
    // type.getApparentType(),
    // type.getArrayElementType(),
    // type.getBaseTypeOfLiteralType(),
    // type.getConstraint(),
    // type.getDefault(),
    // type.getNonNullableType(),
    // type.getNumberIndexType(),
    // type.getStringIndexType(),
    // type.getTargetType(),
  ].forEach((t) => recurseType(t, level + 1));
  // type.getBaseTypes().forEach((t) => recurseType(t, level + 1));
  // type.getIntersectionTypes().forEach((t) => recurseType(t, level + 1));
  type.getTupleElements().forEach((t) => recurseType(t, level + 1));
  // type.getTypeArguments().forEach((t) => recurseType(t, level + 1));
  // type.getUnionTypes().forEach((t) => recurseType(t, level + 1));
};

const recurse = (node: Node, level = 0) => {
  node.getPos;
  node.getEnd;
  node.getStartLineNumber;
  if (node.getKindName() === 'TypeLiteral') {
    console.log('\n\n\n');
    console.log(node.getKindName());
    console.log(node.getSymbol()?.getDeclaredType().getText());
    console.log('\n\n\n');
    return;
  }

  const type = node.getSymbol()?.getDeclaredType();
  console.log('  '.repeat(level), node.getKindName() + ':');
  recurseType(type, level);

  node.getChildren().forEach((n) => recurse(n, level + 1));
};

project.getSourceFiles().forEach((src) => {
  recurse(src);
});

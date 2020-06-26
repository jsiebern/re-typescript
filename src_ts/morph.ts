import * as fs from 'fs';
import { Project } from 'ts-morph';
import { walkNodes } from './ast';

const project = new Project({
  useInMemoryFileSystem: true,
  compilerOptions: {
    noEmitOnError: false,
  },
});
const created = project.createSourceFile(
  'foo.d.ts',
  `
interface A<Param> {
x: string;
y: number;
z: Param;
} 
declare namespace XY {
  type keys = keyof A<boolean> extends "aa" ? string : number;
  type bla = string;
}
export type xx = XY.bla;
`
);
created.saveSync();
const created2 = project.createSourceFile(
  'simple.d.ts',
  `
type x = {
  y: string,
};

type y = x;
`
);
created2.saveSync();

// type

const diagnostics = project.getPreEmitDiagnostics();
const errors = diagnostics.length
  ? project.formatDiagnosticsWithColorAndContext(diagnostics)
  : undefined;

const result = {
  ...walkNodes(project.getSourceFiles()),
  errors: errors,
};

const printed = JSON.stringify({
  ...result,
  symbols: [...result.symbols],
  nodes: [...result.nodes],
  types: [...result.types],
  sourceFiles: [...result.sourceFiles],
});
fs.writeFileSync(`./test.json`, printed);
// console.log(printed);

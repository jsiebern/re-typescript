import { Project } from 'ts-morph';
import { walkNodes } from './ast';

const project = new Project({ useInMemoryFileSystem: true });
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

// type

const diagnostics = project.getPreEmitDiagnostics();
const errors = diagnostics.length
  ? project.formatDiagnosticsWithColorAndContext(diagnostics)
  : undefined;

const result = {
  ...walkNodes(project.getSourceFiles()),
  errors: errors,
};

// fs.writeFileSync(`./test.json`, JSON.stringify(result));
console.log(
  JSON.stringify({
    ...result,
    symbols: [...result.symbols],
    declarations: [...result.declarations],
    sourceFiles: [...result.sourceFiles],
  })
);

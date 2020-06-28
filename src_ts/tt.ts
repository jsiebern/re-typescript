import ts from 'typescript';

export default class MemoryCompilerHost implements ts.CompilerHost {
  files: { [fileName: string]: string } = {};

  getSourceFile(
    filename: string,
    languageVersion: ts.ScriptTarget,
    onError?: (message: string) => void
  ): ts.SourceFile {
    var text = this.files[filename];
    if (!text) throw new Error();
    return ts.createSourceFile(filename, text, languageVersion);
  }
  getDefaultLibFileName = (options: ts.CompilerOptions) => 'lib.d.ts';
  getDirectories = (path: string): string[] => [];

  writeFile = (
    filename: string,
    data: string,
    writeByteOrderMark: boolean,
    onError?: (message: string) => void
  ) => {};
  getCurrentDirectory = () => '';
  getCanonicalFileName = (fileName: string) => fileName;
  useCaseSensitiveFileNames = () => true;
  getNewLine = () => '\n';
  fileExists = (fileName: string) => !!this.files[fileName];
  readFile = (fileName: string) => this.files[fileName];

  addFile(fileName: string, body: string) {
    this.files[fileName] = body;
  }
}

let program = ts.createProgram({
  rootNames: [],
  options: {},
});

function compile(fileNames: string[], options: ts.CompilerOptions): void {
  let program = ts.createProgram(fileNames, options);
  let emitResult = program.emit();

  let allDiagnostics = ts
    .getPreEmitDiagnostics(program)
    .concat(emitResult.diagnostics);

  allDiagnostics.forEach((diagnostic) => {
    if (diagnostic.file) {
      let { line, character } = diagnostic.file.getLineAndCharacterOfPosition(
        diagnostic.start!
      );
      let message = ts.flattenDiagnosticMessageText(
        diagnostic.messageText,
        '\n'
      );
      console.log(
        `${diagnostic.file.fileName} (${line + 1},${character + 1}): ${message}`
      );
    } else {
      console.log(
        ts.flattenDiagnosticMessageText(diagnostic.messageText, '\n')
      );
    }
  });

  let exitCode = emitResult.emitSkipped ? 1 : 0;
  console.log(`Process exiting with code '${exitCode}'.`);
  process.exit(exitCode);
}

compile(process.argv.slice(2), {
  noEmitOnError: true,
  noImplicitAny: true,
  target: ts.ScriptTarget.ES5,
  module: ts.ModuleKind.CommonJS,
});

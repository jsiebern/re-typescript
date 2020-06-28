open Ts_morph;

let project = Runtime.project;

let sourceFile =
  project->Project.createSourceFile(
    "test.d.ts",
    {|
type bla = {
  a: string;
  b: string;
};
type c = keyof bla;
export type d = ['s1', 's2', string];
|},
  );
sourceFile->SourceFile.saveSync;

let diagnostics =
  project
  ->Project.getPreEmitDiagnostics
  ->Project.formatDiagnosticsWithColorAndContext(project, _);
Js.log(diagnostics);

Preparse.preParse();

project
->Project.getSourceFiles
->Belt.Array.forEach(sourceFile => {
    let node = sourceFile->SourceFile.compilerNodeJson;
    let parsed = Typescript_bs.read_node(node);

    Fs.writeFileSync(
      "src_ts/bs/test.json",
      parsed->Typescript_bs.write_node->Js.Json.stringify,
    );
  });

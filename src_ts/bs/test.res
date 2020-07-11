open Ts_morph;

let project = Runtime.project;

let sourceFile =
  project->Project.createSourceFile(
    "test.d.ts", `
 type variant = 2 | 4 | 8 | 16;
`,
  );
sourceFile->SourceFile.saveSync;

let diagnostics =
  project
  ->Project.getPreEmitDiagnostics
  ->Project.formatDiagnosticsWithColorAndContext(project, _);
Js.log(diagnostics);

Preparse.preParse(Runtime.project);

project
->Project.getSourceFiles
->Belt.Array.forEach(sourceFile => {
    let node = sourceFile->SourceFile.compilerNodeJson;
    let parsed = Typescript_bs.read_node(node);

    Js.log(parsed->Typescript_bs.write_node->Js.Json.stringifyWithSpace(2));
    Fs.writeFileSync(
      "src_ts/bs/test.json",
      parsed->Typescript_bs.write_node->Js.Json.stringifyWithSpace(2),
    );
  });

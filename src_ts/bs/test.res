open Ts_morph;

let project = Runtime.project;

let sourceFile =
  project->Project.createSourceFile(
    "test.d.ts",
    `
type with_param<a = [string, number]> = a;
        type call_params = with_param; 
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

    // Js.log(parsed->Typescript_bs.write_node->Js.Json.stringifyWithSpace(2));
    Fs.writeFileSync(
      "src_ts/bs/test.json",
      parsed->Typescript_bs.write_node->Js.Json.stringifyWithSpace(2),
    );
  });

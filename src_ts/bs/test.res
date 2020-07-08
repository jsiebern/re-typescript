open Ts_morph;

let project = Runtime.project;

let sourceFile =
  project->Project.createSourceFile(
    "test.d.ts",
    `
interface i_1<C, A = string> { field1: A, fieldx: C }
interface i_2<B, A, B> extends i_1<A> { field2: B }
type x = i_2<boolean, string>;
type y = i_1<string>;
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

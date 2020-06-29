open Ts_morph;

[@bs.val] [@bs.scope "process"] external argv: array(string) = "argv";
let files = Belt.Array.sliceToEnd(argv, 2);

let project = Runtime.project;

files
->Belt.Array.map(path_str => {
    let spl = path_str->Js.String.split(":", _);
    (spl->Belt.Array.getExn(0), spl->Belt.Array.getExn(1));
  })
->Belt.Array.forEach(((name, path)) => {
    let src = Fs.readFileSync(path, "utf8");
    let sourceFile = project->Project.createSourceFile(name, src);
    sourceFile->SourceFile.saveSync;
  });

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

    Js.log(parsed->Typescript_bs.write_node->Js.Json.stringify);
  });

open Ts_morph;
open Js_of_ocaml.Js;

// Load Project into global namespace
let require = (s: t(js_string)): 'a =>
  Unsafe.fun_call(Unsafe.js_expr("require"), [|Unsafe.inject(s)|]);

let tsMorph = require("ts-morph" |> string);
Js_of_ocaml.Js.Unsafe.global##.project :=  tsMorph##._Project;

let c =
  Project.Config.make(
    ~useInMemoryFileSystem=true,
    ~compilerOptions=
      Project.Config.CompilerOptions.make(
        ~lib=[|"lib", "lib.es2015.collection.d.ts"|],
        ~noLib=false,
        ~importHelpers=true,
        (),
      ),
    (),
  );
let p = Project.make(c);
let sf = p#createSourceFile("/path/file.d.ts", "type x = string");
sf#saveSync();

let diagnostics = p#getPreEmitDiagnostics();
let diagnosticsResult = p#formatDiagnosticsWithColorAndContext(diagnostics);
Console.log(
  diagnosticsResult != "" ? diagnosticsResult : "> Diagnostics okay",
);

let source_files = [|sf|];

switch (Parser.parse__Entry(~source_files)) {
| [|(_, _, parsed)|] => Console.log(Js_of_ocaml.Json.output(parsed))
| _ => raise(Failure("Unexpected result"))
};

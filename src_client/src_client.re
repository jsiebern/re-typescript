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
let sf =
  p#createSourceFile(
    "/path/file.d.ts",
    {|
  type t_string = string
  type t_any = any
  // This is a comment
/* This is a comment */
|},
  );
sf#saveSync();

let diagnostics = p#getPreEmitDiagnostics();
let diagnosticsResult = p#formatDiagnosticsWithColorAndContext(diagnostics);
Console.log(
  diagnosticsResult != "" ? diagnosticsResult : "> Diagnostics okay",
);

let source_files = [|sf|];

let parsed =
  switch (Parser.parse__Entry(~source_files)) {
  | [|(_, _, parsed)|] =>
    Console.log(Js_of_ocaml.Json.output(parsed));
    [|parsed|];
  | _ => raise(Failure("Unexpected result"))
  };

let (_, structure) = Ast_generator.generate(parsed);
// Console.log("---------------------------------------");
// Console.log(
//   Js_of_ocaml.Json.output(structure)
//   |> to_string
//   |> Yojson.Basic.from_string
//   |> Yojson.Basic.pretty_to_string,
// );

open Migrate_parsetree;

let migrate_ast = ast => {
  let migration =
    Versions.migrate(Versions.ocaml_406, Versions.ocaml_current);

  migration.copy_structure(ast);
};

Reason_toolchain.RE.print_implementation_with_comments(
  Format.str_formatter,
  (migrate_ast(structure), []),
);
let parsed_text = Format.flush_str_formatter();
Console.log("\n\n");
Console.log("---------------------------------------");
Console.log("\n\n");
Console.log(parsed_text);
Console.log("\n\n");

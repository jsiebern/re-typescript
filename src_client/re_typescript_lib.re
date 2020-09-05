open Ts_morph;
open Js_of_ocaml.Js;

type print_language =
  | Reason
  | OCaml
  | ReScript;
type config = Ts_morph.Project.Config.t;
type project = Ts_morph.Project.t;
type source_file = Ts_morph.SourceFile.t;
type file = (string, string);
type node = Ast.Node.node(Ast.Node.Constraint.exactlyModule);
type ast_representation = Migrate_parsetree.Ast_406.Parsetree.structure_item;

// Load Project into global namespace
let require = (s: t(js_string)): 'a =>
  Unsafe.fun_call(Unsafe.js_expr("require"), [|Unsafe.inject(s)|]);

let tsMorph = require("ts-morph" |> string);
Js_of_ocaml.Js.Unsafe.global##.project :=  tsMorph##._Project;

let default_project_config =
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

let create_project = config => Project.make(config);
let create_source_file = ((file_path, file_content), project) => {
  let source_file = project#createSourceFile(file_path, file_content);
  source_file#saveSync();
  source_file;
};
let delete_source_file = (source_file, project) =>
  project#removeSourceFile(source_file);
let create_source_files = (files, project) =>
  files |> CCArray.map(file => project |> create_source_file(file));
let get_diagnostics = project => {
  let diagnostics = project#getPreEmitDiagnostics();
  let diagnosticsResult =
    project#formatDiagnosticsWithColorAndContext(diagnostics);

  diagnosticsResult |> CCString.trim != "" ? Some(diagnosticsResult) : None;
};

let get_generated_ast = nodes => Ast_generator.generate(nodes) |> snd;

let migrate_ast = ast => {
  open Migrate_parsetree;
  let migration =
    Versions.migrate(Versions.ocaml_406, Versions.ocaml_current);
  migration.copy_structure(ast);
};

let print_code = (~print_language=Reason, ast) => {
  let ast = migrate_ast(ast);
  switch (print_language) {
  | Reason =>
    Reason_toolchain.RE.print_implementation_with_comments(
      Format.str_formatter,
      (ast, []),
    )
  | OCaml =>
    Reason_toolchain.ML.print_implementation_with_comments(
      Format.str_formatter,
      (ast, []),
    )
  | ReScript => raise(Failure("ReScript not available for now"))
  };
  Format.flush_str_formatter();
};

let parse_files = files => Parser.parse__Entry(~source_files=files);

let quick_parse =
    (~config=default_project_config, files: array((string, string))) => {
  let project = create_project(config);
  let source_files = project |> create_source_files(files);

  switch (get_diagnostics(project)) {
  | Some(error) => Result.Error(error)
  | None =>
    let nodes_parsed = parse_files(source_files);
    let generated = get_generated_ast(nodes_parsed);
    Result.Ok(print_code(generated));
  };
};

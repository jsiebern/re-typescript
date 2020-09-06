type print_language =
  | Reason
  | OCaml
  | ReScript;
type config = Ts_morph.Project.Config.t;
type project = Ts_morph.Project.t;
type source_file = Ts_morph.SourceFile.t;
type file = (string, string);
type node = Ast.Node.node(Ast.Node.Constraint.exactlyModule);
type ast_representation;

let default_project_config: config;
let create_project: config => project;
let create_source_file: (file, project) => source_file;
let delete_source_file: (source_file, project) => unit;
let create_source_files: (array(file), project) => array(source_file);
let get_diagnostics: project => option(string);
let get_generated_ast:
  (~print_language: print_language=?, array(node)) =>
  list(ast_representation);
let print_code:
  (~print_language: print_language=?, list(ast_representation)) => string;
let parse_files: array(source_file) => array(node);

let quick_parse:
  (~config: Ts_morph.Project.Config.t=?, array((string, string))) =>
  Result.t(string, string);

open Re_typescript_base;
open Re_typescript_config;
open Re_typescript_fs;
open Js_of_ocaml;

Pastel.setMode(HumanReadable);

Example_files.register_examples();

Js.export("run", Run.run);
Js.export("example_list", Example_files.examples_list);

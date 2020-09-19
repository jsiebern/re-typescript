include Rely.Make({
  let config =
    Rely.TestFrameworkConfig.initialize({
      snapshotDir: "tests/__snapshots__",
      projectDir: "src",
    });
});

module Lib = Re_typescript_lib;
let project = Lib.create_project(Lib.default_project_config);

type rndfun = unit => float;
let rnd: rndfun = Js_of_ocaml.Js.Unsafe.global##._Math##.random;
let file_path = Printf.sprintf("/%f/test.d.ts", rnd());
let file = project#createSourceFile(file_path, "");
file#saveSync();
let print = (~ctx=?, value) => {
  Console.log("--> STARTING");
  file#replaceWithText(value)#saveSync();

  switch (Lib.get_diagnostics(project)) {
  | Some(err) => raise(Failure(err))
  | None =>
    let nodes_parsed = Lib.parse_files(~warnings=false, [|file|]);
    let generated = Lib.get_generated_ast(nodes_parsed);
    Lib.print_code(generated);
  };
};

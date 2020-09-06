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
let print = (~ctx=?, value) => {
  let file_path = Printf.sprintf("/%f/test.d.ts", rnd());

  let result =
    Lib.quick_parse(
      ~config=ctx |> CCOpt.value(~default=Lib.default_project_config),
      [|(file_path, value)|],
    );
  switch (result) {
  | Error(err) => raise(Failure(err))
  | Ok(code) => code
  };
  // let source_file = project#createSourceFile(file_path, value);
  // source_file#saveSync();
  // let nodes = Lib.parse_files([|source_file|]);
  // let ast = Lib.get_generated_ast(nodes);
  // Lib.print_code(ast);
};

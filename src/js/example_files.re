open Js_of_ocaml;

module Bridge = Re_typescript_config.Bridge;
let examples = Examples.examples;

// -----------------------------------------------------------------------------

let register_examples = () => {
  let rec walk = (~path=Fs.base_path, folders_or_files) => {
    folders_or_files
    |> CCList.iter(
         fun
         | Examples.Folder(name, children) =>
           walk(~path=Fp.append(path, name), children)
         | File(name, content) => {
             Js_of_ocaml.Sys_js.create_file(
               ~name=Fp.append(path, name) |> Fp.toString,
               ~content,
             );
           },
       );
  };
  walk(examples);
};

module File = CCIO.File;

let rec example_tree = (~exclude=[], path) => {
  Fs.list_of_dir(path)
  |> CCList.filter_map(sub_path => {
       let sub_path_str = sub_path |> Fp.toString;
       let base_name = Fp.baseName(sub_path) |> CCOpt.get_exn;
       if (exclude |> CCList.exists(CCEqual.string(base_name))) {
         None;
       } else {
         Some(
           File.is_directory(sub_path_str)
             ? Bridge.Folder((base_name, example_tree(sub_path)))
             : Bridge.File(base_name),
         );
       };
     });
};

let examples_list = () =>
  Bridge.string_of_example_list(
    try({
      let entries = Fs.list_of_dir(Fs.base_path);
      entries
      |> CCList.map(example_path => {
           let example_path_str = example_path |> Fp.toString;
           {
             Bridge.path: example_path_str,
             meta:
               Bridge.example_meta_of_string(
                 Sys_js.read_file(
                   ~name=Fp.append(example_path, "meta.json") |> Fp.toString,
                 ),
               ),
             files: example_tree(~exclude=["meta.json"], example_path),
           };
         });
    }) {
    | e =>
      Console.error(e);
      [];
    },
  );

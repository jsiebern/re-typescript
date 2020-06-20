module Fp = File_path;
type path = Fp.t(Fp.absolute);

type config = {
  tsconfig: option(path),
  loader: (module Loader.T),
};
module type Config = {let config: config;};

module type T = {
  let config: config;
  let resolve:
    (~current_file: path, string) => CCResult.t((string, path), string);
};

let add_dts = p => Fp.append_extension(p, ".d.ts");
let is_relative = module_specifier =>
  CCString.prefix(~pre="./", module_specifier)
  || CCString.prefix(~pre="../", module_specifier);
let resolve_package_json_types = (~loader, current_dir: path) => {
  module Loader = (val loader: Loader.T);

  let path = Fp.append(current_dir, "package.json");
  switch (Loader.file_read(path)) {
  | Error(_) as e => e
  | Ok(content) =>
    try({
      let types_relative =
        Package_json_j.package_json_of_string(content).types
        |> CCOpt.flat_map(Fp.relative);
      switch (types_relative) {
      | None => Ok(None)
      | Some(types) => Ok(Some(Fp.join(current_dir, types)))
      };
    }) {
    | e => Error(Printexc.to_string(e))
    }
  };
};

let rec generate_check_tree =
        (~acc=[], ~loader, ~current_dir, module_specifier) => {
  let node_modules = Fp.append(current_dir, "node_modules");
  let node_modules_sub = Fp.append(node_modules, module_specifier);
  let node_modules_types = Fp.append(node_modules, "@types");
  let tree = acc @ [Fp.append(node_modules, module_specifier) |> add_dts];
  let tree =
    switch (resolve_package_json_types(~loader, node_modules_sub)) {
    | Ok(None)
    | Error(_) => tree
    | Ok(Some(path)) => tree @ [path]
    };
  let tree =
    tree
    @ [
      Fp.append(node_modules_types, module_specifier) |> add_dts,
      Fp.append(node_modules_sub, "index.d.ts"),
    ];
  let new_dir = Fp.At.(current_dir /../ "");
  if (Fp.eq(current_dir, new_dir)) {
    tree;
  } else {
    generate_check_tree(
      ~acc=tree,
      ~loader,
      ~current_dir=new_dir,
      module_specifier,
    );
  };
};

module Make = (C: Config) : T => {
  let config = C.config;
  let resolve =
      (~current_file: path, module_specifier: string)
      : CCResult.t((string, path), string) => {
    module Loader = (val config.loader: Loader.T);
    let current_dir = current_file |> Fp.dirName;

    let as_relative =
      is_relative(module_specifier) ? Fp.relative(module_specifier) : None;
    let paths_to_check =
      switch (as_relative) {
      | None =>
        generate_check_tree(
          ~loader=config.loader,
          ~current_dir,
          module_specifier,
        )
      | Some(module_path) =>
        let paths_to_check = [Fp.join(current_dir, module_path) |> add_dts];
        let paths_to_check =
          switch (
            resolve_package_json_types(~loader=config.loader, current_dir)
          ) {
          | Ok(None)
          | Error(_) => paths_to_check
          | Ok(Some(types_path)) => paths_to_check @ [types_path]
          };
        paths_to_check
        @ [
          Fp.join(current_dir, module_path)
          |> Fp.append(_, "index")
          |> add_dts,
        ];
      };

    switch (
      paths_to_check
      |> CCList.find_opt(file_path => Loader.file_exists(file_path))
    ) {
    | None =>
      Error(
        Printf.sprintf("Could not resolve a file for: %s", module_specifier),
      )
    | Some(file_path) =>
      Loader.file_read(file_path) |> CCResult.map(res => (res, file_path))
    };
  };
};

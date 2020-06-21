open Js_of_ocaml;

type path = Fp.t(Fp.absolute);
let base_path = Fp.absoluteExn("/examples");

module Loader: Re_typescript_fs.Loader.T = {
  let fs_name = "Js_of_ocaml";
  let file_read = path =>
    try(Ok(Sys_js.read_file(~name=path |> Fp.toString))) {
    | e => Error(Printexc.to_string(e))
    };
  let file_exists = path => CCIO.File.exists(path |> Fp.toString);
};

module File = CCIO.File;
let list_of_dir = (~recurse=false, path) => {
  let base = path |> Fp.toString;
  let get_entry = File.read_dir(~recurse, base);
  let rec walk = res => {
    switch (get_entry()) {
    | None => res
    | Some(entry) =>
      let entry =
        recurse ? Fp.absoluteExn(entry) : Fp.append(base_path, entry);
      walk(res @ [entry]);
    };
  };
  walk([]);
};

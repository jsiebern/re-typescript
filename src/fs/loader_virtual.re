module type Config = {let tbl: Hashtbl.t(Fp.t(Fp.absolute), string);};
module Make = (C: Config) : Loader.T => {
  let fs_name = "Virtual File System";

  let keys = CCHashtbl.keys_list(C.tbl);
  let file_read = (path: Fp.t(Fp.absolute)) =>
    switch (Hashtbl.find_opt(C.tbl, path)) {
    | Some(f) => Ok(f)
    | None => Error("File does not exist")
    };
  let file_exists = path => keys |> CCList.exists(Fp.eq(path));
};

module type T = {let lookup_file: string => CCResult.t(string, string);};

module type Loader_hashtbl_value = {let tbl: Hashtbl.t(string, string);};
module Resolver_hashtbl: (Loader_hashtbl_value) => T =
  (Value: Loader_hashtbl_value) => {
    let lookup_file = name =>
      switch (Hashtbl.find_opt(Value.tbl, name)) {
      | Some(f) => Ok(f)
      | None => Error("File does not exist")
      };
  };

module Loader_fs: T = {
  let lookup_file = path => {
    let file_path = Filename.concat(Sys.getcwd(), path);
    CCIO.File.exists(file_path)
      ? CCIO.File.read(file_path) : Error("File does not exist");
  };
};

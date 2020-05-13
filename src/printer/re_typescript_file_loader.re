module type T = {let lookup_file: string => option(string);};

module type Loader_hashtbl_value = {let tbl: Hashtbl.t(string, string);};
module Resolver_hashtbl: (Loader_hashtbl_value) => T =
  (Value: Loader_hashtbl_value) => {
    let lookup_file = name => Hashtbl.find_opt(Value.tbl, name);
  };

module Loader_fs: T = {
  let lookup_file = path => {
    let file_path = Filename.concat(BatSys.getcwd(), path);
    BatSys.file_exists(file_path)
      ? try(Some(BatPervasives.input_file(file_path))) {
        | _ => None
        }
      : None;
  };
};

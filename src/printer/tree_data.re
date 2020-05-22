open Re_typescript_base;
open Tree_types;
open Tree_utils;

module Type = {
  type t = Hashtbl.t(Path.t, ts_type);
  let map: t = Hashtbl.create(0);
  let add = (~path, type_) => Hashtbl.add(map, path, type_);
  let get = (~path) => Hashtbl.find_opt(map, path);
  let replace = (~path, type_) => Hashtbl.replace(map, path, type_);

  let order = ref([]);
  let add_order = (path: Path.t) => {
    order := order^ @ [path];
  };
  let clear = () => {
    Hashtbl.clear(map);
    order := [];
  };
};
module Ref = {
  type t = Hashtbl.t(Path.t, Path.t);
  let map: t = Hashtbl.create(0);
  let add = (~from: Path.t, ~to_: Path.t) => Hashtbl.add(map, to_, from);
  let get = (to_: Path.t) => Hashtbl.find_opt(map, to_);
  let get_all = (to_: Path.t) => Hashtbl.find_all(map, to_);
  let clear = () => {
    Hashtbl.clear(map);
  };

  let resolve_ref =
      (~remember=true, ~from: Path.t, lookup: Path.t): option(Path.t) => {
    let scope = from |> Path.to_scope;
    switch (lookup) {
    | ([], _) => None
    | ([_] as one, sub) =>
      let path = (scope @ one, sub);

      if (remember) {
        add(~from, ~to_=path);
      };
      if (Path.eq(from, path)) {
        Some(path);
      } else {
        Type.get(~path) |> CCOpt.map(_ => path);
      };
    | _ => raise(Exceptions.Parser_error("PATH NOT IMPLEMENTED"))
    };
  };
};

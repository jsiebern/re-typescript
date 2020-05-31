open Re_typescript_base;
open Tree_types;
open Tree_utils;

let no_pi: Ts.with_pi('a) => 'a = ({item, _}) => item;

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

  let extract_parameters = (~path) =>
    switch (get(~path)) {
    | Some(TypeDeclaration({td_parameters, _})) => Some(td_parameters)
    | _ => None
    };
};
module Arguments = {
  type t = Hashtbl.t(Path.t, list(ts_identifier));
  let map: t = Hashtbl.create(0);
  let add = (~path, arg: ts_identifier) =>
    CCHashtbl.add_list(map, path, arg);
  let get = (~path) => CCHashtbl.get_or(map, path, ~default=[]);
};
module Parameters = {
  type t = Hashtbl.t(list(string), list(ts_type_parameter));
  let map: t = Hashtbl.create(0);
  let get = (~path) => Hashtbl.find_opt(map, path);
  let add = (~path, params) =>
    switch (params) {
    | [] => ()
    | params => Hashtbl.add(map, path, params)
    };
  let has_parameter = (~path, ~param: ts_identifier) =>
    switch (get(~path)) {
    | Some(args) =>
      args
      |> CCList.find_opt(({tp_name, _}) =>
           CCEqual.string(tp_name |> Ident.value, param |> Ident.value)
         )
    | None => None
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

  let rec resolve_ref =
          (~recursive=false, ~remember=true, ~from: Path.t, lookup: Path.t)
          : option(Path.t) => {
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
        switch (Type.get(~path)) {
        | None => None
        | Some(
            TypeDeclaration({
              td_type: Reference({tr_path_resolved: Some(path_resolved), _}),
              _,
            }),
          )
            when CCEqual.bool(recursive, true) =>
          resolve_ref(~recursive=true, ~remember, ~from, path_resolved)
        | Some(_) => Type.get(~path) |> CCOpt.map(_ => path)
        };
      };
    | _ => raise(Exceptions.Parser_error("PATH NOT IMPLEMENTED"))
    };
  };
};

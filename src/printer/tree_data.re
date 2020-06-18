open Re_typescript_base;
open Tree_types;
open Tree_utils;

let no_pi: Ts.with_pi('a) => 'a = ({item, _}) => item;
let to_pi: 'a => Ts.with_pi('a) =
  item => {item, pi: Re_typescript_base.Parse_info.zero};

module Declarations = {
  type t = Hashtbl.t(Path.t, (Ts.declaration, bool));
  let map: t = Hashtbl.create(0);
  let get = (~path) => Hashtbl.find_opt(map, path);
  let has = (~path) => get(~path) |> CCOpt.is_some;
  let is_complete = (~path) =>
    get(~path) |> CCOpt.map_or(~default=false, snd);
  let set_complete = (~path) =>
    CCHashtbl.update(map, ~k=path, ~f=(_, item) =>
      item |> CCOpt.map(((t, _)) => (t, true))
    );
  let get_type_declaration = (~path) =>
    get(~path) |> CCOpt.map(fst) |> CCOpt.get_exn;

  let add_list = (~path, lst) =>
    lst
    |> CCList.iter((dec: Ts.declaration) =>
         switch (dec, dec |> path_of_declaration(~path)) {
         | (d, Some(path)) => Hashtbl.add(map, path, (d, false))
         | _ => ()
         }
       );
};
module Type = {
  type t = Hashtbl.t(Path.t, ts_type);
  let map: t = Hashtbl.create(0);
  let add = (~path, type_) => Hashtbl.add(map, path, type_);
  let get = (~path) => Hashtbl.find_opt(map, path);
  let replace = (~path, type_) => Hashtbl.replace(map, path, type_);
  let remove = (~path) => Hashtbl.remove(map, path);

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
  let clear = () => Hashtbl.clear(map);
};
module Parameters = {
  type t =
    | Fixed(ts_type_parameter)
    | FromRoot(ts_type_parameter)
    | FromChild(ts_type_parameter);
  type map = Hashtbl.t(Path.t, list(t));
  let map: map = Hashtbl.create(0);
  let clear = () => Hashtbl.clear(map);

  // --------------------------------
  // `wrap` / `unwrap` functions
  // --------------------------------
  let param_of_t =
    fun
    | Fixed(p)
    | FromRoot(p)
    | FromChild(p) => p;
  let ident_of_param = p => p.tp_name;
  let ident_of_t = p => param_of_t(p).tp_name;
  let wrap_fixed = p => Fixed(p);
  let wrap_root = p => FromRoot(p);
  let wrap_child = p => FromChild(p);
  let eq = (a, b) =>
    Ident.eq(param_of_t(a).tp_name, param_of_t(b).tp_name);

  // --------------------------------
  // `get` functions
  // --------------------------------
  let get = (~path) => CCHashtbl.get_or(~default=[], map, path);
  let get_root = (~path) =>
    CCHashtbl.get_or(~default=[], map, (fst(path), []));
  let get_unpacked = (~path) => get(~path) |> CCList.map(param_of_t);
  let get_root_unpacked = (~path) => get_unpacked(~path=(fst(path), []));
  let get_fixed = (~path) =>
    get(~path)
    |> CCList.filter_map(
         fun
         | Fixed(p) => Some(p)
         | FromRoot(_)
         | FromChild(_) => None,
       );
  let get_root_fixed = (~path) => get_fixed(~path=(fst(path), []));
  let get_inherited = (~path) =>
    get(~path)
    |> CCList.filter_map(
         fun
         | Fixed(_) => None
         | FromRoot(p)
         | FromChild(p) => Some(p),
       );
  let get_root_inherited = (~path) => get_inherited(~path=(fst(path), []));

  // --------------------------------
  // `add` functions
  // --------------------------------
  let add = (~path, param) =>
    Hashtbl.replace(map, path, get(~path) @ [param]);
  let add_list = (~path, params: list(t)) =>
    Hashtbl.replace(map, path, get(~path) @ params);
  let add_list_fixed = (~path, params: list(ts_type_parameter)) =>
    add_list(~path, params |> CCList.map(wrap_fixed));
  let add_list_no_duplicates = (~path, params: list(t)) => {
    let existing = get(~path);
    Hashtbl.replace(
      map,
      path,
      existing
      @ CCList.filter(param => !CCList.mem(~eq, param, existing), params),
    );
  };

  // --------------------------------
  // `has` functions
  // --------------------------------
  let has = (~fixed_only=true, ~path, param: ts_identifier) =>
    (fixed_only ? get_fixed(~path) : get(~path) |> CCList.map(param_of_t))
    |> CCList.map(p => p.tp_name)
    |> CCList.find_opt(Ident.eq(param))
    |> CCOpt.is_some;
  let has_root = (~fixed_only=true, ~path, param: ts_identifier) =>
    has(~fixed_only, ~path=(fst(path), []), param);

  // --------------------------------
  // `spread` / `hoist` functions
  // --------------------------------
  let rec spread_to_root = (~path, params: list(ts_type_parameter)) =>
    switch (path, params) {
    | ((_, []), _)
    | (_, []) => ()
    | (path, params) =>
      let new_path = path |> Path.cut_sub;
      add_list_no_duplicates(
        ~path=new_path,
        params |> CCList.map(wrap_child),
      );
      spread_to_root(~path=new_path, params);
    };
  let rec hoist_from_root =
          (~sub_on=1, ~path, params: list(ts_type_parameter)) =>
    switch (path, params) {
    | ((_, []), _)
    | (_, []) => ()
    | ((_, p_sub), _) when CCList.length(p_sub) <= sub_on => ()
    | ((p_root, p_sub), params) =>
      let new_path = (p_root, p_sub |> CCList.take(sub_on));
      add_list_no_duplicates(
        ~path=new_path,
        params |> CCList.map(wrap_child),
      );
      hoist_from_root(~sub_on=sub_on + 1, ~path, params);
    };
  let hoist_from_root_by_ident = (~path, ident) => {
    get_root_unpacked(~path)
    |> CCList.filter(({tp_name, _}) => Ident.eq(tp_name, ident))
    |> hoist_from_root(~path);
  };
};
module Imports = {
  type t = {
    bindings: list(string),
    from: string,
    parsed: bool,
  };
  type map = Hashtbl.t(Path.t, list(t));
  let map: map = Hashtbl.create(0);

  let get = (~path) => CCHashtbl.get_or(~default=[], map, path);
  let find = (~path, binding) =>
    get(~path)
    |> CCList.find_opt(i => i.bindings |> CCList.exists(b => b == binding));

  let add = (~path, import: t) => CCHashtbl.add_list(map, path, import);
  let add_binding = (~path, ~from, binding) => {
    add(~path, {bindings: [binding], from, parsed: false});
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
    let from_scope = from |> Path.to_scope;
    switch (lookup) {
    | ([], _) => None
    | ([_] as one, sub) =>
      let path = (from_scope @ one, sub);

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
    | (p, sub) =>
      let path = (from_scope @ p, sub);
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
    };
  };
};

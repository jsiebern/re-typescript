open Re_typescript_base;
open Decode_result;
open Decode_config;
open Decode_utils;

exception Decode_Error(string);
let types: ref(list(Ts.type_def)) = ref([]);
let record_cache: Hashtbl.t(string, Decode_result.type_def) =
  Hashtbl.create(0);
let injected: Hashtbl.t(string, Decode_result.type_def) = Hashtbl.create(0);
let record_referenced = ref([]);
let has_been_referenced = name =>
  record_referenced^
  |> List.find_opt(x => x == name)
  |> Tablecloth.Option.is_some;

let rec decode = (~ctx: config=defaultConfig, toplevel: Ts.toplevel) => {
  Hashtbl.clear(injected);
  types := toplevel.types |> List.map(fst);
  record_referenced := [];
  let types = toplevel.types |> List.map(decode_type_def);

  types
  |> Tablecloth.List.append(
       Hashtbl.fold((_, v, p) => [v, ...p], injected, [])
       |> Tablecloth.List.reverse
       |> Tablecloth.List.map(
            ~f=
              fun
              | TypeDeclaration(_) as type_dec => type_dec
              | _ =>
                raise(Decode_Error("Only TypeDeclarations can be injected")),
          )
       |> Tablecloth.List.reverse,
     )
  |> Tablecloth.List.filter_map(
       ~f=
         fun
         | TypeDeclaration((name, _) as n, Record([]))
             when has_been_referenced(name) =>
           Some(TypeDeclaration(n, Base(Any)))
         | TypeDeclaration(_, Record([])) => None
         | v => Some(v),
     );
}
and decode_type_def: ((Ts.type_def, bool)) => type_def =
  fun
  | (`TypeDef(name, `Obj(fields)), _) => {
      let name = name |> to_valid_typename;
      let record =
        Record(fields |> List.map(decode_obj_field(~parent_name=name)));
      Hashtbl.add(record_cache, fst(name), record);
      TypeDeclaration(name, record);
    }
  | (`InterfaceDef(name, extends_ref, fields), _) => {
      let name = name |> to_valid_typename;
      let record =
        Record(
          fields
          |> Tablecloth.List.map(~f=decode_obj_field(~parent_name=name))
          |> Tablecloth.List.append(decode_extends_ref(extends_ref))
          |> Tablecloth.List.reverse
          |> Tablecloth.List.uniqueBy(
               ~f=
                 fun
                 | RecordField((a, _), _, _) => a
                 | _ => "",
             ),
        );
      Hashtbl.add(record_cache, fst(name), record);
      TypeDeclaration(name, record);
    }
  | (`TypeDef(name, type_), _) => {
      let name = name |> to_valid_typename;
      TypeDeclaration(name, decode_type(~parent_name=name, type_));
    }
and decode_type: (~parent_name: (string, string), Ts.type_) => type_def =
  (~parent_name) =>
    fun
    | `String => Base(String)
    | `Number => Base(Number)
    | `Boolean => Base(Boolean)
    | `Void => Base(Void)
    | `Any => Base(Any)
    | `Array(type_) => decode_array(~parent_name, type_)
    | `Tuple(types) => decode_tuple(~parent_name, types)
    | `Ref(ref_) => Base(Ref(ref_ |> decode_ref_type_name))
    | `Enum(_)
    | `Union(_) => raise(Decode_Error("Not yet implemented"))
    | `Undefined =>
      raise(Decode_Error("Undefined cannot exist outside of a union"))
    | `Null => raise(Decode_Error("Null cannot exist outside of a union"))
    | `Obj(_) =>
      raise(Decode_Error("Obj should never be reached in this switch"))
    | `TypeExtract(ref_, names) => decode_type_extract(ref_, names)
  and decode_array = (~parent_name, type_) => {
    Array(switch (type_) {
      | `Obj(fields) => decode_inline_record(~parent_name, ~key="t",~fields)
      | t => decode_type(~parent_name, t)
    })
  }
and decode_tuple = (~parent_name, types) => {
  Tuple(
    types
    |> Tablecloth.List.mapi(~f=i =>
         fun
         | `Obj(fields) =>
           decode_inline_record(
             ~parent_name,
             ~key=i + 1 |> string_of_int,
             ~fields,
           )
         | t => decode_type(~parent_name, t)
       ),
  );
}
and decode_inline_record = (~parent_name, ~key, ~fields) => {
  let sub_type_name =
    Printf.sprintf("%s_%s", fst(parent_name), key |> to_valid_ident |> fst);
  let record =
    Record(
      fields
      |> Tablecloth.List.map(
           ~f=decode_obj_field(~parent_name=(sub_type_name, sub_type_name)),
         ),
    );
  Hashtbl.add(
    injected,
    sub_type_name,
    TypeDeclaration((sub_type_name, sub_type_name), record),
  );
  Hashtbl.add(record_cache, sub_type_name, record);
  Base(Ref((sub_type_name, sub_type_name)));
}
and decode_obj_field = (~parent_name) =>
  fun
  | {key, type_: `Obj(fields), optional, readonly} => {
      let t_ref = decode_inline_record(~parent_name, ~key, ~fields);
      RecordField(
        key |> to_valid_ident,
        optional ? Optional(t_ref) : t_ref,
        readonly,
      );
    }
  | {key, optional: true, readonly, type_} => {
      RecordField(
        key |> to_valid_ident,
        Optional(type_ |> decode_type(~parent_name)),
        readonly,
      );
    }
  | {key, optional: false, readonly, type_} =>
    RecordField(
      key |> to_valid_ident,
      type_ |> decode_type(~parent_name),
      readonly,
    )
and decode_type_extract = (ref_: Ts.ref_, fields: list(string)) => {
  let fields_in_type = decode_extends_ref(ref_);
  let find_field = name =>
    fields_in_type
    |> Tablecloth.List.find(
         ~f=
           fun
           | RecordField((n, _), _, _) when n == name => true
           | _ => false,
       );
  switch (fields) {
  | [] => raise(Not_found)
  | [name] =>
    switch (find_field(name |> to_valid_typename |> fst)) {
    | Some(RecordField(_, type_, _)) => type_
    | Some(_)
    | None =>
      // TODO: Decide wether to to just return any here instead of an error
      Console.error(fields);
      raise(
        Decode_Error(
          Printf.sprintf(
            "Could not find field %s in %s",
            name,
            decode_ref_type_name(ref_) |> snd,
          ),
        ),
      );
    }
  | [name, ...rest] =>
    switch (find_field(name |> to_valid_typename |> fst)) {
    | Some(RecordField(_, Base(Ref((ref_name, _))), _)) =>
      decode_type_extract([(ref_name, [])], rest)
    | Some(_)
    | None =>
      // TODO: Decide wether to to just return any here instead of an error
      Console.error(fields);
      raise(
        Decode_Error(
          Printf.sprintf(
            "Could not find field %s in %s",
            name,
            decode_ref_type_name(ref_) |> snd,
          ),
        ),
      );
    }
  };
}
and decode_extends_ref = (ref_: Ts.ref_) => {
  // Only implement this naively for now
  let lookup_name = fst(decode_ref_type_name(ref_));
  Tablecloth.Option.(
    Hashtbl.find_opt(record_cache, lookup_name)
    |> map(
         ~f=
           fun
           | Record(fields) => fields
           | _ => [],
       )
    |> with_default(~default=[])
  );
}
and decode_ref_type_name = (ref_: Ts.ref_): (string, string) => {
  let idents =
    ref_
    |> Tablecloth.List.map(~f=fst)
    |> Tablecloth.List.map(~f=v => ["_", v])
    |> Tablecloth.List.concat;
  let ref_resolved =
    (
      switch (idents) {
      | [] => ""
      | [_, ...rest] =>
        rest |> Tablecloth.List.fold_left(~initial="", ~f=(p, e) => p ++ e)
      }
    )
    |> to_valid_typename;
  record_referenced := [ref_resolved |> fst, ...record_referenced^];
  ref_resolved;
};

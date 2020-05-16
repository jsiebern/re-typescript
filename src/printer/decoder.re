open Re_typescript_base;
open Decode_result;
open Decode_config;
open Decode_utils;

exception Decode_Error(string);
let types: ref(list(Ts.type_def)) = ref([]);
let record_cache: Hashtbl.t(string, Decode_result.type_def) =
  Hashtbl.create(0);
let injected: Hashtbl.t(string, Decode_result.type_def) = Hashtbl.create(0);

let rec decode = (~ctx: config=defaultConfig, toplevel: Ts.toplevel) => {
  Hashtbl.clear(injected);
  types := toplevel.types |> List.map(fst);
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
     );
}
and decode_type_def: ((Ts.type_def, bool)) => type_def =
  fun
  | (`TypeDef(name, `Obj(fields)), _) => {
      let name = name |> to_valid_typename;
      let record =
        Record(fields |> List.map(decode_obj_field(~obj_name=name)));
      Hashtbl.add(record_cache, fst(name), record);
      TypeDeclaration(name, record);
    }
  | (`InterfaceDef(name, extends_ref, fields), _) => {
      let name = name |> to_valid_typename;
      let record =
        Record(
          fields
          |> Tablecloth.List.map(~f=decode_obj_field(~obj_name=name))
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
  | (`TypeDef(name, type_), _) =>
    TypeDeclaration(name |> to_valid_typename, decode_type(type_))
and decode_type: Ts.type_ => type_def =
  fun
  | `String => Base(String)
  | `Number => Base(Number)
  | `Boolean => Base(Boolean)
  | `Void => Base(Void)
  | `Any => Base(Any)
  | `Array(type_) => Array(decode_type(type_))
  | `Tuple(types) => Tuple(types |> List.map(decode_type))
  | `Ref(ref_) => Base(Ref(ref_ |> decode_ref_type_name))
  | `Enum(_)
  | `Union(_) => raise(Decode_Error("Not yet implemented"))
  | `Undefined =>
    raise(Decode_Error("Undefined cannot exist outside of a union"))
  | `Null => raise(Decode_Error("Null cannot exist outside of a union"))
  | `Obj(_) =>
    raise(Decode_Error("Obj should never be reached in this switch"))
  | `TypeExtract(ref_, names) => decode_type_extract(ref_, names)
and decode_obj_field = (~obj_name) =>
  fun
  | {key, type_: `Obj(fields), _} as field => {
      let sub_type_name =
        Printf.sprintf("%s_%s", fst(obj_name), key |> to_valid_ident |> fst);
      let record =
        Record(
          fields
          |> Tablecloth.List.map(
               ~f=decode_obj_field(~obj_name=(sub_type_name, sub_type_name)),
             ),
        );
      Hashtbl.add(
        injected,
        sub_type_name,
        TypeDeclaration((sub_type_name, sub_type_name), record),
      );
      Hashtbl.add(record_cache, sub_type_name, record);
      decode_obj_field(
        ~obj_name,
        {...field, type_: `Ref([(sub_type_name, [])])},
      );
    }
  | {key, optional: true, readonly, type_} => {
      RecordField(
        key |> to_valid_ident,
        Optional(type_ |> decode_type),
        readonly,
      );
    }
  | {key, optional: false, readonly, type_} =>
    RecordField(key |> to_valid_ident, type_ |> decode_type, readonly)
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
  (
    switch (idents) {
    | [] => ""
    | [_, ...rest] =>
      rest |> Tablecloth.List.fold_left(~initial="", ~f=(p, e) => p ++ e)
    }
  )
  |> to_valid_typename;
};

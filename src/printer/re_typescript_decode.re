open Re_typescript_base;
open Re_typescript_decode_result;
open Re_typescript_decode_config;
open Re_typescript_decode_utils;

exception Re_Typescript_Decode_Error(string);

let types: ref(list(Ts.type_def)) = ref([]);
let record_cache: Hashtbl.t(string, Re_typescript_decode_result.type_def) =
  Hashtbl.create(0);

let rec decode = (~ctx: config=defaultConfig, toplevel: Ts.toplevel) => {
  types := toplevel.types |> List.map(fst);
  toplevel.types |> List.map(decode_type_def);
}
and decode_type_def: ((Ts.type_def, bool)) => type_def =
  fun
  | (`TypeDef(name, `Obj(fields)), _) => {
      let name = name |> to_valid_typename;
      let record = Record(fields |> List.map(decode_obj_field));
      Hashtbl.add(record_cache, fst(name), record);
      TypeDeclaration(name, record);
    }
  | (`InterfaceDef(name, extends_ref, fields), _) => {
      let name = name |> to_valid_typename;
      let record =
        Record(
          fields
          |> BatList.map(decode_obj_field)
          |> BatList.append(decode_extends_ref(extends_ref))
          |> BatList.unique(~eq=(a, b) =>
               switch (a, b) {
               | (RecordField((a, _), _, _), RecordField((b, _), _, _)) =>
                 BatString.equal(a, b)
               | _ => false
               }
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
  | `Union(_) => raise(Re_Typescript_Decode_Error("Not yet implemented"))
  | `Undefined =>
    raise(
      Re_Typescript_Decode_Error("Undefined cannot exist outside of a union"),
    )
  | `Null =>
    raise(Re_Typescript_Decode_Error("Null cannot exist outside of a union"))
  | `Obj(_) =>
    raise(
      Re_Typescript_Decode_Error(
        "Obj should never be reached in this switch",
      ),
    )
and decode_obj_field =
  fun
  | {key, optional: true, readonly, type_} => {
      RecordField(
        key |> to_valid_ident,
        Optional(type_ |> decode_type),
        readonly,
      );
    }
  | {key, optional: false, readonly, type_} =>
    RecordField(key |> to_valid_ident, type_ |> decode_type, readonly)
and decode_extends_ref = (ref_: Ts.ref_) => {
  // Only implement this naively for now
  let lookup_name = fst(decode_ref_type_name(ref_));
  prerr_endline(lookup_name);
  BatOption.(
    Hashtbl.find_opt(record_cache, lookup_name)
    |> map_default(
         fun
         | Record(fields) => fields
         | _ => [],
         [],
       )
  );
}
and decode_ref_type_name = (ref_: Ts.ref_): (string, string) => {
  let idents = ref_ |> BatList.map(fst) |> BatList.enum;

  let o = BatIO.output_string();
  idents
  |> BatEnum.print(
       ~sep="_",
       (o, part) => {
         BatPrintf.fprintf(o, "%s", part);
         ();
       },
       o,
     );
  BatIO.close_out(o) |> to_valid_typename;
};

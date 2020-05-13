open Re_typescript_base;
open Re_typescript_decode_result;
open Re_typescript_decode_config;
open Re_typescript_decode_utils;

exception Re_Typescript_Decode_Error(string);

let rec decode = (~ctx: config=defaultConfig, toplevel: Ts.toplevel) => {
  toplevel.types |> List.map(decode_type_def);
}
and decode_type_def: ((Ts.type_def, bool)) => type_def =
  fun
  | (`TypeDef(name, type_), _) =>
    TypeDeclaration(name |> to_valid_typename, decode_type(type_))
  | (`InterfaceDef(name, extends_ref, fields), _) =>
    TypeDeclaration(
      name |> to_valid_typename,
      Record(fields |> List.map(decode_obj_field)),
    )
and decode_type: Ts.type_ => type_def =
  fun
  | `String => Base(String)
  | `Number => Base(Number)
  | `Boolean => Base(Boolean)
  | `Void => Base(Void)
  | `Any => Base(Any)
  | `Obj(fields) => Record(fields |> List.map(decode_obj_field))
  | `Array(type_) => Array(decode_type(type_))
  | `Tuple(types) => Tuple(types |> List.map(decode_type))
  | `Ref(ref_) => Base(Ref(""))
  | `Undefined =>
    raise(
      Re_Typescript_Decode_Error("Undefined cannot exist outside of a union"),
    )
  | `Null =>
    raise(Re_Typescript_Decode_Error("Null cannot exist outside of a union"))
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
    RecordField(key |> to_valid_ident, type_ |> decode_type, readonly);

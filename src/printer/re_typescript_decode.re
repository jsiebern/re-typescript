open Re_typescript_base;
open Re_typescript_decode_result;
open Re_typescript_decode_utils;

let rec decode = (~ctx: config=defaultConfig, toplevel: Ts.toplevel) => {
  toplevel.types |> List.map(decode_type_def);
}
and decode_type_def: ((Ts.type_def, bool)) => type_def =
  fun
  | (`TypeDef(name, type_), _) => TypeDeclaration(name, decode_type(type_))
and decode_type: Ts.type_ => type_def =
  fun
  | `String => Base(String)
  | `Number => Base(Number)
  | `Boolean => Base(Boolean)
  | `Null => Base(Null)
  | `Undefined => Base(Undefined)
  | `Void => Base(Void)
  | `Any => Base(Any);

type toplevel = {
  types: list((type_def, bool)),
  imports: list(import),
}
and type_def = [
  | `TypeDef(string, type_, list(type_arg))
  | `InterfaceDef(string, ref_, list(obj_field), list(type_arg))
  | `EnumDef(string, list(enum_field), bool)
]
and type_ = [
  | `String
  | `Number
  | `Boolean
  | `Null
  | `Undefined
  | `Void
  | `Any
  | `Union(list(union_member))
  | `Array(type_)
  | `Tuple(list(type_))
  | `Obj(list(obj_field))
  | `Ref(ref_)
  | `TypeExtract(ref_, list(string))
]
and union_member = [
  | `U_String(string)
  | `U_Number(int)
  | `U_Type(type_)
  | `U_Bool(bool)
]
and prim_value = [ | `V_String(string) | `V_Number(int)]
and enum_field = {
  key: string,
  default: option(prim_value),
}
and obj_field = {
  key: string,
  type_,
  optional: bool,
  readonly: bool,
}
and ref_ = list((string, list(type_)))
and import = {
  path: string,
  name: import_name,
}
and import_name = [
  | `Named(string)
  | `Alias(import_name, string)
  | `Star
  | `List(list(import_name))
]
and type_arg = {
  name: string,
  constraint_: option(type_),
  default: option(type_),
};

type extract_wrapper = [ | `T(type_def, bool) | `I(import) | `Empty];
let make_top_level = (lst: list(extract_wrapper)) =>
  lst
  |> CCListLabels.fold_left(
       ~f=
         (p, value) =>
           switch (value) {
           | `I(import) => {
               ...p,
               imports: CCListLabels.append(p.imports, [import]),
             }
           | `T(type_def) => {
               ...p,
               types: CCListLabels.append(p.types, [type_def]),
             }
           | `Empty => p
           },
       ~init={imports: [], types: []},
     );

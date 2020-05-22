type module_ = {
  types: list(type_def),
  imports: list(import),
  exports: list(string),
  has_declare: bool,
  is_namespace: bool,
  name: string,
}
and type_def = [
  | `Module(module_)
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
  | `Enum(list(enum_field), bool)
  | `Ref(ref_)
  | `TypeExtract(ref_, list(list(string)))
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
  f_key: string,
  f_type_: type_,
  f_optional: bool,
  f_readonly: bool,
}
and ref_ = list((string, list(type_)))
and import = {
  i_path: string,
  i_name: import_name,
}
and import_name = [
  | `Named(string)
  | `Alias(import_name, string)
  | `Star
  | `List(list(import_name))
]
and type_arg = {
  a_name: string,
  a_constraint_: option(type_),
  a_default: option(type_),
};

type extract_wrapper = [ | `T(type_def, bool) | `I(import) | `Empty];
let make_module = (lst: list(extract_wrapper)) =>
  lst
  |> CCListLabels.fold_left(
       ~f=
         (p, value) =>
           switch (value) {
           | `I(import) => {
               ...p,
               imports: CCListLabels.append(p.imports, [import]),
             }
           | `T(type_def, exported) => {
               ...p,
               types: p.types @ [type_def],
               exports:
                 exported
                   ? p.exports
                     @ [
                       switch (type_def) {
                       | `TypeDef(name, _, _) => name
                       | `InterfaceDef(name, _, _, _) => name
                       | `EnumDef(name, _, _) => name
                       | `Module({name, _}) => name
                       },
                     ]
                   : p.exports,
             }
           | `Empty => p
           },
       ~init={
         has_declare: false,
         is_namespace: false,
         name: "",
         imports: [],
         types: [],
         exports: [],
       },
     );

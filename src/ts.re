type toplevel = {
  types: list(type_def),
  imports: list(import),
}
and type_def = [
  | `TypeDef(string, type_)
  | `InterfaceDef(string, ref_, list(obj_field))
]
and type_ = [
  | `String
  | `Number
  | `Boolean
  | `Obj(list(obj_field))
  | `Ref(ref_)
]
and obj_field = {
  key: string,
  type_,
  optional: bool,
}
and ref_ = list((string, list(type_)))
and import = {
  path: string,
  name: importName,
}
and importName = [
  | `Named(string)
  | `Alias(importName, string)
  | `Star
  | `List(list(importName))
];

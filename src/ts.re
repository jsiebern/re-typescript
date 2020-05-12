type type_def = [
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
  name: string,
  type_,
  required: bool,
}
and ref_ = list((string, list(type_)));
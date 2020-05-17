type type_def =
  | Base(base_type)
  | Optional(type_def)
  | Nullable(type_def)
  | TypeDeclaration((string, string), type_def)
  | Record(list(type_def))
  | RecordField((string, string), type_def, bool)
  | Union(list(type_def))
  | VariantEnum(list((string, string)))
  | VariantString(list(string))
  | VariantInt(list(int))
  | VariantMixed(list(value_type))
  | Array(type_def)
  | Tuple(list(type_def))

and base_type =
  | String
  | Number
  | Boolean
  | Void
  | Any
  | Ref((string, string))
and value_type =
  | U_String(string)
  | U_Number(int)
  | U_Bool(bool);

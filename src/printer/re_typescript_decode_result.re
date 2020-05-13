type type_def =
  | Base(base_type)
  | Optional(type_def)
  | Nullable(type_def)
  | TypeDeclaration((string, string), type_def)
  | Record(list(type_def))
  | RecordField((string, string), type_def, bool)
  | Union(list(type_def))
  | VariantString(list((string, option(string))))
  | VariantInt(list((int, option(int))))
  | VariantMixed(list((string, option(value_type))))
  | Array(type_def)
  | Tuple(list(type_def))

and base_type =
  | String
  | Number
  | Boolean
  | Void
  | Any
  | Ref(string)
and value_type =
  | V_String(string)
  | V_Int(int);

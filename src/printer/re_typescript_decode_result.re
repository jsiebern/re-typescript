type type_def =
  | Base(base_type)
  | Optional(type_def)
  | Nullable(type_def)
  | TypeDeclaration(string, type_def)
  | Record(string, type_def)
  | RecordField(string, type_def, bool)
  | Union(list(type_def))
  | VariantString(list((string, option(string))))
  | VariantInt(list((int, option(int))))
  | VariantMixed(list((string, option(value_type))))

and base_type =
  | String
  | Number
  | Boolean
  | Null
  | Undefined
  | Void
  | Any
and value_type =
  | V_String(string)
  | V_Int(int);

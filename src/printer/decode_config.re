open Re_typescript_base;

type bucklescript_config = {
  string_variant_mode,
  number_variant_mode,
  mixed_variant_mode,
}
and string_variant_mode = [ | `Variant | `PolyVariant | `BsInline]
and number_variant_mode = [
  | `Variant(option(number_variant_mode_name))
  | `PolyVariant(option(number_variant_mode_name))
  | `BsInline(option(number_variant_mode_name))
]
and mixed_variant_mode = [ | `BsUnboxed]
and number_variant_mode_name = {
  prefix: string,
  suffix: string,
};

let default_bucklescript_config = {
  string_variant_mode: `BsInline,
  mixed_variant_mode: `BsUnboxed,
  number_variant_mode: `BsInline(Some({prefix: "_", suffix: ""})),
};
let default_bucklescript_bindings_config = {
  string_variant_mode: `Variant,
  mixed_variant_mode: `BsUnboxed,
  number_variant_mode: `Variant(Some({prefix: "_", suffix: ""})),
};

type output_type =
  | BucklescriptBindings(bucklescript_config)
  | Bucklescript(bucklescript_config)
  | Native
and array_mode =
  | List
  | Array
and number_mode =
  | Int
  | Float
  | Unboxed
and intersection_def =
  | Merge
  | Tuple
  | Ignore
and intersection_mode = {
  objects: intersection_def,
  unions: intersection_def,
  classes: intersection_def,
  functions: intersection_def,
  other: intersection_def,
  tuple_members_optional: bool,
}
and config = {
  suppress_warning_for_extended_records: bool,
  omit_extended_unreferenced_records: bool,
  output_type,
  array_mode,
  intersection_mode,
  number_mode,
  files: list((string, list(Ts.declaration))),
  file_loader: (module File_loader.T),
  generate_parser: bool,
  generate_serializer: bool,
};
let default_config: config = {
  suppress_warning_for_extended_records: false,
  omit_extended_unreferenced_records: false,
  output_type: Bucklescript(default_bucklescript_config),
  array_mode: Array,
  number_mode: Float,
  intersection_mode: {
    objects: Tuple,
    unions: Tuple,
    classes: Tuple,
    functions: Tuple,
    other: Tuple,
    tuple_members_optional: false,
  },
  files: [],
  file_loader: (module File_loader.Loader_fs),
  generate_parser: false,
  generate_serializer: false,
};

open Config_t;
open! Bridge_bs;

let default_bucklescript_config = {
  string_variant_mode: #BsInline,
  mixed_variant_mode: #BsUnboxed,
  number_variant_mode: #BsInline(None),
  union_mode: #KeepLiterals,
};

let default_config: config = {
  suppress_warning_for_extended_records: false,
  omit_extended_unreferenced_records: false,
  output_type: Bucklescript,
  bucklescript_config: default_bucklescript_config,
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
  generate_parser: false,
  generate_serializer: false,
};

let config = {
  ...default_config,
  number_mode: Int,
  output_type: Bucklescript,
  omit_extended_unreferenced_records: true,
};
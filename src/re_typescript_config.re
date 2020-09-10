type print_language =
  | ReasonML
  | ReScript;

module Unions = {
  type shared_literal =
    | Unboxed
    | Variant
    | PolymorphicVariant;
  type number_literal = shared_literal;
  type string_literal = shared_literal;
  type t = {
    number_literal,
    string_literal,
  };
};
module Numbers = {
  type mode =
    | Int
    | Float
    | Unboxed({conversion_helpers: bool});
  type t = {mode};
};
module Types = {
  type t =
    | All
    | ExportsOnly
    | Custom(string);
};
type t = {
  print_language,
  top_level_module: bool,
  bindings: bool,
  unions: Unions.t,
  numbers: Numbers.t,
  types: Types.t,
};

let default_config: t = {
  print_language: ReasonML,
  top_level_module: true,
  bindings: false,
  unions: {
    number_literal: Unboxed,
    string_literal: PolymorphicVariant,
  },
  types: All,
  numbers: {
    mode: Int,
  },
};

let config = ref(default_config);

let getConfig = () => config^;

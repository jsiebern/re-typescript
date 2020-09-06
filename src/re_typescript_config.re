type print_language =
  | ReasonMl
  | ReScript;

module Unions = {
  type number_literal =
    | Unboxed
    | PolymorphicVariant;
  type t = {number_literal};
};
module Numbers = {
  type mode =
    | Int
    | Float
    | Unboxed({conversion_helpers: bool});
  type t = {mode};
};
type t = {
  print_language,
  top_level_module: bool,
  bindings: bool,
  unions: Unions.t,
  numbers: Numbers.t,
};

let default_config: t = {
  print_language: ReasonMl,
  top_level_module: true,
  bindings: false,
  unions: {
    number_literal: Unboxed,
  },
  numbers: {
    mode: Int,
  },
};

let config = ref(default_config);

let getConfig = () => config^;

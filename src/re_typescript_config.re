type print_language =
  | ReasonMl
  | ReScript;

module Unions = {
  type number_literal =
    | UnboxedModule
    | PolymorphicVariant;
  type t = {number_literal};
};
type t = {
  print_language,
  top_level_module: bool,
  bindings: bool,
  unions: Unions.t,
};

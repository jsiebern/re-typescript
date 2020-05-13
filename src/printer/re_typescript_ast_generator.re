open Migrate_parsetree;
open Ast_406;
open Parsetree;

module type T = {
  let generate:
    (
      ~ctx: Re_typescript_decode_config.config,
      list(Re_typescript_decode_result.type_def)
    ) =>
    structure;
};

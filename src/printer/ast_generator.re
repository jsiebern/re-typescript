open Migrate_parsetree;
open Ast_406;
open Parsetree;

module type T = {
  let generate:
    (~ctx: Decode_config.config, list(Decode_result.type_def)) => structure;
};

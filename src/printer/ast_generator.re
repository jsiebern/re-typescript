open Migrate_parsetree;
open Ast_406;
open Parsetree;

module type T = {
  let generate:
    (
      ~ctx: Decode_config.config,
      (
        list(Tree_utils.Path.t),
        Hashtbl.t(Tree_utils.Path.t, Tree_types.ts_type),
      )
    ) =>
    structure;
};

open Migrate_parsetree;
open Ast_406;
open Parsetree;

module type T = {
  let generate:
    (
      ~ctx: Re_typescript_config.Config.config,
      (
        list(Tree_utils.Path.t),
        Hashtbl.t(Tree_utils.Path.t, Tree_types.ts_type),
      )
    ) =>
    structure;
};

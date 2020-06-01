open Migrate_parsetree;
open Decode_config;

module Config = Decode_config;

let get_generator: Decode_config.output_type => (module Ast_generator.T) =
  fun
  | Bucklescript(bucklescript_config)
  | BucklescriptBindings(bucklescript_config) =>
    (module
     Ast_generator_bucklescript.Make({
       let config = bucklescript_config;
     }))
  | Native => (module Ast_generator_native);

let structure_from_ts =
    (~ctx: config, input: list(Re_typescript_base.Ts.declaration)) => {
  module Generator = (val get_generator(ctx.output_type): Ast_generator.T);
  let ast =
    Generator.generate(~ctx, Tree_parse.parse__entry_module(~ctx, input));
  let migration =
    Versions.migrate(Versions.ocaml_406, Versions.ocaml_current);

  migration.copy_structure(ast);
};
let print_from_ts =
    (~ctx: config, input: list(Re_typescript_base.Ts.declaration)) => {
  Pprintast.string_of_structure(structure_from_ts(~ctx, input));
};

open Migrate_parsetree;
open Re_typescript_config.Config;

module Tree_utils = Tree_utils;

let get_generator: (~ctx: config) => (module Ast_generator.T) =
  (~ctx: config) =>
    switch (ctx.output_type) {
    | Bucklescript
    | BucklescriptBindings =>
      (module
       Ast_generator_bucklescript.Make({
         let config = ctx.bucklescript_config;
       }))
    | Native => (module Ast_generator_native)
    };

let structure_from_ts =
    (~ctx: config, input: list(Re_typescript_base.Ts.declaration)) => {
  module Generator = (val get_generator(~ctx): Ast_generator.T);
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

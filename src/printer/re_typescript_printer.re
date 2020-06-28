open Migrate_parsetree;
open Re_typescript_config.Config;

module Tree_types = Tree_types;
module Tree_utils = Tree_utils;
module Ast_generator = Ast_generator;

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

let migrate_ast = ast => {
  let migration =
    Versions.migrate(Versions.ocaml_406, Versions.ocaml_current);

  migration.copy_structure(ast);
};

let structure_from_ts =
    (
      ~ctx: config,
      ~resolver: (module Re_typescript_fs.Resolver.T),
      ~parser:
         string =>
         CCResult.t(list(Re_typescript_base.Ts.declaration), string),
      entry: Fp.t(Fp.absolute),
    ) => {
  module Generator = (val get_generator(~ctx): Ast_generator.T);
  let ast =
    Generator.generate(
      ~ctx,
      Tree_parse.parse__entry(~ctx, ~resolver, ~parser, entry),
    );
  migrate_ast(ast);
};
let print_from_ts =
    (
      ~ctx: config,
      ~resolver: (module Re_typescript_fs.Resolver.T),
      ~parser:
         string =>
         CCResult.t(list(Re_typescript_base.Ts.declaration), string),
      entry: Fp.t(Fp.absolute),
    ) => {
  Pprintast.string_of_structure(
    structure_from_ts(~ctx, ~resolver, ~parser, entry),
  );
};

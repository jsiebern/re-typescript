open Migrate_parsetree;
open Re_typescript_decode_config;
open Re_typescript_decode_utils;
open Re_typescript_decode;

let print_from_ts = (~ctx: config, input: Re_typescript_base.Ts.toplevel) => {
  module Generator = (
    val get_decoder(ctx.output_type): Re_typescript_ast_generator.T
  );
  let ast = Generator.generate(~ctx, decode(~ctx, input));
  let migration =
    Versions.migrate(Versions.ocaml_406, Versions.ocaml_current);

  Pprintast.string_of_structure(migration.copy_structure(ast));
};

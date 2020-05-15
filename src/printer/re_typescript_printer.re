open Migrate_parsetree;
open Decode_config;
open Decode_utils;
open Decoder;

module Config = Decode_config;

let print_from_ts = (~ctx: config, input: Re_typescript_base.Ts.toplevel) => {
  module Generator = (val get_decoder(ctx.output_type): Ast_generator.T);
  let ast = Generator.generate(~ctx, decode(~ctx, input));
  let migration =
    Versions.migrate(Versions.ocaml_406, Versions.ocaml_current);

  Pprintast.string_of_structure(migration.copy_structure(ast));
};

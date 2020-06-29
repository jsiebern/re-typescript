open Re_typescript_printer;

module Typescript_t = Typescript_t;
module Typescript_j = Typescript_j;
module Parse = Parse;

open Re_typescript_config.Config;

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

  module Resolver = (val resolver: Re_typescript_fs.Resolver.T);
  module Loader = (val Resolver.config.loader: Re_typescript_fs.Loader.T);
  let file = Loader.file_read(entry);
  let content = file |> CCResult.get_or(~default="");
  let files = [(entry |> Fp.baseName |> CCOpt.get_exn, content)];
  let ast =
    Generator.generate(
      ~ctx,
      Parse.parse(Re_typescript_ts_parser_bin.parse(files)),
    );
  migrate_ast(ast);
};

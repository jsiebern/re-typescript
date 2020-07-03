open Re_typescript_printer;

module Typescript_t = Typescript_t;
module Typescript_j = Typescript_j;
module Parse = Parse;

open Re_typescript_config.Config;

let structure_from_ts = (~ctx: config, files: list((string, string))) => {
  module Generator = (val get_generator(~ctx): Ast_generator.T);

  let ast =
    Generator.generate(
      ~ctx,
      Parse.parse(files |> CCList.get_at_idx_exn(0) |> snd),
    );
  migrate_ast(ast);
};

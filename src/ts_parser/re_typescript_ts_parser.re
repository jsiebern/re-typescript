open Re_typescript_printer;

module Typescript_t = Typescript_t;
module Typescript_j = Typescript_j;
module Test = Test;
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
  let ast = Generator.generate(~ctx, Parse.parse(Test.content));
  migrate_ast(ast);
};

module type Config = {
  let config: Re_typescript_config.Config.bucklescript_config;
};
module Make: (Config) => Ast_generator.T;

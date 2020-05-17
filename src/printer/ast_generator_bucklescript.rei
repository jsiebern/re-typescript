module type Config = {let config: Decode_config.bucklescript_config;};
module Make: (Config) => Ast_generator.T;

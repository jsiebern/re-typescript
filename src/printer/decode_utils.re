open Re_typescript_base;

let ts_from_string = (content: string) => {
  let lexbuf = Lexing.from_string(content);

  try(Ok(Parser.main(Lexer.read, lexbuf))) {
  | Lexer.SyntaxError(msg) => Error(msg)
  | Parser.Error =>
    Error(
      Printf.sprintf(
        "At offset %d: syntax error.\n%s",
        Lexing.lexeme_start(lexbuf),
        Lexing.(
          try(
            "\n\n\""
            ++ sub_lexeme(
                 lexbuf,
                 lexeme_start(lexbuf) - 15,
                 lexeme_end(lexbuf),
               )
            ++ "\"\n\n"
          ) {
          | _ => lexeme(lexbuf)
          }
        ),
      ),
    )
  | e => raise(e)
  };
};

let get_decoder: Decode_config.output_type => (module Ast_generator.T) =
  fun
  | Bucklescript(bucklescript_config)
  | BucklescriptBindings(bucklescript_config) =>
    (module
     Ast_generator_bucklescript.Make({
       let config = bucklescript_config;
     }))
  | Native => (module Ast_generator_native);

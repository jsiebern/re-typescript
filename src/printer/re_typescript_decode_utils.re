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

let get_decoder:
  Re_typescript_decode_config.output_type =>
  (module Re_typescript_ast_generator.T) =
  fun
  | Bucklescript
  | BucklescriptBindings => (module Re_typescript_ast_generator_bucklescript)
  | Native => (module Re_typescript_ast_generator_native);

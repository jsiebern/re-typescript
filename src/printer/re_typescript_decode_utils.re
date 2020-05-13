open Re_typescript_base;

type output_type =
  | BucklescriptBindings
  | Bucklescript
  | Native
and array_mode =
  | List
  | Array
and number_mode =
  | Int
  | Float
  | Unboxed
and config = {
  output_type,
  array_mode,
  number_mode,
  files: list((string, Ts.toplevel)),
  file_loader: (module Re_typescript_file_loader.T),
};

let defaultConfig = {
  output_type: Bucklescript,
  array_mode: Array,
  number_mode: Float,
  files: [],
  file_loader: (module Re_typescript_file_loader.Loader_fs),
};

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

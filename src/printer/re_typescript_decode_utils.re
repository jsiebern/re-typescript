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

let to_valid_ident = ident => (
  BatString.uncapitalize_ascii(
    if (ident.[0] >= '0' && ident.[0] <= '9') {
      "_" ++ ident;
    } else {
      // from gist of sgrove, source:
      // https://gist.github.com/sgrove/335bf1759d8d2f685dfea80d4e6afac7
      [
        "and",
        "as",
        "asr",
        "assert",
        "begin",
        "class",
        "constraint",
        "do",
        "done",
        "downto",
        "else",
        "end",
        "esfun",
        "exception",
        "external",
        "false",
        "for",
        "fun",
        "function",
        "functor",
        "if",
        "in",
        "include",
        "inherit",
        "initializer",
        "land",
        "lazy",
        "let",
        "lor",
        "lsl",
        "lsr",
        "lxor",
        "match",
        "method",
        "mod",
        "module",
        "mutable",
        "new",
        "nonrec",
        "object",
        "of",
        "open",
        "open!",
        "or",
        "pri",
        "private",
        "pub",
        "public",
        "rec",
        "sig",
        "struct",
        "switch",
        "then",
        "to",
        "true",
        "try",
        "type",
        "val",
        "virtual",
        "when",
        "while",
        "with",
      ]
      |> List.exists(reserved_word => ident == reserved_word)
        ? ident ++ "_" : ident;
    },
  ),
  ident,
);

let to_valid_typename = tn =>
  tn |> BatString.uncapitalize_ascii |> to_valid_ident;

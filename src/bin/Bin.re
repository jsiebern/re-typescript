open Re_typescript_base;

let content = {|
type union = string | number | (a: string, b?: number) => string | boolean;
|};

let () = {
  let lexbuf = Lexing.from_string(content |> CCString.trim);

  try(
    {
      print_newline();
      Console.log("--------------------------------------");
      Console.log(
        Re_typescript_printer.print_from_ts(
          ~ctx={
            ...Re_typescript_printer.Config.default_config,
            number_mode: Int,
            output_type:
              Bucklescript({
                ...Re_typescript_printer.Config.default_bucklescript_config,
                string_variant_mode: `BsInline,
              }),
          },
          {...Parser.main(Lexer.read, lexbuf), name: "Entry"},
        ),
      );
      Console.log("--------------------------------------");
      print_newline();
    }
  ) {
  | Lexer.SyntaxError(msg) => Printf.fprintf(stderr, "%s", msg)
  | Parser.Error => Console.error(Error.parser_error(~content, ~lexbuf))
  | e =>
    Console.error(e);
    raise(e);
  };
};

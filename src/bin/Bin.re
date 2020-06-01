open Re_typescript_base;

let content = {|
//type union_1 = string | number | { inline: boolean } | undefined;
interface i_1<A = string> { field1: A };
interface i_2 extends i_1 { field2: boolean }
type x = i_2;
type y = i_1;
|};

let () = {
  let lexbuf = Lexing.from_string(content |> CCString.trim);

  try(
    {
      print_newline();
      Random.self_init();
      Console.log((
        Random.int(1000000),
        Random.int(1000000),
        Random.int(1000000),
        Random.int(1000000),
      ));
      Console.log("--------------------------------------");
      let str =
        Re_typescript_printer.structure_from_ts(
          ~ctx={
            ...Re_typescript_printer.Config.default_config,
            number_mode: Int,
            output_type:
              Bucklescript({
                ...Re_typescript_printer.Config.default_bucklescript_config,
                string_variant_mode: `BsInline,
              }),
          },
          Parser_incr.parse(lexbuf),
        );
      Reason_toolchain.RE.print_implementation_with_comments(
        Format.str_formatter,
        (str, []),
      );
      Console.log(Format.flush_str_formatter());

      Console.log("--------------------------------------");
      print_newline();
    }
  ) {
  | Syntaxerr.Error(msg) => Console.error(msg)
  | Lexer.SyntaxError(msg) => Console.error(msg)
  | Parser_incr.Parsing_error(_)
  | Parser.Error => Console.error(Error.parser_error(~content, ~lexbuf))
  | e =>
    Console.error(e);
    raise(e);
  };
};

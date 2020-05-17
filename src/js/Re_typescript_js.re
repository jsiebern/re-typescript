open Re_typescript_base;
open Js_of_ocaml;

let run = (content: Js.t(Js.js_string)) => {
  let content = content |> Js.to_string;
  let lexbuf = Lexing.from_string(content |> CCString.trim);

  try(
    Re_typescript_printer.print_from_ts(
      ~ctx={
        ...Re_typescript_printer.Config.default_config,
        number_mode: Int,
        output_type:
          Bucklescript({
            ...Re_typescript_printer.Config.default_bucklescript_config,
            string_variant_mode: `PolyVariant,
          }),
      },
      Parser.main(Lexer.read, lexbuf),
    )
    |> Js.string
  ) {
  | Lexer.SyntaxError(msg) =>
    Js.raise_js_error([%js new Js.error_constr](msg |> Js.string))
  | Parser.Error =>
    Js.raise_js_error(
      [%js new Js.error_constr](
        Error.parser_error(~content, ~lexbuf) |> Js.string,
      ),
    )
  | e =>
    Console.error(e);
    raise(e);
  };
};

Js.export("run", run);

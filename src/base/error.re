module FCP =
  FileContextPrinter.Make({
    let config =
      FileContextPrinter.Config.initialize({linesBefore: 1, linesAfter: 3});
  });

let parser_error = (~content, ~lexbuf: Lexing.lexbuf) => {
  let location =
    FCP.print(
      content |> Tablecloth.String.trim |> Tablecloth.String.split(~on="\n"),
      ~highlight=(
        (
          lexbuf.Lexing.lex_start_p.pos_lnum,
          lexbuf.Lexing.lex_start_p.pos_cnum
          - lexbuf.Lexing.lex_start_p.pos_bol
          + 1,
        ),
        (
          lexbuf.Lexing.lex_curr_p.pos_lnum,
          lexbuf.Lexing.lex_curr_p.pos_cnum
          - lexbuf.Lexing.lex_curr_p.pos_bol
          + 1,
        ),
      ),
    );
  Pastel.(
    <Pastel>
      <Pastel> "\n" </Pastel>
      <Pastel bold=true color=Red>
        "          ReTypescript Syntax Error\n"
      </Pastel>
      <Pastel bold=true color=Red>
        "----------------------------------------------------"
      </Pastel>
      <Pastel> "\n" </Pastel>
      <Pastel> location </Pastel>
      <Pastel> "\n" </Pastel>
      <Pastel bold=true color=Red>
        "----------------------------------------------------"
      </Pastel>
      <Pastel italic=true>
        "\n   Check your input at the corresponding position\n"
      </Pastel>
    </Pastel>
  );
};
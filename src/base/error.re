module FCP =
  FileContextPrinter.Make({
    let config =
      FileContextPrinter.Config.initialize({linesBefore: 1, linesAfter: 3});
  });

let parser_error = (~content, ~start: Lexing.position, ~end_: Lexing.position) => {
  let location =
    FCP.print(
      content |> CCString.trim |> CCString.split(~by="\n"),
      ~highlight=(
        (start.pos_lnum, start.pos_cnum - start.pos_bol + 1),
        (end_.pos_lnum, end_.pos_cnum - end_.pos_bol + 1),
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

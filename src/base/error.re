module FCP =
  FileContextPrinter.Make({
    let config =
      FileContextPrinter.Config.initialize({linesBefore: 1, linesAfter: 3});
  });

let print_error = (~msg=?, ~content, highlight) => {
  let location =
    FCP.print(
      content |> CCString.trim |> CCString.split(~by="\n"),
      ~highlight,
    );
  Pastel.(
    <Pastel>
      <Pastel> "\n" </Pastel>
      <Pastel bold=true color=Red> "ReTypescript Error\n" </Pastel>
      <Pastel bold=true color=Red>
        "----------------------------------------------------"
      </Pastel>
      <Pastel color=White>
        <Pastel> "\n" </Pastel>
        <Pastel> location </Pastel>
        <Pastel> "\n" </Pastel>
      </Pastel>
      <Pastel bold=true color=Red>
        "----------------------------------------------------"
      </Pastel>
      <Pastel italic=true>
        {Printf.sprintf(
           "\n%s\n",
           msg
           |> CCOpt.value(
                ~default="Check your input at the corresponding position",
              ),
         )}
      </Pastel>
    </Pastel>
  );
};

let parser_error_with_info = (~msg=?, ~content, pos: Parse_info.t) => {
  print_error(
    ~msg?,
    ~content,
    ((pos.line, pos.col), (pos.line, pos.col + pos.idx)),
  );
};

let parser_error =
    (~msg=?, ~content, ~start: Lexing.position, ~end_: Lexing.position) => {
  print_error(
    ~msg?,
    ~content,
    (
      (start.pos_lnum, start.pos_cnum - start.pos_bol + 1),
      (end_.pos_lnum, end_.pos_cnum - end_.pos_bol + 1),
    ),
  );
};

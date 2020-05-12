let process = (lexbuf: Lexing.lexbuf) => {
  let rec tToStr =
    fun
    | `Ref(e) => Printf.sprintf("Ref<%s>", rToStr(e))
    | `String => Printf.sprintf("string")
    | `Number => Printf.sprintf("number")
    | `Boolean => Printf.sprintf("boolean")
    | `Obj(items) =>
      items
      |> List.fold_left(
           (p, x: Ts.obj_field) => {
             Printf.sprintf("%s  %s: %s\n", p, x.name, tToStr(x.type_))
           },
           "\n",
         )
  and rToStr = r =>
    r
    |> List.fold_left(
         (p, (x, t)) =>
           Printf.sprintf(
             "%s %s%s",
             p,
             x,
             switch (t) {
             | [] => ""
             | t => (t |> List.fold_left((p, t) => tToStr(t), "<")) ++ ">"
             },
           ),
         "",
       )
  and pr = t =>
    switch (t) {
    | `TypeDef(name, t) =>
      Printf.fprintf(stdout, "%s: %s\n", name, tToStr(t))
    | `InterfaceDef(name, extends, items) =>
      Printf.fprintf(stdout, "%s:", name);
      switch (extends) {
      | [] => ()
      | e => Printf.fprintf(stdout, " ...%s", rToStr(e))
      };
      Printf.fprintf(stdout, "\n");
      items
      |> List.iter((x: Ts.obj_field) => {
           Printf.fprintf(stdout, "  %s: %s\n", x.name, tToStr(x.type_))
         });
    };

  /* Run the parser on this line of input. */
  try(Parser.main(Lexer.read, lexbuf) |> List.iter(t => pr(t))) {
  | Lexer.SyntaxError(msg) => Printf.fprintf(stderr, "%s%!", msg)
  | Parser.Error =>
    Printf.fprintf(
      stderr,
      "At offset %d: syntax error.\n%!",
      Lexing.lexeme_start(lexbuf),
    )
  };
};

let () =
  process(
    Lexing.from_string(
      {|
    type x = string;
    type y = number;
    type someObj = {
        some: boolean,
        other: string,
        rec: someObj,
    };
    interface next extends React.CSSProperties<string> {
        has: number;
        obj: somObj;
    };
|},
    ),
  );
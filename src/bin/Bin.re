open Re_typescript_base;

let process = (lexbuf: Lexing.lexbuf) => {
  let rec tToStr =
    fun
    | `Ref(e) => Printf.sprintf("Ref<%s>", rToStr(e))
    | `String => Printf.sprintf("string")
    | `Number => Printf.sprintf("number")
    | `Boolean => Printf.sprintf("boolean")
    | `Obj(items) =>
      " {"
      ++ (
        items
        |> List.fold_left(
             (p, x: Ts.obj_field) => {
               Printf.sprintf("%s  %s: %s\n", p, x.key, tToStr(x.type_))
             },
             "\n",
           )
      )
      ++ "}\n"
  and rToStr = r =>
    r
    |> List.fold_left(
         (p, (x, t)) => {
           Printf.sprintf(
             "%s%s%s",
             p,
             x,
             switch (t) {
             | [] => ""
             | t =>
               (
                 t
                 |> List.fold_left(
                      (p, t) => Printf.sprintf("%s, %s", p, tToStr(t)),
                      "<",
                    )
               )
               ++ ">"
             },
           )
         },
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
           Printf.fprintf(stdout, "  %s: %s\n", x.key, tToStr(x.type_))
         });
    };

  /* Run the parser on this line of input. */
  try({
    let tl = Parser.main(Lexer.read, lexbuf);
    tl.imports
    |> List.iter((i: Ts.import) => Printf.fprintf(stdout, "%s\n", i.path));
    tl.types |> List.iter(t => pr(t));
  }) {
  | Lexer.SyntaxError(msg) => Printf.fprintf(stderr, "%s%!", msg)
  | Parser.Error =>
    Printf.fprintf(
      stderr,
      "At offset %d: syntax error.\n%!",
      Lexing.lexeme_start(lexbuf),
    )
  };
};

let content = {|
import { Palette } from './createPalette';
import * as React from 'react';
import { CSSProperties } from './withStyles';

type x = string;
type y = number;
type someObj = {
    some: boolean,
    other: string,
    rec: someObj,
};
interface next extends React.CSSProperties<Required<{
  fontFamily: string;
  nested: React.SomeOther<{color: number},string,boolean>;
}>> {
    has: number;
    obj: somObj;
};


|};

let () = {
  // process(Lexing.from_string(content));
  Re_typescript_printer.print_from_ts(
    Parser.main(Lexer.read, Lexing.from_string(content)),
  );
};
open Re_typescript_base;

module FCP =
  FileContextPrinter.Make({
    let config =
      FileContextPrinter.Config.initialize({linesBefore: 1, linesAfter: 3});
  });

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

type emptyObj = {};
interface emptyI {};

export declare type Subset<T, U> = {
  [key in keyof T]: key extends keyof U ? T[key] : never;
};
|};
let content = {|
interface I_a {
        field_1: string;
        field_2: string;
        field_3: {
          inline: boolean;
          nested: string;
        }
      }
|};

let () = {
  let lexbuf = Lexing.from_string(content |> Tablecloth.String.trim);

  try(
    Printf.fprintf(
      stdout,
      "%s",
      Re_typescript_printer.print_from_ts(
        ~ctx=Re_typescript_printer.Config.defaultConfig,
        Parser.main(Lexer.read, lexbuf),
      ),
    )
  ) {
  | Lexer.SyntaxError(msg) => Printf.fprintf(stderr, "%s", msg)
  | Parser.Error =>
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
    let error =
      Pastel.(
        <Pastel>
          "\n"
          <Pastel bold=true color=Red>
            "          ReTypescript Syntax Error\n"
          </Pastel>
          <Pastel bold=true color=Red>
            "----------------------------------------------------"
          </Pastel>
          "\n"
          location
          "\n"
          <Pastel bold=true color=Red>
            "----------------------------------------------------"
          </Pastel>
          <Pastel italic=true>
            "\n   Check your input at the corresponding position\n"
          </Pastel>
        </Pastel>
      );
    Console.error(error);
  | e =>
    Console.error(e);
    raise(e);
  };
};

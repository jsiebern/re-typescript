open Re_typescript_base;

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
type x = [string, number, bool];
|};

let () = {
  let lexbuf = Lexing.from_string(content |> Tablecloth.String.trim);

  try(
    Printf.fprintf(
      stdout,
      "%s",
      Re_typescript_printer.print_from_ts(
        ~ctx={
          ...Re_typescript_printer.Config.defaultConfig,
          number_mode: Int,
        },
        Parser.main(Lexer.read, lexbuf),
      ),
    )
  ) {
  | Lexer.SyntaxError(msg) => Printf.fprintf(stderr, "%s", msg)
  | Parser.Error => Console.error(Error.parser_error(~content, ~lexbuf))
  | e =>
    Console.error(e);
    raise(e);
  };
};

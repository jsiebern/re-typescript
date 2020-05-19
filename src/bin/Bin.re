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
type orig_arg<a = string> = a;
type with_arg<a = { b: orig_arg<number> }> = a;
type call_args = with_arg;
|};

let () = {
  let lexbuf = Lexing.from_string(content |> CCString.trim);

  try(
    Printf.fprintf(
      stdout,
      "%s",
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

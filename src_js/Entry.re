module R = {
  type a;
  [@bs.module "reason"] external printRE: a => string = "printRE";
  [@bs.module "reason"] external parseML: string => a = "parseML";
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

module X = {
  [@react.component]
  let make = () => {
    let (v, setV) = React.useReducer((_, v) => v, "");

    <>
      <textarea
        value=v
        onChange={e => setV(e->ReactEvent.Form.target##value)}
      />
      <div className="display">
        <pre>
          {let lexbuf = Lexing.from_string(v);
           try(
             R.printRE(
               R.parseML(
                 Re_typescript_printer.print_from_ts(
                   Parser.main(Lexer.read, lexbuf),
                 ),
               ),
             )
           ) {
           | Lexer.SyntaxError(msg) => Printf.sprintf("%s%!", msg)
           | Parser.Error =>
             Printf.sprintf(
               "At offset %d: syntax error.\n%!",
               Lexing.lexeme_start(lexbuf),
             )
           | _ => ""
           }}
          ->React.string
        </pre>
      </div>
    </>;
  };
};

ReactDOMRe.renderToElementWithId(<X />, "app");
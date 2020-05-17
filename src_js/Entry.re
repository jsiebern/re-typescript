let content = {|
import { Palette } from './createPalette';
import * as React from 'react';
import { CSSProperties } from './withStyles';

interface I_one {
  key_1: string;
  key_2: number;
}
interface I_two extends I_one {
  key_1: number;
  key_3: boolean;
}

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
    obj: someObj;
};

interface NonEmptyI { field: string };

type module = number[];
type interfaceA = Array<NonEmptyI>

interface I_a {
  field_1: string;
  field_2: string;
  field_3: {
    inline: boolean;
    nested: string;
  }
}

type moreObj = {
  another: number,
}
type type_extr = {
    some: boolean,
    other: string,
    rec: type_extr,
    obj: moreObj,
    nested: {
      more: moreObj
    }
};
type t = type_extr['rec']['some'];
type nd = type_extr["obj"]['another'];
type n = type_extr['nested']['more']["another"];

type tpl = [string, number, boolean];
type tpl_inline = [string, { field: number }, boolean];

enum clean_enum { Red, Green, Blue };

type null_string = string | null;
type opt_string = string | undefined;

type variant = 'red' | 'blue' | undefined;
interface inline_variant {
  field: 'red' | 'blue',
}
type inline_variant_in_arr = Array<inline_variant['field']>;

type variant_num = 2 | 4 | 8 | 16;

type x = 1 | 'string' | undefined | false | 3;
|};

module Highlight = {
  [@react.component] [@bs.module "react-highlight.js"]
  external make: (~language: string, ~children: string) => React.element =
    "default";
};

module CodeMirror = {
  [%bs.raw "require('codemirror/lib/codemirror.css')"];
  [%bs.raw "require('codemirror/mode/javascript/javascript')"];
  type ts_mode = {
    name: string,
    typescript: bool,
  };
  type options = {
    lineNumbers: bool,
    mode: ts_mode,
    theme: string,
  };
  [@react.component] [@bs.module]
  external make:
    (
      ~value: string,
      ~onChange: string => unit,
      ~options: option(options)=?,
      ~className: option(string)=?,
      ~preserveScrollPosition: option(bool)=?
    ) =>
    React.element =
    "react-codemirror";
};

module X = {
  [@react.component]
  let make = () => {
    let (v, setV) = React.useReducer((_, v) => v, content);
    let (re, setRe) = React.useReducer((_, v) => v, true);
    let result = BsPrinter.usePrintedValue(~re, v);

    <>
      <CodeMirror
        className="editor"
        value=v
        onChange=setV
        preserveScrollPosition=true
        options={
          lineNumbers: true,
          mode: {
            name: "javascript",
            typescript: true,
          },
          theme: "monokai",
        }
      />
      {switch (result) {
       | Ok(printed) =>
         <div className="display">
           <button className="switch-button" onClick={_ => setRe(!re)}>
             (re ? "ReasonML" : "ocaml")->React.string
           </button>
           <pre>
             <Highlight language={re ? "reasonml" : "ocaml"}>
               printed
             </Highlight>
           </pre>
         </div>
       | Error(e) =>
         <div
           className="error"
           style={ReactDOMRe.Style.make(~padding="15px", ())}
           dangerouslySetInnerHTML={"__html": e}
         />
       }}
    </>;
  };
};

ReactDOMRe.renderToElementWithId(<X />, "app");

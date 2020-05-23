let content = {|
// --------------------------------------------
// re-typescript
// An opinionated attempt at finally solving typescript interop for ReasonML / OCaml.
// --------------------------------------------

// --- What works?

// --- Base type declarations
type s = string;
type n = number; // re-typescript has different number resolution modes, default is "float"
type b = boolean;
type v = void;
type a = any; // Any: A separate any type will be created that is marked as unboxed. Currently this would need to be decoded via %identity access

// --- Unions that contain null / undefined are wrapped in the coversion
type s_null =     string | null;
type s_opt =      string | undefined;
type s_null_opt = string | null | undefined;

// --- Interfaces & basic interface inheritance
interface IOne {
  key_1: string;
  key_2: boolean;
}
interface ITwo extends IOne {
  key_3: number;
}

// --- Interfaces & obj types are generated equal as records
type obj_1 = { key: string };

// --- Basic type extraction
// (Referenced type get resolved at the moment, this will probably change to shared types being hoised into scope for both call sites)
interface IExtraction {
  key_1: boolean;
  key_2: obj_1;
}
type extracted_bool = IExtraction['key_1'];
type extracted_string_nested = IExtraction['key_2']['key'];

// --- Array declarations
type arr_of_bool    = Array<boolean>;
type arr_of_string  = string[];

// --- Inline object creation
type array_of_inline = Array<{ inline: boolean }>;
interface IInline {
  field_1: string;
  inline: {
    field: number;
    nested: {
      nested_field: boolean;
    }
  }
}

// --- Tuples
type tpl_1 = [string, string, number];
type tpl_2 = [boolean, { inline: boolean }];

// --- Enums
enum enum_1 { Red, Green, Blue };

// --- String literals
// (String literals have 3 config types: Variant, PolyVariant, BsInline. These will be handled differently for generating the encoder / decoder functions.)
type string_literal = "double" | 'single' | '$_bad_string';
type string_literal_opt = 'one' | 'two' | undefined;

// --- Numeric literals (Generates bs.inline by default)
type num_literal = 2 | 4 | 8 | 12;

// --- Mixed literals (Generates bs.unboxed by default)
type mixed_literal_1 = 2 | 'string' | 8 | 'other_string';
type mixed_literal_2 = false | 1;

// --- Type unions (Generates bs.unboxed by default)
// (Creating a variant alongside the unboxed value is planned. Still undecided on how to build the classify structure.)
type union_1 = string | number | undefined;
type union_2 = string_literal | string;

// --- Type parameters
type with_param<A> = A;
type use_param = with_param<string>;

interface IParam<A, B, C = string> {
  a: A,
  b: B,
  c: C,
}
interface IParamExt<A, B = number, C = boolean> extends IParam<A,B,C> {}

type use_interface_param = IParamExt<string>; // Default params are lazy, they will be applied at the call site of the parameterized type

interface IParamDefaultInline<P = { is_default: boolean }> {
  key: string;
  p: P;
}
type use_default_inline = IParamDefaultInline;

// --- Function definitions
declare function some_function(): void;
declare function some_other_function(): { inline: string };
declare function some_function_args(a: string, b: number): string;
declare function some_function_opt_args(a: string, b?: number): string;
declare function some_function_inline_args(a: string | number, b?: { inline: number }): string;
declare function some_function_as_any();
declare function some_function_params<A,B,C>(a: A, b: B): C;

// --- Inline function definitions
interface i_inline_function {
    field: string;
    action: (a: string, b?: number) => void;
}
type i_inline_access = i_inline_function['action'];



// --------------------------------------------
// What doesn't?
// --------------------------------------------

// enum enum_2 { Red, Green, Blue = 5 };
// enum enum_3 { Red = "red", Green = "green", Blue = "blue" };
// type union_1 = string | number | { inline: boolean } | undefined;
// type x<B> = string | B; (might beed to only generate module at the callsite)

// interface IParamNested<A, B> {
//   a: A,
//   b: {
//     snd_b: B
//   }
// }

// type x = 'x' | 'y';
// type y = 'z' | x;

// export declare type Subset<T, U> = {
//  [key in keyof T]: key extends keyof U ? T[key] : never;
// };
|};

module Highlight = {
  [@react.component] [@bs.module "react-highlight.js"]
  external make: (~language: string, ~children: string) => React.element =
    "default";
};

module MonacoEditor = {
  [@react.component] [@bs.module "@monaco-editor/react"]
  external make:
    (
      ~height: string=?,
      ~width: string=?,
      ~language: [@bs.string] [ | `javascript | `typescript],
      ~value: string,
      ~onChange: ('a, string) => unit
    ) =>
    React.element =
    "ControlledEditor";
};

module X = {
  [@react.component]
  let make = () => {
    let (v, setV) = React.useReducer((_, v) => v, content);
    let (re, setRe) = React.useReducer((_, v) => v, true);
    let result = BsPrinter.usePrintedValue(~re, v);

    <>
      <MonacoEditor
        height="100vh"
        width="70vw"
        value=v
        onChange={(_, v) => setV(v)}
        language=`typescript
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
           style={ReactDOMRe.Style.make(
             ~padding="15px",
             ~whiteSpace="pre",
             (),
           )}
           dangerouslySetInnerHTML={"__html": e}
         />
       }}
    </>;
  };
};

ReactDOMRe.renderToElementWithId(<X />, "app");

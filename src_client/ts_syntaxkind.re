open Js_of_ocaml.Js;

// Load Project into global namespace
let require = (s: t(js_string)): 'a =>
  Unsafe.fun_call(Unsafe.js_expr("require"), [|Unsafe.inject(s)|]);

let tsMorph = require("ts-morph" |> string);
Js_of_ocaml.Js.Unsafe.global##._SyntaxKind :=  tsMorph##.ts##._SyntaxKind;

include (
          [%js]: {
            type syntaxKind = Ojs.t;
            [@js.global "SyntaxKind"]
            let syntaxKind: syntaxKind;
          }
        );

open Ts_morph;
open Js_of_ocaml.Js;

include (
          [%js]: {
            module Config: {
              type t;
              [@js.builder]
              let make:
                (
                  ~sourceFile: Ts_raw.Node.t=?,
                  ~typeChecker: Ts_raw.TypeChecker.t=?,
                  unit
                ) =>
                t;
            };
          }
        );

let require = (s: t(js_string)): 'a =>
  Unsafe.fun_call(Unsafe.js_expr("require"), [|Unsafe.inject(s)|]);

let tsMorph = require("ts-morph" |> string);
let createWrappedNode = tsMorph##.createWrappedNode;
let nodeStatic = tsMorph##._Node;
let typeGuards = tsMorph##._TypeGuards;
let isNamedNode = (t: Ojs.t) =>
  Unsafe.fun_call(nodeStatic##.isNamedNode, [|Unsafe.inject(t)|]);

let tsMorphCommon = require("@ts-morph/common" |> string);
let getSyntaxKindName = tsMorphCommon##.getSyntaxKindName;

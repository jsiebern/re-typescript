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
let isTypeAliasDeclaration = (t: Ojs.t) =>
  Unsafe.fun_call(
    nodeStatic##.isTypeAliasDeclaration,
    [|Unsafe.inject(t)|],
  );
let ts = tsMorph##.ts;
let symbolFlags = ts##._SymbolFlags;
let getSymbolFlag = (flag: Ts_nodes.Symbol.flags) =>
  Unsafe.get(
    symbolFlags,
    Unsafe.inject(flag |> Ts_nodes.Symbol.flags_to_js),
  );

let tsMorphCommon = require("@ts-morph/common" |> string);
let getSyntaxKindName = (sKind: int) =>
  Unsafe.fun_call(
    tsMorphCommon##.getSyntaxKindName,
    [|Unsafe.inject(sKind |> Ojs.int_to_js)|],
  )
  |> Ojs.string_of_js;

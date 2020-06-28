module Ts = Typescript_t;
module Types = Re_typescript_printer.Tree_types;

open Types;
open Re_typescript_printer.Tree_utils;

let types: Hashtbl.t(ts_path, ts_type) = Hashtbl.create(10);
type order = {mutable lst: list(ts_path)};
let order = {lst: []};

let rec parse__node = (node: Ts.node) => {
  switch (node) {
  | `TypeAliasDeclaration(tad) => parse__TypeAliasDeclaration(tad)
  | _ => raise(Not_found)
  };
}
and parse__TypeAliasDeclaration = (declaration: Ts.node_TypeAliasDeclaration) => {
  switch (declaration) {
  | {
      name: `Identifier({escapedText, _}),
      resolvedSymbol: Some({resolvedType: Some(t), _}),
      _,
    } =>
    let path = ([escapedText], []);
    let td =
      Types.TypeDeclaration({
        td_name: escapedText |> Ident.of_string,
        td_type: parse__type_definition(t),
        td_parameters: [],
      });
    Hashtbl.add(types, path, td);
    order.lst = order.lst @ [path];
  | _ => ()
  };
}
and parse__type_definition = (type_def: Ts.type_) => {
  switch (type_def) {
  | `String(_) => Base(String)
  | `Number(_) => Base(Number)
  | _ => raise(Not_found)
  };
}
and parse__sourceFile = (sourceFile: Ts.node_SourceFile) => {
  sourceFile.statements |> CCList.iter(parse__node);
}
and parse = (source: string) => {
  switch (Typescript_j.node_of_string(source)) {
  | `SourceFile(sf) => parse__sourceFile(sf)
  | _ => raise(Not_found)
  };

  (order.lst, types);
};

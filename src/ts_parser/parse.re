module Ts = Typescript_t;
module Types = Re_typescript_printer.Tree_types;

let pp_node = node =>
  Typescript_j.string_of_node(node)
  |> Yojson.Basic.from_string
  |> Yojson.Basic.pretty_to_string;
let pp_type = type_ =>
  Typescript_j.string_of_type_(type_)
  |> Yojson.Basic.from_string
  |> Yojson.Basic.pretty_to_string;

open Types;
open Re_typescript_printer.Tree_utils;

let types: Hashtbl.t(ts_path, ts_type) = Hashtbl.create(10);
type order = {mutable lst: list(ts_path)};
let order = {lst: []};

let rec parse__node = (node: Ts.node) => {
  switch (node) {
  | `TypeAliasDeclaration(tad) => parse__TypeAliasDeclaration(tad)
  | `EnumDeclaration(ed) => parse__EnumDeclaration(ed)
  | `FunctionDeclaration(fd) => parse__FunctionDeclaration(fd)
  | node =>
    Console.log(
      Typescript_j.string_of_node(node)
      |> Yojson.Basic.from_string
      |> Yojson.Basic.pretty_to_string,
    )
  };
}
and parse__FunctionDeclaration = (declaration: Ts.node_FunctionDeclaration) => {
  let name =
    switch (declaration.name) {
    | Some(`Identifier({escapedText, _})) => escapedText
    | _ => raise(Not_found)
    };

  let fn = {
    fu_params:
      declaration.parameters
      |> CCList.map(param =>
           switch (param) {
           | `Parameter(
               (
                 {name: `Identifier({escapedText, resolvedSymbol, _}), _}: Ts.node_Parameter
               ),
             ) as param => {
               fp_name: escapedText |> Ident.of_string,
               fp_type:
                 switch (
                   parse__typeOfNode(
                     ~parent=`FunctionDeclaration(declaration),
                     param,
                   )
                 ) {
                 | Some(t) => t
                 | None => raise(Not_found)
                 },
               fp_optional:
                 switch (resolvedSymbol) {
                 | None => false
                 // Todo: Replace flags type with "t"
                 | Some({flags, _}) =>
                   Typescript_flags.Symbol.(flags |> Obj.magic && optional)
                 },
             }
           | _ => raise(Not_found)
           }
         ),
    fu_return:
      switch (
        declaration.type_
        |> CCOpt.flat_map(
             parse__typeOfNode(~parent=`FunctionDeclaration(declaration)),
           )
      ) {
      | Some(t) => t
      | None => raise(Not_found)
      },
  };

  let td =
    Types.TypeDeclaration({
      td_name: name |> Ident.of_string,
      td_type: Function(fn),
      td_parameters: [],
    });
  let path = ([name], []);
  Hashtbl.add(types, path, td);
  order.lst = order.lst @ [path];
  ();
}
and parse__TypeLiteral = (~parent: Ts.node, type_literal: Ts.node_TypeLiteral) => {
  let isObject =
    switch (
      type_literal.resolvedType |> CCOpt.map(Typescript_unwrap.unwrap_Type)
    ) {
    | None => false
    | Some({flags, _}) =>
      Typescript_flags.Type.(flags |> Obj.magic && object_)
    };
  if (isObject) {
    Interface(
      type_literal.members
      |> CCList.map(member =>
           switch (member) {
           | `PropertySignature(
               (
                 {name: `Identifier({escapedText, _}), type_, _}: Ts.node_PropertySignature
               ),
             ) as pSig => {
               f_name: escapedText |> Ident.of_string,
               f_type:
                 parse__typeOfNode(~parent=`TypeLiteral(type_literal), pSig)
                 |> CCOpt.get_or(~default=Base(Any)),
             }
           | node =>
             Console.log(pp_node(node));
             raise(Not_found);
           }
         ),
      false,
    );
  } else {
    Console.log("Currently invalid type literal:");
    Console.log(pp_node(`TypeLiteral(type_literal)));
    Base(Any);
  };
}
and parse__EnumDeclaration = (declaration: Ts.node_EnumDeclaration) => {
  let name =
    switch (declaration.name) {
    | `Identifier({escapedText, _}) => escapedText
    | _ => raise(Not_found)
    };
  let members =
    declaration.members
    |> CCList.map(member =>
         switch (member) {
         | `EnumMember(
             ({name: `Identifier({escapedText, _}), _}: Ts.node_EnumMember),
           ) =>
           escapedText |> Ident.of_string
         | _ => raise(Not_found)
         }
       );

  let td =
    Types.TypeDeclaration({
      td_name: name |> Ident.of_string,
      td_type: Enum(members),
      td_parameters: [],
    });
  let path = ([name], []);
  Hashtbl.add(types, path, td);
  order.lst = order.lst @ [path];
}
and parse__TypeAliasDeclaration = (declaration: Ts.node_TypeAliasDeclaration) => {
  let parent = `TypeAliasDeclaration(declaration);
  switch (declaration) {
  // ConditionalType will never be printed, no such concept in reason
  | {type_: `ConditionalType(_), _} => ()
  | {name: `Identifier({escapedText, _}), _} =>
    // Try to get the definition from ident symbol, maybe a better way later
    let type_ =
      parse__typeOfNode_forEach(
        ~isDeclarationChild=true,
        ~parent,
        [declaration.type_],
      );
    let path = ([escapedText], []);
    let td =
      Types.TypeDeclaration({
        td_name: escapedText |> Ident.of_string,
        td_type: type_ |> CCOpt.get_or(~default=Base(Any)),
        td_parameters:
          declaration.typeParameters
          |> CCOpt.map_or(~default=[], params =>
               params
               |> CCList.map(param =>
                    switch (param) {
                    | `TypeParameter(
                        (
                          {name: `Identifier({escapedText, _}), _}: Ts.node_TypeParameter
                        ),
                      ) => {
                        tp_name: Ident.of_string(escapedText),
                        tp_extends: None,
                        tp_default: None,
                      }

                    | _ => raise(Not_found)
                    }
                  )
             ),
      });
    Hashtbl.add(types, path, td);
    order.lst = order.lst @ [path];
  | node =>
    Console.log(pp_node(`TypeAliasDeclaration(node)));
    raise(Not_found);
  };
}
and parse__wrapNonDeclarationChild = (~parent, node: Ts.node): ts_type => {
  let values = node |> Typescript_unwrap.unwrap_Node;
  let parentSymbol = (parent |> Typescript_unwrap.unwrap_Node).resolvedSymbol;
  let identName =
    switch (parentSymbol, values.resolvedSymbol) {
    | (Some({name, _}), Some(child)) => name ++ child.name
    | (Some({name, _}), None) => name ++ "__type"
    | (None, Some({name, _})) => "__" ++ name
    | (None, None) => raise(Not_found)
    };
  parse__TypeAliasDeclaration({
    pos: values.pos,
    end_: values.end_,
    // Fix: Retrieve real kind
    kind: (-1),
    kindName: "TypeAliasDeclaration",
    // Fix
    flags: Typescript_flags.Node.const |> Obj.magic,
    modifiers: None,
    decorators: None,
    resolvedSymbol: values.resolvedSymbol,
    resolvedType: None,
    name:
      `Identifier({
        resolvedType: None,
        escapedText: identName,

        pos: values.pos,
        end_: values.end_,
        // Fix
        kind: (-1),
        kindName: "Identifier",
        flags: Typescript_flags.Node.none |> Obj.magic,
        modifiers: None,
        decorators: None,
        resolvedSymbol: None,
        originalKeywordKind: None,
      }),
    typeParameters: None,
    type_: node,
  });
  Reference({
    tr_path: [identName],
    tr_path_resolved: Some(([identName], [])),
    tr_parameters: [],
  });
}
and parse__typeOfNode_forEach =
    (~isDeclarationChild=false, ~parent: Ts.node, nodes: list(Ts.node))
    : option(ts_type) => {
  nodes |> CCList.find_map(parse__typeOfNode(~isDeclarationChild, ~parent));
}
and parse__typeOfNode =
    (~isDeclarationChild=false, ~parent: Ts.node, node: Ts.node)
    : option(ts_type) =>
  try(Some(parse__typeOfNode_exn(~isDeclarationChild, ~parent, node))) {
  | Not_found => None
  }
and parse__typeOfNode_exn =
    (~isDeclarationChild=false, ~parent: Ts.node, node: Ts.node): ts_type => {
  switch (node) {
  | `VoidKeyword(_) => Base(Void)
  | `StringKeyword(_) => Base(String)
  | `NumberKeyword(_) => Base(Number)
  | `BooleanKeyword(_) => Base(Boolean)
  | `AnyKeyword(_) => Base(Any)
  | `NullKeyword(_) => Base(Null)
  | `UndefinedKeyword(_) => Base(Undefined)
  | `NeverKeyword(_) => Base(Never)
  | `TypeLiteral(tl) as node =>
    isDeclarationChild
      ? parse__TypeLiteral(~parent, tl)
      : parse__wrapNonDeclarationChild(~parent, node)
  | otherNode =>
    let asGeneric = otherNode |> Typescript_unwrap.unwrap_Node;
    switch (asGeneric) {
    | {resolvedType: Some(resolvedType), _} =>
      parse__typeOfType(~isDeclarationChild, ~parent, resolvedType)
    | {resolvedSymbol: Some({resolvedType: Some(resolvedType), _}), _} =>
      parse__typeOfType(~isDeclarationChild, ~parent, resolvedType)
    | _ =>
      Console.log(
        Typescript_j.string_of_node(node)
        |> Yojson.Basic.from_string
        |> Yojson.Basic.pretty_to_string,
      );
      raise(Not_found);
    };
  };
}
and parse__typeOfType =
    (~isDeclarationChild=false, ~parent: Ts.node, type_def: Ts.type_)
    : Types.ts_type => {
  switch (type_def) {
  | `String(_) => Base(String)
  | `Number(_) => Base(Number)
  | `Any(_) => Base(Any)
  | `Boolean(_) => Base(Boolean)
  | `StringLiteral({value, _}) => StringLiteral([value |> Ident.of_string])
  | t =>
    Console.log(
      Typescript_j.string_of_node(parent)
      |> Yojson.Basic.from_string
      |> Yojson.Basic.pretty_to_string,
    );
    Console.log(
      Typescript_j.string_of_type_(t)
      |> Yojson.Basic.from_string
      |> Yojson.Basic.pretty_to_string,
    );

    Base(Any);
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

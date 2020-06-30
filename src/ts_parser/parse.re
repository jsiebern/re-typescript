module Ts = Typescript_t;
module Types = Re_typescript_printer.Tree_types;

module Debug = {
  PastelConsole.init();

  let pp_node = node =>
    Typescript_j.string_of_node(node)
    |> Yojson.Basic.from_string
    |> Yojson.Basic.pretty_to_string;
  let pp_type = type_ =>
    Typescript_j.string_of_type_(type_)
    |> Yojson.Basic.from_string
    |> Yojson.Basic.pretty_to_string;
  let pp_node_kind = node => (node |> Typescript_unwrap.unwrap_Node).kindName;
  let pp_type_kind = type_ =>
    (type_ |> Typescript_unwrap.unwrap_Type).kindName;

  let write_node = node =>
    pp_node_kind(node)
    ++ (
      Typescript_unwrap.unwrap_Name(node)
      |> CCOpt.map(name => "(" ++ name ++ ")")
      |> CCOpt.value(~default="")
    );

  type path_member = [
    | `String(string)
    | `Error(string)
    | `Pos(string)
    | `Node(Ts.node)
    | `Type(Ts.type_)
    | `Pair(path_member, path_member)
  ];
  type parse = {
    mutable json_enabled: bool,
    mutable enabled: bool,
    mutable currentEntry: string,
    mutable parsePath: Hashtbl.t(string, list(path_member)),
  };
  let parse = {
    json_enabled: false,
    enabled: true,
    currentEntry: "",
    parsePath: Hashtbl.create(0),
  };

  let put = member =>
    CCHashtbl.add_list(parse.parsePath, parse.currentEntry, member);
  let add = str => put(`String(str));
  let add_pos = str => put(`Pos(str));

  let add_node = (node: Ts.node) => put(`Node(node));
  let add_type = (type_: Ts.type_) => put(`Type(type_));
  let add_pair = (node: Ts.node, type_: Ts.type_) =>
    put(`Pair((`Node(node), `Type(type_))));
  let add_node_pair = (node: Ts.node, node2: Ts.node) =>
    put(`Pair((`Node(node), `Node(node2))));
  let set_current = (node: Ts.node) => {
    parse.currentEntry = write_node(node);
  };
  exception Parse_error_with_path(string);
  let print_path = () => {
    Console.debug(
      Pastel.(
        {
          let indent_length = "   ";
          let rec pr = (path_left: list(path_member), depth) => {
            let indent =
              depth >= 0
                ? {
                  CCString.repeat(indent_length, depth);
                }
                : "";
            let br = depth < 0 ? "" : "\n";
            <Pastel>
              {switch (path_left) {
               | [] => <Pastel />
               | [`Node(node), ...left] =>
                 <Pastel>
                   indent
                   <Pastel color=Yellow> {write_node(node)} </Pastel>
                   br
                   {pr(left, depth)}
                   br
                 </Pastel>
               | [`Type(type_), ...left] =>
                 <Pastel>
                   indent
                   <Pastel color=Blue> {pp_type_kind(type_)} </Pastel>
                   br
                   {pr(left, depth)}
                   br
                 </Pastel>
               | [`Pair(m1, m2), ...left] =>
                 <Pastel>
                   indent
                   <Pastel color=White>
                     {"[ "
                      ++ {
                        pr([m1], -1);
                      }
                      ++ " > "
                      ++ {
                        pr([m2], -1);
                      }
                      ++ " ]"}
                   </Pastel>
                   br
                   {pr(left, depth)}
                   br
                 </Pastel>
               | [`Pos(pos), ...left] =>
                 <Pastel>
                   indent
                   indent_length
                   <Pastel underline=true color=Green> pos </Pastel>
                   br
                   {pr(left, depth + 2)}
                   br
                 </Pastel>
               | [`String(str), ...left] =>
                 <Pastel>
                   indent
                   <Pastel color=Cyan> str </Pastel>
                   br
                   {pr(left, depth)}
                   br
                 </Pastel>
               | [`Error(str), ...left] =>
                 <Pastel>
                   indent
                   <Pastel color=RedBright bold=true> str </Pastel>
                   br
                   {pr(left, depth)}
                   br
                 </Pastel>
               }}
            </Pastel>;
          };
          <Pastel>
            "\n"
            <Pastel bold=true color=CyanBright underline=true>
              {parse.currentEntry ++ ":"}
              "\n"
            </Pastel>
            {switch (Hashtbl.find_opt(parse.parsePath, parse.currentEntry)) {
             | None => <Pastel color=White> "n/a" </Pastel>
             | Some(path) => pr(path |> CCList.rev, 0)
             }}
            "\n"
          </Pastel>;
        }
      ),
    );
  };
  let raiseWith = (~type_=?, ~node=?, str) =>
    if (parse.enabled) {
      {
        if (str !== "") {
          put(`Error(">>> " ++ str));
        };
        print_path();
      };
      if (parse.json_enabled) {
        Console.debug(
          Pastel.(
            <Pastel>
              {switch (type_, node) {
               | (None, None) => ""
               | (Some(t), None) =>
                 <Pastel>
                   <Pastel bold=true underline=true color=CyanBright>
                     "Type:"
                   </Pastel>
                   <Pastel backgroundColor=WhiteBright color=Black>
                     "\n"
                     "\n"
                     "    "
                     {pp_type(t)}
                     "    "
                     "\n"
                   </Pastel>
                   "\n"
                   "\n"
                 </Pastel>

               | (None, Some(n)) =>
                 <Pastel>
                   <Pastel bold=true underline=true color=CyanBright>
                     "Node:"
                   </Pastel>
                   <Pastel backgroundColor=WhiteBright color=Black>
                     "\n"
                     "\n"
                     "    "
                     {pp_node(n)}
                     "    "
                     "\n"
                   </Pastel>
                   "\n"
                   "\n"
                 </Pastel>
               | (Some(t), Some(n)) =>
                 <Pastel>
                   <Pastel bold=true underline=true color=CyanBright>
                     "Type:"
                   </Pastel>
                   <Pastel backgroundColor=WhiteBright color=Black>
                     "\n"
                     "\n"
                     "    "
                     {pp_type(t)}
                     "    "
                     "\n"
                   </Pastel>
                   "\n"
                   "\n"
                   <Pastel bold=true underline=true color=CyanBright>
                     "Node:"
                   </Pastel>
                   <Pastel backgroundColor=WhiteBright color=Black>
                     "\n"
                     "\n"
                     "    "
                     {pp_node(n)}
                     "    "
                     "\n"
                   </Pastel>
                   "\n"
                   "\n"
                 </Pastel>
               }}
            </Pastel>
          ),
        );
      };
      if (parse.enabled) {
        raise(Parse_error_with_path(str));
      };
    };
  let raise = () => {
    raiseWith("");
  };
};

open Types;
open Re_typescript_printer.Tree_utils;

let types: Hashtbl.t(ts_path, ts_type) = Hashtbl.create(10);
type order = {mutable lst: list(ts_path)};
let order = {lst: []};

let rec parse__node = (node: Ts.node) => {
  Debug.add_pos("parse__node");
  Debug.add_node(node);
  switch (node) {
  | `TypeAliasDeclaration(tad) => parse__TypeAliasDeclaration(tad)
  | `EnumDeclaration(ed) => parse__EnumDeclaration(ed)
  | `FunctionDeclaration(fd) => parse__FunctionDeclaration(fd)
  | `InterfaceDeclaration(id) => parse__InterfaceDeclaration(id)
  | node => Debug.raise()
  };
}
and parse__FunctionDeclaration = (declaration: Ts.node_FunctionDeclaration) => {
  Debug.add_pos("parse__FunctionDeclaration");
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
      switch (declaration.type_) {
      | None => Base(Any) // No return: void
      | Some(dec_type) =>
        parse__typeOfNode_exn(
          ~parent=`FunctionDeclaration(declaration),
          dec_type,
        )
      },
  };

  let td =
    Types.TypeDeclaration({
      td_name: name |> Ident.of_string,
      td_type: Function(fn),
      td_parameters:
        parse__DeclarationTypeParameters(
          ~parent=`FunctionDeclaration(declaration),
          declaration.typeParameters,
        ),
    });
  let path = ([name], []);
  Hashtbl.add(types, path, td);
  order.lst = order.lst @ [path];
  ();
}
and parse__Signature = (~parent: Ts.node, signature: Ts.node) => {
  Debug.add_pos("parse__Signature");
  switch (signature) {
  | `MethodSignature(
      (
        {name: Some(`Identifier({escapedText, _})), type_, _}: Ts.node_MethodSignature
      ),
    ) as pSig
  | `PropertySignature(
      (
        {name: `Identifier({escapedText, _}), type_, _}: Ts.node_PropertySignature
      ),
    ) as pSig =>
    Some((
      escapedText |> Ident.of_string,
      switch (parse__typeOfNode(~parent, pSig)) {
      | Some(t) => t
      | None =>
        Debug.add_node(pSig);
        Debug.raiseWith("PropertySignature type could not be resolved");
        Base(Any);
      },
    ))
  | node =>
    Debug.add_node(node);
    Debug.raiseWith("Signature not resolveable");
    None;
  };
}
and parse__InterfaceDeclaration = (declaration: Ts.node_InterfaceDeclaration) => {
  Debug.add_pos("parse__InterfaceDeclaration");
  let parent = `InterfaceDeclaration(declaration);
  let name =
    switch (declaration.name) {
    | `Identifier({escapedText, _}) => escapedText
    | _ => raise(Not_found)
    };

  let members =
    declaration.members
    |> CCList.filter_map(member =>
         switch (parse__Signature(~parent, member)) {
         | None => None
         | Some((name, t)) => Some({f_name: name, f_type: t}: ts_field)
         }
       );

  let td =
    Types.TypeDeclaration({
      td_name: name |> Ident.of_string,
      td_type: Interface(members, false),
      td_parameters:
        parse__DeclarationTypeParameters(~parent, declaration.typeParameters),
    });
  let path = ([name], []);
  Hashtbl.add(types, path, td);
  order.lst = order.lst @ [path];
  ();
}
and parse__TypeLiteral = (~parent: Ts.node, type_literal: Ts.node_TypeLiteral) => {
  Debug.add_pos("parse__TypeLiteral");
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
      |> CCList.filter_map(member =>
           switch (parse__Signature(~parent, member)) {
           | None => None
           | Some((name, t)) => Some({f_name: name, f_type: t}: ts_field)
           }
         ),
      false,
    );
  } else {
    Console.error("Currently invalid type literal:");
    Debug.add_node(`TypeLiteral(type_literal));
    Debug.raise();
    Base(Any);
  };
}
and parse__EnumDeclaration = (declaration: Ts.node_EnumDeclaration) => {
  Debug.add_pos("parse__EnumDeclaration");
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
and parse__DeclarationTypeParameters = {
  Debug.add_pos("parse__DeclarationTypeParameters");
  (~parent: Ts.node, typeParameters: option(list(Ts.node))) => {
    typeParameters
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
       );
  };
}
and parse__TypeAliasDeclaration = (declaration: Ts.node_TypeAliasDeclaration) => {
  Debug.add_pos("parse__TypeAliasDeclaration");
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
          parse__DeclarationTypeParameters(
            ~parent,
            declaration.typeParameters,
          ),
      });
    Hashtbl.add(types, path, td);
    order.lst = order.lst @ [path];
  | node =>
    Debug.add_node(`TypeAliasDeclaration(node));
    Debug.raise();
  };
}
and parse__wrapNonDeclarationChild = (~parent, node: Ts.node): ts_type => {
  Debug.add_pos("parse__wrapNonDeclarationChild");
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
  Debug.add_pos("parse__typeOfNode_exn");
  Debug.add_node_pair(parent, node);
  switch (node) {
  | `VoidKeyword(_) => Base(Void)
  | `StringKeyword(_) => Base(String)
  | `NumberKeyword(_) => Base(Number)
  | `BooleanKeyword(_) => Base(Boolean)
  | `AnyKeyword(_) => Base(Any)
  | `NullKeyword(_) => Base(Null)
  | `UndefinedKeyword(_) => Base(Undefined)
  | `NeverKeyword(_) => Base(Never)
  | `Parameter({name, _}) as p =>
    Debug.add("Parameter");
    parse__typeOfNode_exn(~isDeclarationChild, ~parent=p, name);
  | `Identifier({resolvedType: Some(resolvedType), _}) =>
    Debug.add("Identifier");
    parse__typeOfType(~isDeclarationChild, ~parent, resolvedType);
  | `TypeParameter({name, _}) as tp =>
    Debug.add("TypeParameter");
    parse__typeOfNode_exn(~isDeclarationChild, ~parent=tp, name);
  | `TypeReference({
      resolvedType: Some(`TypeParameter(_)),
      typeName:
        `Identifier({resolvedSymbol: Some({fullyQualifiedName, _}), _}),
      _,
    }) =>
    Debug.add("TypeReference");
    Debug.add("FullyQualifiedName(" ++ fullyQualifiedName ++ ")");
    Arg(fullyQualifiedName |> Ident.of_string);
  | `TypeLiteral(tl) as node =>
    Debug.add("TypeLiteral");
    isDeclarationChild
      ? parse__TypeLiteral(~parent, tl)
      : parse__wrapNonDeclarationChild(~parent, node);
  | otherNode =>
    Debug.add("OtherNode:");
    Debug.add_node(otherNode);
    let asGeneric = otherNode |> Typescript_unwrap.unwrap_Node;
    switch (asGeneric) {
    | {resolvedType: Some(resolvedType), _} =>
      Debug.add("Node->ResolvedType");
      parse__typeOfType(~isDeclarationChild, ~parent, resolvedType);
    | {resolvedSymbol: Some({resolvedType: Some(resolvedType), _}), _} =>
      Debug.add("Symbol->ResolvedType");
      parse__typeOfType(~isDeclarationChild, ~parent, resolvedType);
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
  Debug.add_pos("parse__typeOfType");
  Debug.add_pair(parent, type_def);
  switch (type_def) {
  | `String(_) => Base(String)
  | `Number(_) => Base(Number)
  | `Any(_) => Base(Any)
  | `Boolean(_) => Base(Boolean)
  | `StringLiteral({value, _}) => StringLiteral([value |> Ident.of_string])
  | `TypeParameter(_) =>
    switch (parent) {
    | `Parameter({type_: Some(type_), _}) =>
      parse__typeOfNode_exn(~isDeclarationChild, ~parent, type_)
    | otherParent =>
      switch (otherParent |> Typescript_unwrap.unwrap_Node) {
      | {
          resolvedType: Some(`TypeParameter(_)),
          resolvedSymbol: Some({fullyQualifiedName, _}),
          _,
        } =>
        Arg(fullyQualifiedName |> Ident.of_string)
      | generic =>
        Debug.add_node(parent);
        Debug.raiseWith("Could not resolve type parameter");
        Base(Any);
      }
    }
  | t =>
    Debug.raiseWith(
      ~node=parent,
      ~type_=t,
      "Could not resolve type '"
      ++ Debug.pp_type_kind(t)
      ++ "' in a meaningful way",
    );

    Base(Any);
  };
}
and parse__sourceFile = (sourceFile: Ts.node_SourceFile) => {
  sourceFile.statements
  |> CCList.iter(node => {
       Debug.set_current(node);
       parse__node(node);
     });
}
and parse = (source: string) => {
  switch (Typescript_j.node_of_string(source)) {
  | `SourceFile(sf) => parse__sourceFile(sf)
  | _ => raise(Not_found)
  };

  (order.lst, types);
};

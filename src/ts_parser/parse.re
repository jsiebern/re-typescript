module Ts = Typescript_t;
module Types = Re_typescript_printer.Tree_types;

module Debug = Parse_debug;

// -------------------------------------------------------------
// --- Todo ---
//
// - Try to order the types (typescript doesn't care where they are defined)
// - Function Overloading inside of an interface (check what tools Typescript offers to resolve that)
// - Check what reason representation makes sense for index signatures (esp. in combination with regular properties)
// - Implement Set / Map types (need some bindings in combination with Bucklescript)
// -------------------------------------------------------------

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
  // Special case, when only one member of CallSignature
  | `InterfaceDeclaration({
      members: [
        `CallSignature({
          end_,
          kind,
          kindName,
          flags,
          modifiers,
          decorators,
          resolvedSymbol,
          resolvedType,
          type_,
          parameters,
          typeArguments,
          _,
        }),
      ],
      pos,
      name,
      typeParameters,
      _,
    }) =>
    parse__FunctionDeclaration({
      pos,
      end_,
      kind,
      kindName,
      flags,
      modifiers,
      decorators,
      resolvedSymbol,
      resolvedType,
      name: Some(name),
      type_,
      parameters,
      typeParameters,
      typeArguments,
      questionToken: None,
      exclamationToken: None,
      body: None,
      asteriskToken: None,
    })
  | `InterfaceDeclaration(id) => parse__InterfaceDeclaration(id)
  | node => Debug.raise()
  };
}
and parse__SignatureDeclarationLike =
    (
      ~parent: Ts.node,
      ~parameters: list(Ts.node),
      ~return_type: option(Ts.node),
    ) => {
  Debug.add_pos("parse__SignatureDeclarationLike");
  Debug.add("Return:");
  let fu_return =
    switch (return_type) {
    | None => Base(Any) // No return: void
    | Some(dec_type) => parse__typeOfNode_exn(~parent, dec_type)
    };
  let fn = {
    fu_params:
      parameters
      |> CCList.mapi((i, param) => {
           Debug.add(Printf.sprintf("Param %i:", i));
           switch (param) {
           | `Parameter(
               (
                 {name: `Identifier({escapedText, _}), questionToken, _}: Ts.node_Parameter
               ),
             ) as param => {
               fp_name: escapedText |> Ident.of_string,
               fp_type:
                 switch (parse__typeOfNode(~parent, param)) {
                 | Some(t) => t
                 | None => raise(Not_found)
                 },
               fp_optional: questionToken |> CCOpt.is_some,
             }
           | _ => raise(Not_found)
           };
         }),
    fu_return,
  };
  Debug.close_pos();
  Function(fn);
}
and parse__FunctionDeclaration = (declaration: Ts.node_FunctionDeclaration) => {
  Debug.add_pos("parse__FunctionDeclaration");
  let parent = `FunctionDeclaration(declaration);
  let name =
    switch (declaration.name) {
    | Some(`Identifier({escapedText, _})) => escapedText
    | _ => raise(Not_found)
    };

  let td =
    Types.TypeDeclaration({
      td_name: name |> Ident.of_string,
      td_type:
        parse__SignatureDeclarationLike(
          ~parent,
          ~parameters=declaration.parameters,
          ~return_type=declaration.type_,
        ),
      td_parameters:
        parse__DeclarationTypeParameters(~parent, declaration.typeParameters),
    });
  let path = ([name], []);
  Hashtbl.add(types, path, td);
  order.lst = order.lst @ [path];
  Debug.close_pos();
  ();
}
and parse__Signature = (~parent: Ts.node, signature: Ts.node) => {
  Debug.add_pos("parse__Signature");
  switch (signature) {
  | `IndexSignature(_) => None
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
    let type_result =
      switch (parse__typeOfNode(~parent, pSig)) {
      | Some(t) => t
      | None =>
        Debug.add_node(pSig);
        Debug.raiseWith("PropertySignature type could not be resolved");
        Base(Any);
      };

    Debug.close_pos();
    Some((escapedText |> Ident.of_string, type_result));
  | node =>
    Debug.add_node(node);
    Debug.raiseWith("Signature not resolveable");
    Debug.close_pos();
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
      td_type:
        members |> CCList.is_empty
          ? {
            Base(Any);
          }
          : Interface(members, false),
      td_parameters:
        parse__DeclarationTypeParameters(~parent, declaration.typeParameters),
    });
  let path = ([name], []);
  Hashtbl.add(types, path, td);
  order.lst = order.lst @ [path];
  ();
  Debug.close_pos();
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
    let members =
      type_literal.members
      |> CCList.filter_map(member =>
           switch (parse__Signature(~parent, member)) {
           | None => None
           | Some((name, t)) => Some({f_name: name, f_type: t}: ts_field)
           }
         );
    Debug.close_pos();
    members |> CCList.is_empty
      ? {
        Base(Any);
      }
      : Interface(members, false);
  } else {
    Console.error("Currently invalid type literal:");
    Debug.add_node(`TypeLiteral(type_literal));
    Debug.close_pos();
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
  Debug.close_pos();
}
and parse__DeclarationTypeParameters = {
  Debug.add_pos("parse__DeclarationTypeParameters");
  (~parent: Ts.node, typeParameters: option(list(Ts.node))) => {
    let tp = {
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
    Debug.close_pos();
    tp;
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
    Debug.close_pos();
  | node =>
    Debug.add_node(`TypeAliasDeclaration(node));
    Debug.close_pos();
    Debug.raise();
  };
}
and parse__EntityName = (~parent, entity_name: Ts.node) => {
  Debug.add_pos("parse__EntityName");

  let rec run = (~carry=[], node: Ts.node) =>
    switch (node) {
    | `Identifier({escapedText, _}) => carry @ [escapedText]
    | `QualifiedName({left, right, _}) => run(~carry, left) @ run(right)
    | _ => carry
    };
  let local_name = run(entity_name) |> CCList.to_string(~sep=".", a => a);
  switch (entity_name) {
  | `Identifier({resolvedSymbol: Some({fullyQualifiedName, _}), _}) => (
      local_name,
      fullyQualifiedName,
    )
  | `Identifier({escapedText, _}) => (local_name, escapedText)
  | _ => (local_name, "")
  };
}
and parse__TypeReference = (~parent, type_ref: Ts.node_TypeReference): ts_type => {
  Debug.add_pos("parse__TypeReference");

  let args =
    type_ref.typeArguments
    |> CCOpt.map_or(
         ~default=[],
         CCList.filter_map(
           parse__typeOfNode(~parent=`TypeReference(type_ref)),
         ),
       );

  let (name, fullyQualifiedName) =
    parse__EntityName(~parent, type_ref.typeName);
  switch (type_ref.resolvedType, name) {
  | (Some(`TypeParameter(_)), _) =>
    Debug.add("TypeReference is TypeParameter");
    Debug.add("FullyQualifiedName(" ++ fullyQualifiedName ++ ")");
    Debug.close_pos();
    Arg(name |> Ident.of_string);
  | (_, "Function") => Base(GenericFunction)
  | (_, "Object") => Base(GenericObject)
  | (_, "Set") =>
    // TODO: Make this cleaner
    Set(args |> CCList.get_at_idx_exn(0))
  | _ =>
    Debug.add("FullyQualifiedName(" ++ fullyQualifiedName ++ ")");

    Debug.close_pos();
    Reference({
      tr_path: [name],
      tr_path_resolved: Some(([name], [])),
      tr_parameters: args,
    });
  };
}
and parse__UnionType = (~parent, types: list(Ts.node)) => {
  Debug.add_pos("parse__UnionType");

  let types =
    types
    |> CCList.mapi((i, type_node) => {
         Debug.add(Printf.sprintf("Union type %i:", i));
         let parsed_type =
           switch (parse__typeOfNode(~parent, type_node)) {
           | None =>
             Debug.raiseWith(
               ~node=type_node,
               "Could not resolve union type member",
             );
             Base(Any);
           | Some(t) => t
           };

         {
           um_ident: get_union_type_name(parsed_type),
           um_type: parsed_type,
           um_classifier: "",
         };
       });

  let result = Union(types);
  Debug.close_pos();
  result;
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
  Debug.close_pos();
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
  let result =
    switch (node) {
    | `VoidKeyword(_) => Base(Void)
    | `StringKeyword(_) => Base(String)
    | `NumberKeyword(_) => Base(Number)
    | `BooleanKeyword(_) => Base(Boolean)
    | `AnyKeyword(_) => Base(Any)
    | `NullKeyword(_) => Base(Null)
    | `UndefinedKeyword(_) => Base(Undefined)
    | `NeverKeyword(_) => Base(Never)
    | `ArrayType({elementType, _}) =>
      Array(parse__typeOfNode_exn(~isDeclarationChild, ~parent, elementType))
    | `UnionType({types, _}) as node =>
      Debug.add("UnionType");
      isDeclarationChild
        ? parse__UnionType(~parent, types)
        : parse__wrapNonDeclarationChild(~parent, node);
    | `FunctionType({type_, parameters, _})
    | `MethodSignature({type_, parameters, _}) =>
      Debug.add("MethodSignature");
      parse__SignatureDeclarationLike(
        ~parent,
        ~parameters,
        ~return_type=type_,
      );
    | `PropertySignature({type_: Some(type_node), _}) as p =>
      Debug.add("PropertySignature");
      parse__typeOfNode_exn(~isDeclarationChild, ~parent=p, type_node);
    | `Parameter({type_: Some(type_), _}) as p =>
      Debug.add("Parameter (with type)");
      parse__typeOfNode_exn(~isDeclarationChild, ~parent=p, type_);
    | `Parameter({name, _}) as p =>
      Debug.add("Parameter (no type)");
      parse__typeOfNode_exn(~isDeclarationChild, ~parent=p, name);
    | `Identifier({resolvedType: Some(resolvedType), _}) =>
      Debug.add("Identifier");
      parse__typeOfType(~isDeclarationChild, ~parent, resolvedType);
    | `TypeParameter({name, _}) as tp =>
      Debug.add("TypeParameter");
      parse__typeOfNode_exn(~isDeclarationChild, ~parent=tp, name);
    | `TypeReference(tr) =>
      Debug.add("TypeReference");
      parse__TypeReference(~parent, tr);
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
  Debug.close_pos();
  result;
}
and parse__typeOfType =
    (~isDeclarationChild=false, ~parent: Ts.node, type_def: Ts.type_)
    : Types.ts_type => {
  Debug.add_pos("parse__typeOfType");
  Debug.add_pair(parent, type_def);
  let result =
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
  Debug.close_pos();
  result;
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

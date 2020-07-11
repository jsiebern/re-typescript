module Ts = Typescript_t;
module Types = Re_typescript_printer.Tree_types;

module Debug = Parse_debug;
open Parse_utils;

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

let types: Hashtbl.t(identPath, ts_type) = Hashtbl.create(10);
type order = {
  mutable lst: list(identPath),
  mutable raw: list(Ts.node),
};
let order = {lst: [], raw: []};

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
          typeNode,
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
      typeNode,
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
and register__TypeDeclaration = (~identChain, type_dec) => {
  Debug.add_pos("register__TypeDeclaration");
  Hashtbl.add(types, identChain, type_dec);
  order.lst = order.lst @ [identChain];
  Debug.close_pos();
}
and parse__SignatureDeclarationLike =
    (
      ~parent: Ts.node,
      ~identChain,
      ~parameters: list(Ts.node),
      ~return_type: option(Ts.node),
    ) => {
  Debug.add_pos("parse__SignatureDeclarationLike");
  Debug.add("Return:");
  let fu_return =
    switch (return_type) {
    | None => Base(Any) // No return: void
    | Some(dec_type) =>
      parse__typeOfNode_exn(
        ~parent,
        ~identChain=identChain @ ["return"],
        dec_type,
      )
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
                 switch (
                   parse__typeOfNode(
                     ~parent,
                     ~identChain=identChain @ [escapedText],
                     param,
                   )
                 ) {
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
  let name = identifierOfNode(parent);
  let identChain = [name];

  let td =
    Types.TypeDeclaration({
      td_name: name |> Ident.of_string,
      td_type:
        parse__SignatureDeclarationLike(
          ~parent,
          ~identChain,
          ~parameters=declaration.parameters,
          ~return_type=declaration.type_,
        ),
      td_parameters:
        parse__DeclarationTypeParameters(
          ~parent,
          ~identChain,
          declaration.typeParameters,
        ),
    });
  Debug.close_pos();
  register__TypeDeclaration(~identChain, td);
}
and parse__Signature = (~parent: Ts.node, ~identChain, signature: Ts.node) => {
  Debug.add_pos("parse__Signature");
  let isOptional =
    switch (signature) {
    | `PropertySignature({questionToken: Some(_), _}) => true
    | _ => false
    };
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
      switch (
        parse__typeOfNode(
          ~parent,
          ~identChain=identChain @ [escapedText],
          pSig,
        )
      ) {
      | Some(t) => t
      | None =>
        Debug.add_node(pSig);
        Debug.raiseWith("PropertySignature type could not be resolved");
        Base(Any);
      };

    Debug.close_pos();
    Some((
      escapedText |> Ident.of_string,
      isOptional ? Optional(type_result) : type_result,
    ));
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
  let name = identifierOfNode(parent);
  let identChain = [name];

  let members =
    declaration.members
    |> CCList.filter_map(member =>
         switch (parse__Signature(~parent, ~identChain, member)) {
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
        parse__DeclarationTypeParameters(
          ~parent,
          ~identChain,
          declaration.typeParameters,
        ),
    });
  Debug.close_pos();
  register__TypeDeclaration(~identChain, td);
}
and parse__IntersectionType =
    (
      ~parent: Ts.node,
      ~identChain,
      intersection_type: Ts.node_IntersectionType,
    ) => {
  Debug.add_pos("parse__IntersectionType");

  let types =
    intersection_type.types
    |> CCList.mapi((i, type_node) => {
         Debug.add(Printf.sprintf("Intersection type %i:", i));
         let parsed_type =
           switch (
             parse__typeOfNode(
               ~parent,
               ~identChain=identChain @ [string_of_int(i + 1)],
               type_node,
             )
           ) {
           | None =>
             Debug.raiseWith(
               ~node=type_node,
               "Could not resolve intersection type member",
             );
             Base(Any);
           | Some(t) => t
           };
         parsed_type;
       });
  let asTuple = Tuple(types);
  Debug.close_pos();
  asTuple;
}
and parse__LiteralType =
    (~parent: Ts.node, ~identChain, literal_type: Ts.node_LiteralType) => {
  Debug.add_pos("parse__LiteralType");
  let lt =
    switch (literal_type.literal) {
    | `TrueKeyword(_) => Literal(Boolean(true))
    | `FalseKeyword(_) => Literal(Boolean(false))
    | `NumericLiteral({text, _})
    | `FirstLiteralToken({text, _}) => Literal(Number(float_of_string(text)))
    | otherNode =>
      switch (literal_type.resolvedType) {
      | Some(resolvedType) =>
        parse__typeOfType(~parent, ~identChain, resolvedType)
      | None =>
        Debug.raiseWith(~node=otherNode, "Could not parse literal type node");
        Base(Any);
      }
    };
  Debug.close_pos();
  lt;
}
and parse__TypeLiteral =
    (~parent: Ts.node, ~identChain, type_literal: Ts.node_TypeLiteral) => {
  Debug.add_pos("parse__TypeLiteral");
  let members =
    type_literal.members
    |> CCList.filter_map(member =>
         switch (parse__Signature(~parent, ~identChain, member)) {
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
}
and parse__EnumDeclaration = (declaration: Ts.node_EnumDeclaration) => {
  Debug.add_pos("parse__EnumDeclaration");
  let parent = `EnumDeclaration(declaration);
  let name = identifierOfNode(parent);
  let identChain = [name];
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
  Debug.close_pos();
  register__TypeDeclaration(~identChain, td);
}
and parse__DeclarationTypeParameters =
    (
      ~parent: Ts.node,
      ~identChain,
      ~target_type: option(ts_type)=?,
      typeParameters: option(list(Ts.node)),
    ) => {
  Debug.add_pos("parse__DeclarationTypeParameters");
  let typeParameters = Some(typeParametersOfNodeRec(parent));
  let tp = {
    typeParameters
    |> CCOpt.map_or(~default=[], params =>
         params
         |> CCList.mapi((i, param) => {
              let name = identifierOfNode(param);
              {
                tp_name: Ident.of_string(name),
                tp_extends: None,
                tp_default: None /* switch (param) {
                      | `TypeParameter({default: Some(default), _}) => parse__typeOfNode(~parent, ~identChain=identChain @ [name], default)
                      | _ => None
                    }*/
              };
            })
       );
  };
  let tp =
    tp
    @ (
      switch (target_type) {
      | Some(Reference({tr_parameters, _})) =>
        tr_parameters
        |> CCList.filter_map(
             fun
             | Arg(ident) =>
               Some({tp_name: ident, tp_extends: None, tp_default: None})
             | _ => None,
           )
      | _ => []
      }
    );
  Debug.close_pos();
  tp;
}
and parse__TypeAliasDeclaration =
    (~identChain=[], declaration: Ts.node_TypeAliasDeclaration) => {
  Debug.add_pos("parse__TypeAliasDeclaration");
  let parent = `TypeAliasDeclaration(declaration);
  let name = Parse_utils.identifierOfNode(parent);
  let identChain = identChain |> CCList.is_empty ? [name] : identChain;
  let type_ =
    parse__typeOfNode(
      ~isDeclarationChild=true,
      ~parent,
      ~identChain,
      declaration.type_,
    );
  let td =
    Types.TypeDeclaration({
      td_name: name |> Ident.of_string,
      td_type: type_ |> CCOpt.get_or(~default=Base(Any)),
      td_parameters:
        parse__DeclarationTypeParameters(
          ~parent,
          ~identChain,
          ~target_type=?type_,
          declaration.typeParameters,
        ),
    });
  Debug.close_pos();
  register__TypeDeclaration(~identChain, td);
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
and parse__TypeReference =
    (~parent, ~identChain, type_ref: Ts.node_TypeReference): ts_type => {
  Debug.add_pos("parse__TypeReference");

  let resolvedParameters =
    switch (Typescript_unwrap.unwrap_Node(type_ref.typeName).resolvedSymbol) {
    | Some({id: Some(id), _}) =>
      findNodeForSymbolId(order.raw, id)
      |> CCOpt.map(typeParametersOfNodeRec)
      |> CCOpt.value(~default=[])
    | _ => []
    };
  let arguments = type_ref.typeArguments |> CCOpt.value(~default=[]);

  let args =
    (
      if (CCList.length(resolvedParameters) === 0) {
        arguments
        |> CCList.mapi((i, arg) =>
             parse__typeOfNode(
               ~parent=`TypeReference(type_ref),
               ~identChain=identChain @ [string_of_int(i + 1)],
               arg,
             )
           );
      } else {
        resolvedParameters
        |> CCList.mapi((i, param) =>
             switch (param, arguments |> CCList.get_at_idx(i)) {
             | (
                 `TypeParameter(({name, _}: Ts.node_TypeParameter)),
                 Some(arg),
               ) =>
               parse__typeOfNode(
                 ~parent=`TypeReference(type_ref),
                 ~identChain=identChain @ [identifierOfNode(name)],
                 arg,
               )
             | (
                 `TypeParameter(
                   ({name, default: Some(arg), _}: Ts.node_TypeParameter),
                 ),
                 None,
               ) =>
               parse__typeOfNode(
                 ~parent=`TypeReference(type_ref),
                 ~identChain=[
                   identifierOfNode(type_ref.typeName),
                   identifierOfNode(name),
                 ],
                 arg,
               )
             | (`TypeParameter(({name, _}: Ts.node_TypeParameter)), None) =>
               Some(Arg(identifierOfNode(name) |> Ident.of_string))
             | (node, _) =>
               Debug.add_node(node);
               Debug.raiseWith(~node, "Invalid TypeParameter");
               None;
             }
           );
      }
    )
    |> CCList.keep_some;

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
  | (_, "Array") => Array(args |> CCList.get_at_idx_exn(0))
  | (_, "Set") =>
    // TODO: Make this cleaner
    Set(args |> CCList.get_at_idx_exn(0))
  | _ =>
    Debug.add("FullyQualifiedName(" ++ fullyQualifiedName ++ ")");

    Debug.close_pos();
    Reference({
      tr_from: identChain,
      // Todo: This should be a full list incl. import follows like modules etc.
      tr_path: [name],
      tr_path_resolved: Some(([name], [])),
      tr_parameters: args,
    });
  };
}
and parse__TupleType = (~parent, ~identChain, elements: list(Ts.node)) => {
  Debug.add_pos("parse__TupleType");

  let elements =
    elements
    |> CCList.mapi((i, type_node) => {
         Debug.add(Printf.sprintf("Tuple type %i:", i));
         let parsed_type =
           switch (
             parse__typeOfNode(
               ~parent,
               ~identChain=identChain @ [string_of_int(i + 1)],
               type_node,
             )
           ) {
           | None =>
             Debug.raiseWith(
               ~node=type_node,
               "Could not resolve Tuple type member",
             );
             Base(Any);
           | Some(t) => t
           };
         parsed_type;
       });
  let asTuple = Tuple(elements);
  Debug.close_pos();
  asTuple;
}
and parse__UnionType = (~parent, ~identChain, types: list(Ts.node)) => {
  Debug.add_pos("parse__UnionType");

  let parsed_types =
    types
    |> CCList.mapi((i, type_node) => {
         Debug.add(Printf.sprintf("Union type %i:", i));
         switch (
           parse__typeOfNode(
             ~parent,
             ~identChain=identChain @ [string_of_int(i + 1)],
             type_node,
           )
         ) {
         | None =>
           Debug.raiseWith(
             ~node=type_node,
             "Could not resolve union type member",
           );
           Base(Any);
         | Some(t) => t
         };
       });

  let result =
    switch (parsed_types) {
    | [] => Base(Never)
    | [one] => one
    | parsed_types =>
      if (parsed_types
          |> CCList.for_all(
               fun
               | Literal(String(_)) => true
               | _ => false,
             )) {
        // String Literal
        StringLiteral(
          parsed_types
          |> CCList.map(
               fun
               | Literal(String(ident)) => ident
               | _ => raise(Failure("Unexpected")),
             ),
        );
      } else if (parsed_types
          |> CCList.for_all(
               fun
               | Literal(Number(_)) => true
               | _ => false,
             )) {
        // Numeric Literal
        NumericLiteral(
          parsed_types
          |> CCList.map(
               fun
               | Literal(Number(value)) => int_of_float(value)
               | _ => raise(Failure("Unexpected")),
             ),
        );
      } else if (parsed_types
          |> CCList.for_all(
               fun
               | Literal(_) => true
               | _ => false,
             )) {
        // Mixed Literal
        let mixedLiteral = parsed_types
          |> CCList.fold_left((p) =>
               fun
               | Literal(Number(value)) => {...p, numbers: [int_of_float(value), ...p.numbers] }
               | Literal(String(ident)) => {...p, strings: [ident, ...p.strings]}
               | Literal(Boolean(b)) => {...p, bools: [b, ...p.bools]}
               | _ => raise(Failure("Unexpected")),
             { strings: [], numbers: [], bools: [] });
        MixedLiteral({
          strings: mixedLiteral.strings |> CCList.rev,
          numbers: mixedLiteral.numbers |> CCList.rev,
          bools: mixedLiteral.bools |> CCList.rev,
        })
      } else {
        // Typed union
        Union(
          parsed_types
          |> CCList.map(parsed_type =>
               {
                 um_ident: get_union_type_name(parsed_type),
                 um_type: parsed_type,
                 um_classifier: "",
               }
             ),
        );
      }
    };

  Debug.close_pos();
  result;
}
and parse__IndexedAccessType =
    (~parent, ~identChain, iat: Ts.node_IndexedAccessType) => {
  Debug.add_pos("parse__IndexedAccessType");
  let parent = `IndexedAccessType(iat);
  let index =
    switch (parse__typeOfNode(~parent, ~identChain, iat.indexType)) {
    | Some(Literal(String(ident))) => Ident.ident(ident)
    | Some(Literal(Number(num))) => Printf.sprintf("_%f", num)
    | _ => "t"
    };
  let identChain = identChain @ [index];

  let objNode = CCOpt.get_exn(iat.typeNode);

  let result =
    switch (parse__typeOfNode(~parent, ~identChain, objNode)) {
    | Some(result) => result
    | None =>
      Debug.raiseWith(
        ~node=iat.objectType,
        "Could not parse indexed access target",
      );
      Base(Any);
    };
  Debug.close_pos();
  result;
}
and parse__wrapNonDeclarationChild =
    (~parent, ~identChain, node: Ts.node): ts_type => {
  Debug.add_pos("parse__wrapNonDeclarationChild");
  let values = node |> Typescript_unwrap.unwrap_Node;
  let identName = identChain |> CCList.to_string(~sep="_", a => a);
  let typeParams = typeParametersOfNodeRec(node);
  parse__TypeAliasDeclaration(
    ~identChain,
    {
      pos: values.pos,
      end_: values.end_,
      // Fix: Retrieve real kind
      kind: (-1),
      kindName: "TypeAliasDeclaration",
      typeNode: None,
      // Fix
      flags: Typescript_flags.Node.const |> Obj.magic,
      modifiers: None,
      decorators: None,
      resolvedSymbol: values.resolvedSymbol,
      resolvedType: None,
      name:
        `Identifier({
          resolvedType: None,
          typeNode: None,
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
      typeParameters: Some(typeParams),
      type_: node,
    },
  );
  Debug.close_pos();
  Reference({
    tr_from: identChain |> CCList.take(CCList.length(identChain) - 1),
    tr_path: identChain,
    tr_path_resolved: Some(([identName], [])),
    tr_parameters:
      typeParams
      |> CCList.filter_map(param =>
           Typescript_unwrap.unwrap_Name(param)
           |> CCOpt.map(name => Arg(Ident.of_string(name)))
         ),
  });
}
and parse__typeOfNode_forEach =
    (
      ~isDeclarationChild=false,
      ~identChain,
      ~parent: Ts.node,
      nodes: list(Ts.node),
    )
    : option(ts_type) => {
  nodes
  |> CCList.find_map(
       parse__typeOfNode(~isDeclarationChild, ~identChain, ~parent),
     );
}
and parse__typeOfNode =
    (~isDeclarationChild=false, ~identChain, ~parent: Ts.node, node: Ts.node)
    : option(ts_type) =>
  try(
    Some(
      parse__typeOfNode_exn(~isDeclarationChild, ~identChain, ~parent, node),
    )
  ) {
  | Not_found => None
  }
and parse__typeOfNode_exn =
    (~isDeclarationChild=false, ~identChain, ~parent: Ts.node, node: Ts.node)
    : ts_type => {
  Debug.add_pos("parse__typeOfNode_exn");
  Debug.add_node_pair(parent, node);
  let result =
    switch (node) {
    | `ParenthesizedType({type_, _}) =>
       parse__typeOfNode_exn(~isDeclarationChild, ~identChain, ~parent, type_)
    | `VoidKeyword(_) => Base(Void)
    | `StringKeyword(_) => Base(String)
    | `NumberKeyword(_) => Base(Number)
    | `BooleanKeyword(_) => Base(Boolean)
    | `AnyKeyword(_) => Base(Any)
    | `NullKeyword(_) => Base(Null)
    | `UndefinedKeyword(_) => Base(Undefined)
    | `NeverKeyword(_) => Base(Never)
    | `FirstLiteralToken({text, _})
    | `NumericLiteral({text,_}) => raise(Failure(text))
    | `ArrayType({elementType, _}) =>
      Array(
        parse__typeOfNode_exn(
          ~identChain=identChain @ ["t"],
          ~parent,
          elementType,
        ),
      )
    | `TupleType({elementTypes, _}) as node =>
      Debug.add("TupleType");
      isDeclarationChild
        ? parse__TupleType(~parent, ~identChain, elementTypes)
        : parse__wrapNonDeclarationChild(~parent, ~identChain, node);
    | `UnionType({types, _} as unionNode) =>
      // Union get's a little bit of a special treatment (because of null / undefined)
      switch (
        types
        |> CCList.partition(
             fun
             | `UndefinedKeyword(_) => true
             | _ => false,
           )
      ) {
      | ([_, ..._], rest) =>
        Debug.add("Union: optional");
        Optional(
          parse__typeOfNode_exn(
            ~identChain=["t_optional", ...identChain],
            ~parent,
            `UnionType({...unionNode, types: rest}),
          ),
        );
      | ([], types) =>
        switch (
          types
          |> CCList.partition(
               fun
               | `NullKeyword(_) => true
               | _ => false,
             )
        ) {
        | ([_, ..._], rest) =>
          Debug.add("Union: Nullable");
          Nullable(
            parse__typeOfNode_exn(
              ~identChain=["t_nullable", ...identChain],
              ~parent,
              `UnionType({...unionNode, types: rest}),
            ),
          );
        | ([], types) =>
          Debug.add("UnionType");
          let identChain =
            switch (identChain) {
            | ["t_nullable", "t_optional", ...rest]
            | ["t_nullable", ...rest]
            | ["t_optional", ...rest] => rest @ ["t"]
            | other => other
            };
          switch (types) {
          | [] => Base(Never)
          | [one] => parse__typeOfNode_exn(~parent, ~identChain, one)
          | more =>
            isDeclarationChild
              ? parse__UnionType(~parent, ~identChain, more)
              : parse__wrapNonDeclarationChild(
                  ~parent,
                  ~identChain,
                  `UnionType({...unionNode, types: more}),
                )
          };
        }
      }
    | `IndexedAccessType(iat) =>
      Debug.add("IndexedAccessType");
      parse__IndexedAccessType(~parent, ~identChain, iat);
    | `FunctionType({type_, parameters, typeParameters, _})
    | `MethodSignature({type_, parameters, typeParameters, _}) =>
      Debug.add("FunctionType / MethodSignature");
      parse__SignatureDeclarationLike(
        ~parent,
        ~identChain,
        ~parameters,
        ~return_type=type_,
      );
    | `PropertySignature({type_: Some(type_node), _}) as p =>
      Debug.add("PropertySignature");
      parse__typeOfNode_exn(
        ~isDeclarationChild,
        ~identChain,
        ~parent=p,
        type_node,
      );
    | `Parameter({type_: Some(type_), _}) as p =>
      Debug.add("Parameter (with type)");
      parse__typeOfNode_exn(
        ~isDeclarationChild,
        ~parent=p,
        ~identChain,
        type_,
      );
    | `Parameter({name, _}) as p =>
      Debug.add("Parameter (no type)");
      parse__typeOfNode_exn(
        ~isDeclarationChild,
        ~parent=p,
        ~identChain,
        name,
      );
    | `Identifier({resolvedType: Some(resolvedType), _}) =>
      Debug.add("Identifier");
      parse__typeOfType(
        ~isDeclarationChild,
        ~parent,
        ~identChain,
        resolvedType,
      );
    | `TypeParameter({name, _}) as tp =>
      Debug.add("TypeParameter");
      parse__typeOfNode_exn(
        ~isDeclarationChild,
        ~parent=tp,
        ~identChain,
        name,
      );
    | `TypeReference(tr) =>
      Debug.add("TypeReference");
      parse__TypeReference(~parent, ~identChain, tr);
    | `LiteralType(lt) =>
      Debug.add("LiteralType");
      parse__LiteralType(~parent, ~identChain, lt);
    | `TypeLiteral(tl) as node =>
      Debug.add("TypeLiteral");
      isDeclarationChild
        ? parse__TypeLiteral(~parent, ~identChain, tl)
        : parse__wrapNonDeclarationChild(~parent, ~identChain, node);
    | `IntersectionType(it) as node =>
      isDeclarationChild
        ? parse__IntersectionType(~parent, ~identChain, it)
        : parse__wrapNonDeclarationChild(~parent, ~identChain, node)
    | otherNode =>
      Debug.add("OtherNode:");
      Debug.add_node(otherNode);
      let asGeneric = otherNode |> Typescript_unwrap.unwrap_Node;
      switch (asGeneric) {
      | {resolvedType: Some(resolvedType), _} =>
        Debug.add("Node->ResolvedType");
        parse__typeOfType(
          ~isDeclarationChild,
          ~parent,
          ~identChain,
          resolvedType,
        );
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
    (
      ~isDeclarationChild=false,
      ~parent: Ts.node,
      ~identChain,
      type_def: Ts.type_,
    )
    : Types.ts_type => {
  Debug.add_pos("parse__typeOfType");
  Debug.add_pair(parent, type_def);
  let result =
    switch (type_def) {
    | `String(_) => Base(String)
    | `Number(_) => Base(Number)
    | `Any(_) => Base(Any)
    | `Boolean(_) => Base(Boolean)
    // Todo: | `EnumLiteral(_)
    | `StringLiteral({value, _}) =>
      Literal(String(value |> Ident.of_string))
    | `NumberLiteral({value, _}) => Literal(Number(value))
    | `TypeParameter(_) =>
      switch (parent) {
      | `Parameter({type_: Some(type_), _}) =>
        parse__typeOfNode_exn(
          ~isDeclarationChild,
          ~parent,
          ~identChain,
          type_,
        )
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
  order.raw = sourceFile.statements;
  order.raw
  |> CCList.iter(node => {
       Debug.set_current(node);
       parse__node(node);
     });
}
and parse = (source: string) => {
  Hashtbl.clear(types);
  order.lst = [];
  order.raw = [];

  switch (Typescript_j.node_of_string(source)) {
  | `SourceFile(sf) => parse__sourceFile(sf)
  | _ => raise(Not_found)
  };

  let order = order.lst;
  //Parse_optimizer.debug_enabled := true;
  let (order, types) = Parse_optimizer.optimizeReferences(order, types);

  let pathMap = Hashtbl.create(Hashtbl.length(types));
  Hashtbl.iter((path, t) => {Hashtbl.add(pathMap, (path, []), t)}, types);

  (CCList.map(l => (l, []), order), pathMap);
};

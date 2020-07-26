open Ast;

let identifier: type t. Identifier.t(t) => string =
  i =>
    switch (i) {
    | TypeParameter(str) => Printf.sprintf("TypeParameter(%s)", str)
    | Module(str) => Printf.sprintf("Module(%s)", str)
    | TypeName(str) => Printf.sprintf("TypeName(%s)", str)
    | PropertyName(str) => Printf.sprintf("PropertyName(%s)", str)
    | VariantIdentifier(str) => Printf.sprintf("VariantIdentifier(%s)", str)
    | SubName(str) => Printf.sprintf("SubName(%s)", str)
    | SubIdent(str) => Printf.sprintf("SubIdent(%i)", str)
    };
let path = (p: Identifier.path) =>
  CCArray.to_string(~sep=" -> ", i => identifier(i), p);

let rec ast_node: type t. Node.node(t) => string =
  n =>
    switch (n) {
    | Module(_) => "Module"
    | Literal(t) =>
      Printf.sprintf(
        "Literal(%s)",
        switch (t) {
        | String(str) => Printf.sprintf("String<%s>", str)
        | Number(n) => Printf.sprintf("Number<%.2f>", n)
        | Boolean(b) => Printf.sprintf("Boolean<%s>", b ? "Y" : "N")
        },
      )
    | SafeDict(t) => Printf.sprintf("SafeDict(%s)", ast_node(t))
    | GenericReference(i) =>
      Printf.sprintf(
        "GenericReference(%s)",
        Ast_generator_utils.Naming.fromIdentifier(i),
      )
    | Basic(b) =>
      Printf.sprintf(
        "Basic(%s)",
        switch (b) {
        | RelevantKeyword(word) =>
          Printf.sprintf("RelevantKeyword<%s>", word)
        | String => "String"
        | Number => "Number"
        | Boolean => "Boolean"
        | Void => "Void"
        | Any => "Any"
        | Null => "Null"
        | Undefined => "Undefined"
        | Never => "Never"
        | This => "This"
        },
      )
    | TypeDeclaration({name, _}) =>
      Printf.sprintf("TypeDeclaration(%s)", identifier(name))
    | Array(t) => Printf.sprintf("Array(%s)", ast_node(t))
    | Optional(t) => Printf.sprintf("Optional(%s)", ast_node(t))
    | Nullable(t) => Printf.sprintf("Nullable(%s)", ast_node(t))
    | Reference({target, _}) =>
      Printf.sprintf(
        "Reference(%s)",
        Ast_generator_utils.Naming.full_identifier_of_path(target),
      )
    | Variant(variants) =>
      Printf.sprintf(
        "Variant(%s)",
        variants
        |> CCArray.to_string(~sep=", ", (v: VariantConstructor.t) =>
             identifier(v.name)
           ),
      )
    | Fixture(_) => "Fixture"
    | Tuple(_) => "Tuple"
    | Function(_) => "Function"
    | Parameter({name, _}) =>
      Printf.sprintf("Parameter(%s)", identifier(name))
    | Record(_) => "Record"
    };

let node_kind = (n: Ts_nodes.nodeKind) =>
  Ts_nodes_util.unwrap_identified(n)#getKindName();

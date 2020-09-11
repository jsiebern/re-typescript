open Ast;
open Parser_utils;

let identifier: type t. Identifier.t(t) => string =
  i =>
    switch (i) {
    | TypeParameter(str) => Printf.sprintf("TypeParameter(%s)", str)
    | Module(str) => Printf.sprintf("Module(%s)", str)
    | TypeName(str) => Printf.sprintf("TypeName(%s)", str)
    | PropertyName(str) => Printf.sprintf("PropertyName(%s)", str)
    | VariantIdentifier(str, _) =>
      Printf.sprintf("VariantIdentifier(%s)", str)
    | SubName(str) => Printf.sprintf("SubName(%s)", str)
    | SubIdent(str) => Printf.sprintf("SubIdent(%i)", str)
    };
let path = (p: Identifier.path) =>
  CCArray.to_string(~sep=" -> ", i => identifier(i), p);

let rec ast_node: type t. (~level: int=?, Node.node(t)) => string =
  (~level=0, n) => {
    let spacer = CCString.repeat("  ", level);

    let rendered =
      switch (n) {
      | Module({types, name, _}) =>
        Printf.sprintf(
          "Module<%s>\n%s",
          name,
          types |> CCArray.to_string(~sep="\n", ast_node(~level=level + 1)),
        )
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
        }
      | TypeDeclaration({name, annot, _}) =>
        Printf.sprintf(
          "TypeDeclaration<%s>\n%s",
          identifier(name),
          ast_node(~level=level + 1, annot),
        )
      | Array(t) => Printf.sprintf("Array(%s)", ast_node(t))
      | Optional(t) => Printf.sprintf("Optional(%s)", ast_node(t))
      | Nullable(t) => Printf.sprintf("Nullable(%s)", ast_node(t))
      | Reference({target, params}) =>
        Printf.sprintf(
          "Reference(%s, [%s])",
          path(target),
          params
          |> CCList.to_string(~sep=", ", ((v, t)) =>
               v ++ ": " ++ ast_node(t)
             ),
        )
      | Variant(variants, _) =>
        Printf.sprintf(
          "Variant(%s)",
          variants
          |> CCArray.to_string(~sep=", ", (v: VariantConstructor.t) =>
               identifier(v.name)
             ),
        )
      | Fixture(_) => "Fixture"
      | Tuple(_) => "Tuple"
      | Function({parameters, return_type}) =>
        Printf.sprintf(
          "Function(%s): %s",
          parameters |> CCArray.to_string(~sep=", ", t => ast_node(t)),
          ast_node(return_type),
        )
      | Parameter({name, type_, _}) =>
        Printf.sprintf(
          "Parameter(%s: %s)",
          Ast_generator_utils.Naming.unwrap(name),
          ast_node(type_),
        )
      | Record(fields) =>
        Printf.sprintf(
          "Record\n%s",
          fields |> CCArray.to_string(~sep="\n", ast_node(~level=level + 1)),
        )
      };
    Printf.sprintf("%s%s%s", spacer, level > 0 ? "- " : "", rendered);
  };

let node_kind = (n: Ts_nodes.nodeKind) =>
  Ts_nodes_util.unwrap_identified(n)#getKindName();

let scope_args = (scope: scope) =>
  Printf.sprintf(
    "-- Args --\n%s\n-- /Args --\n",
    CCList.to_string(
      ~sep="\n",
      ((key, node)) => Printf.sprintf("%s: %s", key, ast_node(node)),
      scope.context_args,
    ),
  );
let scope_params = (scope: scope) =>
  Printf.sprintf(
    "-- Params --\n%s\n-- /Params --\n",
    CCList.to_string(
      ~sep="\n",
      ((key, node)) =>
        Printf.sprintf(
          "%s: %s",
          key,
          node |> CCOpt.map(ast_node) |> CCOpt.value(~default="-"),
        ),
      scope.context_params,
    ),
  );
let scope_nodes = (scope: scope) =>
  Printf.sprintf(
    "-- Nodes --\n%s\n-- /Nodes --\n",
    CCArray.to_string(~sep="\n", ast_node, scope.root_declarations),
  );

let scope_refs = (scope: scope) =>
  Printf.sprintf(
    "-- Refs --\n%s\n-- /Refs --\n",
    scope.refs
    |> CCHashtbl.to_list
    |> CCList.to_string(~sep="\n", ((to_, from)) =>
         Printf.sprintf(
           "%s: [%s]",
           Ast_generator_utils.Naming.full_identifier_of_path(to_),
           from
           |> CCArray.to_string(
                ~sep=", ",
                Ast_generator_utils.Naming.full_identifier_of_path,
              ),
         )
       ),
  );

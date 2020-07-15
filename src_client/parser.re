// TODO: Eventually define an interface so that multiple backends can be built
// ALso: Plugin system should be solved here as well, before the AST is built
// Could use a similar system to graphql-ppx (see https://github.com/reasonml-community/graphql-ppx/blob/master/src/base/validations.re for reference)
open Ast;
open Node;
module Exceptions = {
  exception UnexpectedAtThisPoint(string);
};

// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Utility types
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
type parse_config = {exports_only: bool};
type runtime = {
  root_modules: array(Node.node(Node.Constraint.exactlyModule)),
  node_count: int,
  parse_config,
};
type scope = {
  source_file: option(Ts_nodes.SourceFile.t),
  root_declarations: array(Node.node(Node.Constraint.moduleLevel)),
  parent: option(Ts_nodes.nodeKind),
  path: Identifier.path,
  has_any: bool,
};
module Runtime = {
  let add_root_module =
      (root_module: Node.node(Node.Constraint.exactlyModule), runtime) => {
    ...runtime,
    root_modules: CCArray.append(runtime.root_modules, [|root_module|]),
  };
  let incr_node = runtime => {...runtime, node_count: runtime.node_count + 1};
};
module Scope = {
  let add_to_path: (Identifier.t(_), scope) => scope =
    (i, scope) => {...scope, path: CCArray.append(scope.path, [|i|])};
  let replace_path: (Identifier.t(_), scope) => scope =
    (i, scope) => {...scope, path: [|i|]};
  let replace_path_arr: (array(Identifier.t(_)), scope) => scope =
    (path, scope) => {...scope, path};
  let add_root_declaration =
      (root_declaration: Node.node(Node.Constraint.moduleLevel), scope) => {
    {
      ...scope,
      root_declarations:
        CCArray.append(scope.root_declarations, [|root_declaration|]),
    };
  };
};
module Path = {
  type t = Identifier.path;
  let hd = (p: t) => {
    CCArray.get_safe(p, CCArray.length(p) - 1);
  };
  let add = (i, p: t) => CCArray.append(p, [|i|]);
  let hd_unsafe = (p: t) => CCArray.get(p, CCArray.length(p) - 1);
  let is_sub = i =>
    switch (i) {
    | Identifier.SubIdent(_)
    | Identifier.SubName(_) => true
    | _ => false
    };
  let unwrap: type t. Identifier.t(t) => string =
    i =>
      switch (i) {
      | Module(str)
      | TypeName(str)
      | PropertyName(str)
      | VariantIdentifier(str)
      | SubName(str) => str
      | SubIdent(i) => string_of_int(i)
      };
  let fold_fun:
    type t.
      (Identifier.t(t), (array(string), bool)) => (array(string), bool) =
    (i, (arr, finished)) =>
      finished
        ? (arr, finished)
        : (
          switch (i) {
          | PropertyName(str)
          | SubName(str) => (CCArray.append([|str|], arr), false)
          | SubIdent(num) => (
              CCArray.append([|string_of_int(num)|], arr),
              false,
            )
          | TypeName(str) => (CCArray.append([|str|], arr), true)
          | Identifier.VariantIdentifier(_)
          | Module(_) => (arr, true)
          }
        );
  let make_sub_type_name = (p: t) => {
    Array.fold_right(fold_fun, p, ([||], false))
    |> fst
    |> CCArray.to_string(~sep="_", a => a);
  };
};

// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Entry / Preparation
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
let rec parse__Entry = (~source_files: array(Ts_morph.SourceFile.t)) => {
  let runtime = {
    root_modules: [||],
    node_count: 0,
    parse_config: {
      exports_only: false,
    },
  };
  let runtime =
    source_files
    |> CCArray.fold_left(
         (runtime, source_file) => {
           let scope = {
             root_declarations: [||],
             source_file: None,
             parent: None,
             path: [||],
             has_any: false,
           };

           let source_file =
             source_file
             |> Ts_morph.SourceFile.castToNode
             |> Ts_nodes.Generic.fromMorphNode
             |> Ts_nodes.SourceFile.fromGeneric;
           let (runtime, _, t) =
             parse__Node__SourceFile(~runtime, ~scope, source_file);
           runtime |> Runtime.add_root_module(t);
         },
         runtime,
       );
  runtime.root_modules;
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Generic Node ("Hub")
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__Generic =
    (~runtime, ~scope, node: Ts_nodes.Generic.t)
    : (runtime, scope, Node.node(Node.Constraint.any)) => {
  let is_sub =
    Path.hd(scope.path) |> CCOpt.map_or(~default=false, Path.is_sub);
  let identifiedNode = Ts_nodes_util.identifyGenericNode(node);
  switch (identifiedNode) {
  | StringKeyword(_)
  | NumberKeyword(_)
  | NeverKeyword(_)
  | NullKeyword(_)
  | ObjectKeyword(_)
  | UndefinedKeyword(_)
  | BooleanKeyword(_)
  | VoidKeyword(_)
  | AnyKeyword(_) => parse__Node__Basic(~runtime, ~scope, identifiedNode)
  | ArrayType(_) => parse__Node__Array(~runtime, ~scope, identifiedNode)
  | TupleType(_) => parse__Node__Tuple(~runtime, ~scope, identifiedNode)
  | TypeLiteral(_) when is_sub =>
    parse__Node__Generic__WrapSubNode(~runtime, ~scope, node)
  | TypeLiteral(typeLiteral) =>
    parse__Node__TypeLiteral(~runtime, ~scope, typeLiteral)
  | _ =>
    raise(
      Exceptions.UnexpectedAtThisPoint(
        Printf.sprintf(
          "> Cannot process generic node '%s'",
          node#getKindName(),
        ),
      ),
    )
  };
}
and parse__Node__Generic_assignable =
    (~runtime, ~scope, node: Ts_nodes.Generic.t)
    : (runtime, scope, Node.node(Node.Constraint.assignable)) => {
  let (runtime, scope, node) = parse__Node__Generic(~runtime, ~scope, node);
  (runtime, scope, node |> Node.Escape.toAssignable);
}
and parse__Node__Generic__WrapSubNode =
    (~runtime, ~scope, node: Ts_nodes.Generic.t) => {
  let name = Path.make_sub_type_name(scope.path);
  let type_name = Identifier.TypeName(name);
  let scope = scope |> Scope.add_to_path(type_name);
  let (runtime, scope, wrapped_type) =
    parse__Node__Generic_assignable(~runtime, ~scope, node);
  let wrapped_type_declaration =
    TypeDeclaration({
      path: scope.path,
      extracted_nodes: [||],
      name: type_name,
      annot: wrapped_type,
      params: [||],
    });
  let scope = scope |> Scope.add_root_declaration(wrapped_type_declaration);
  (runtime, scope, Reference({target: [|type_name|], params: [||]}));
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Source File
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__SourceFile =
    (~runtime, ~scope, node: Ts_nodes.SourceFile.t)
    : (runtime, scope, Node.node(Node.Constraint.atLeastModule('poly))) => {
  let source_file_name = node#getBaseNameWithoutExtension();
  let scope = {...scope, source_file: Some(node)};
  let children_to_traverse: array(Ts_nodes.Generic.t) =
    runtime.parse_config.exports_only
      ? node#getExportedDeclarations() : node#getStatements();

  let (runtime: runtime, scope: scope) =
    CCArray.fold_left(
      ((runtime, scope), node) => {
        let scope = scope |> Scope.replace_path(Module(source_file_name));
        let (runtime, scope, res) =
          parse__Node__Declaration(~runtime, ~scope, node);
        let scope = scope |> Scope.add_root_declaration(res);
        (runtime, scope);
      },
      (runtime, scope),
      children_to_traverse,
    );

  let (runtime, scope, prependFixtures) =
    parse__Fixtures__ForSourceFile(~runtime, ~scope);
  (
    runtime,
    scope,
    Module({
      name: source_file_name,
      path: node#getFilePath(),
      types: CCArray.append(prependFixtures, scope.root_declarations),
    }),
  );
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- TypeDeclaration
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__Declaration:
  (~runtime: runtime, ~scope: scope, Ts_morph.Node.t) =>
  (runtime, scope, Node.node(Node.Constraint.moduleLevel)) =
  (~runtime, ~scope, node: Ts_morph.Node.t) => {
    let identified_node = Ts_nodes_util.identifyNode(node);
    switch (identified_node) {
    // | ClassDeclaration
    // | InterfaceDeclaration
    | EnumDeclaration(enum) =>
      parse__Node__EnumDeclaration(~runtime, ~scope, enum)
    | FunctionDeclaration(fn_declaration) =>
      parse__Node__FunctionDeclaration(~runtime, ~scope, fn_declaration)
    // | VariableDeclaration
    | TypeAliasDeclaration(typeAlias) =>
      parse__Node__TypeAliasDeclaration(~runtime, ~scope, typeAlias)
    // | NamespaceDeclaration
    // | Expression
    | _ =>
      Console.error("> " ++ node#getKindName());
      raise(Failure("Only declarations allowed here"));
    };
  }
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- FunctionDeclaration
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__FunctionDeclaration =
    (~runtime, ~scope, node: Ts_nodes.FunctionDeclaration.t) => {
  let name =
    switch (node#getName()) {
    | Some(name) => name
    | None when node#isDefaultExport() => "default"
    | None => raise(Failure("Expected a name for this function"))
    };
  let type_name: Identifier.t(Identifier.Constraint.exactlyTypeName) =
    Identifier.TypeName(name);
  let type_path = CCArray.append(scope.path, [|Identifier.TypeName(name)|]);
  let scope = {...scope, parent: Some(FunctionDeclaration(node))};

  let (runtime, scope, return_type) =
    switch (node#getReturnTypeNode()) {
    | Some(return_node) =>
      let scope = scope |> Scope.add_to_path(Identifier.SubName("return"));
      parse__Node__Generic_assignable(~runtime, ~scope, return_node);
    | None => (runtime, scope, Basic(Any))
    };

  let (runtime, scope, parameters) =
    node#getParameters()
    |> CCArray.foldi(
         ((runtime, scope, params), i, param) => {
           let name = Identifier.PropertyName(param#getName());
           let is_optional = param#isOptional();
           let current_path =
             type_path |> Path.add(Identifier.SubName(Path.unwrap(name)));
           let scope = scope |> Scope.replace_path_arr(current_path);
           // TODO: isRestParameter
           let (runtime, scope, type_) =
             switch (param#getTypeNode()) {
             | None => (runtime, scope, Basic(Any))
             | Some(t) => parse__Node__Generic_assignable(~runtime, ~scope, t)
             };
           (
             runtime,
             scope,
             CCArray.append(
               params,
               [|Parameter({name, is_optional, type_, named: is_optional})|],
             ),
           );
         },
         (runtime, scope, [||]),
       );

  (
    runtime,
    scope,
    TypeDeclaration({
      path: scope.path,
      extracted_nodes: [||],
      name: type_name,
      annot: Function({return_type, parameters}),
      params: [||],
    }),
  );
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Type Literal
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__TypeLiteral =
    (~runtime, ~scope, node: Ts_nodes.TypeLiteral.t) => {
  let nodes_to_parse =
    node#getMembers() |> CCArray.map(Ts_nodes_util.identifyGenericNode);
  let (runtime, signatures) =
    nodes_to_parse
    |> CCArray.fold_left(
         ((runtime, nodes), node) => {
           let (runtime, _, signature) =
             parse__Node__SignatureLike(~runtime, ~scope, node);
           (
             runtime,
             switch (signature) {
             | (None, _, _) =>
               raise(
                 Failure("Type literal property should probably have a name"),
               )
             | (Some(name), is_optional, t) =>
               CCArray.append(
                 nodes,
                 [|
                   Parameter({
                     name: Identifier.PropertyName(name),
                     is_optional,
                     type_: t,
                     named: true,
                   }),
                 |],
               )
             },
           );
         },
         (runtime, [||]),
       );

  (runtime, scope, Record(signatures));
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Signature Hub
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__SignatureLike:
  (~runtime: runtime, ~scope: scope, Ts_nodes.nodeKind) =>
  (
    runtime,
    scope,
    (option(string), bool, Node.node(Node.Constraint.assignable)),
  ) =
  (~runtime, ~scope, node: Ts_nodes.nodeKind) => {
    switch (node) {
    | PropertySignature(node) =>
      let name = Some(node#getName());
      let type_node = node#getTypeNode();
      let (runtime, _, t) =
        switch (type_node) {
        | None => (runtime, scope, Basic(Any))
        | Some(type_node) =>
          parse__Node__Generic_assignable(
            ~runtime,
            ~scope=
              scope |> Scope.add_to_path(Identifier.SubName(node#getName())),
            type_node,
          )
        };
      let is_optional = node#getQuestionTokenNode() |> CCOpt.is_some;
      (runtime, scope, (name, is_optional, t));
    | MethodSignature(_)
    | ConstructSignatureDeclaration(_)
    | CallSignatureDeclaration(_)
    | IndexSignatureDeclaration(_) => raise(Not_found)
    | _ => raise(Failure("Not an signature"))
    };
  }
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- EnumDeclaration
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__EnumDeclaration =
    (~runtime, ~scope, node: Ts_nodes.EnumDeclaration.t) => {
  // TODO: Solving this simply for now, we need to look at the propertyName eventuall though!!
  let members = node#getMembers();
  let type_name: Identifier.t(Identifier.Constraint.exactlyTypeName) =
    Identifier.TypeName(node#getName());
  let scope = {
    ...scope,
    parent: Some(EnumDeclaration(node)),
    path:
      CCArray.append(scope.path, [|Identifier.TypeName(node#getName())|]),
  };
  let variant_constructors =
    members
    |> CCArray.map(member =>
         {
           VariantConstructor.name:
             Identifier.VariantIdentifier(member#getName()),
           arguments: [||],
         }
       );
  let annotation = Variant(variant_constructors);

  (
    runtime,
    scope,
    TypeDeclaration({
      path: scope.path,
      extracted_nodes: [||],
      name: type_name,
      annot: annotation,
      params: [||],
    }),
  );
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- TypeAliasDeclaration
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__TypeAliasDeclaration:
  (~runtime: runtime, ~scope: scope, Ts_nodes.TypeAliasDeclaration.t) =>
  (runtime, scope, Node.node(Node.Constraint.moduleLevel)) =
  (~runtime, ~scope, node: Ts_nodes.TypeAliasDeclaration.t) => {
    let type_name: Identifier.t(Identifier.Constraint.exactlyTypeName) =
      Identifier.TypeName(node#getName());
    let scope = {
      ...scope,
      parent: Some(TypeAliasDeclaration(node)),
      path:
        CCArray.append(scope.path, [|Identifier.TypeName(node#getName())|]),
    };

    let (runtime, scope, annotation) =
      parse__Node__Generic_assignable(~runtime, ~scope, node#getTypeNode());

    (
      runtime,
      scope,
      TypeDeclaration({
        path: scope.path,
        extracted_nodes: [||],
        name: type_name,
        annot: annotation,
        params: [||],
      }),
    );
  }
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Basic Nodes
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__Basic = (~runtime, ~scope, node: Ts_nodes.nodeKind) => {
  switch (node) {
  | StringKeyword(_) => (runtime, scope, Basic(String))
  | NumberKeyword(_) => (runtime, scope, Basic(Number))
  | NeverKeyword(_) => (runtime, scope, Basic(Never))
  | ObjectKeyword(_) => (runtime, scope, Basic(RelevantKeyword("Object")))
  | UndefinedKeyword(_) => (runtime, scope, Basic(Undefined))
  | VoidKeyword(_) => (runtime, scope, Basic(Void))
  | NullKeyword(_) => (runtime, scope, Basic(Null))
  | BooleanKeyword(_) => (runtime, scope, Basic(Boolean))
  | AnyKeyword(_) => (runtime, {...scope, has_any: true}, Basic(Any))
  | _ => raise(Exceptions.UnexpectedAtThisPoint("Not a basic type"))
  };
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Tuples
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__Tuple = (~runtime, ~scope, node: Ts_nodes.nodeKind) => {
  switch (node) {
  | TupleType(node) =>
    // TODO: Handle potentially extracted type here
    // Whenever "inner" types are parsed, we should use a separate "parse__Node" function
    // Maybe

    let children_to_traverse = node#getElementTypeNodes();
    let (
      runtime: runtime,
      scope: scope,
      inner: array(Node.node(Node.Constraint.assignable)),
    ) =
      CCArray.fold_left(
        ((runtime, scope, nodes), node) => {
          let (runtime, scope, res) =
            parse__Node__Generic_assignable(~runtime, ~scope, node);
          (runtime, scope, CCArray.append(nodes, [|res|]));
        },
        (runtime, scope, [||]),
        children_to_traverse,
      );

    (runtime, scope, Tuple(inner));
  | _ => raise(Exceptions.UnexpectedAtThisPoint("Not a tuple"))
  };
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Arrays
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Node__Array = (~runtime, ~scope, node: Ts_nodes.nodeKind) => {
  switch (node) {
  | ArrayType(node) =>
    let (runtime, scope, inner) =
      // TODO: Handle potentially extracted type here
      // Whenever "inner" types are parsed, we should use a separate "parse__Node" function
      // Maybe
      parse__Node__Generic_assignable(
        ~runtime,
        ~scope,
        node#getElementTypeNode(),
      );
    (runtime, scope, Array(inner));

  | _ => raise(Exceptions.UnexpectedAtThisPoint("Not an array"))
  };
}
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// --- Adding Fixtures
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
and parse__Fixtures__ForSourceFile = (~runtime, ~scope) => {
  (
    runtime,
    scope,
    if (scope.has_any) {
      [|Fixture(AnyUnboxed)|];
    } else {
      [||];
    },
  );
};

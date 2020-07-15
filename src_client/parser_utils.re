open Ast;

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
      | TypeParameter(str)
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
          | TypeParameter(str)
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

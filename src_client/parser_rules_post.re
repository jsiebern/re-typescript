open Ast;
open Parser_utils;

let rule_keep_referenced_empty_records_as_any =
    (~runtime, ~scope, root_module: Node.node(Node.Constraint.exactlyModule)) => {
  let scope = ref(scope);
  let had_any = scope^.has_any;

  let rec walk: type t. Node.node(t) => option(Node.node(t)) =
    node =>
      switch (node) {
      | Node.Module({types, _} as m) =>
        let types = types |> CCArray.filter_map(t => walk(t));
        Some(Node.Module({...m, types}));
      | TypeDeclaration({annot: Record([||]), path, _} as td) =>
        if (CCHashtbl.get(scope^.refs, path)
            |> CCOpt.map_or(~default=0, CCArray.length) > 0) {
          scope := {...scope^, has_any: true};
          Some(TypeDeclaration({...td, annot: Basic(Any)}));
        } else {
          None;
        }
      | other => Some(other)
      };
  let root_module = walk(root_module) |> CCOpt.get_exn;
  let root_module =
    switch (scope^.has_any, had_any) {
    | (true, false) =>
      switch (root_module) {
      | Module({types, _} as m) =>
        Node.Module({
          ...m,
          types: CCArray.append([|Node.Fixture(AnyUnboxed)|], types),
        })
      }
    | _ => root_module
    };
  (runtime, scope, root_module);
};

let run = (~runtime, ~scope, node: Node.node(Node.Constraint.exactlyModule)) => {
  rule_keep_referenced_empty_records_as_any(~runtime, ~scope, node);
};

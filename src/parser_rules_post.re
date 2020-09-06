open Ast;
open Parser_utils;

module Helpers = {
  let find_td = (path, current_type_list) =>
    CCArray.find_idx(
      node =>
        switch (node) {
        | Node.TypeDeclaration({path: p, _}) when Path.eq(p, path) => true
        | _ => false
        },
      current_type_list,
    )
    |> CCOpt.flat_map(((i, v)) =>
         switch (v) {
         | Node.TypeDeclaration(inner) => Some((i, inner))
         | _ => None
         }
       );
  let find_td_unsafe = (path, current_type_list) =>
    find_td(path, current_type_list) |> CCOpt.get_exn;

  let get_replaceable_ref_target =
      (~scope, ~records=false, ~current_type_list, ~source_path, target_path) => {
    let scoped_path = Path.make_current_scope(source_path);
    let qualified_target = CCArray.append(scoped_path, target_path);
    let ref_num =
      CCHashtbl.get(scope.refs, qualified_target)
      |> CCOpt.map_or(~default=0, CCArray.length);
    if (ref_num <= 1) {
      let target_idx = current_type_list |> find_td(qualified_target);
      switch (target_idx) {
      | Some((ti, {annot: Record(_), _})) when !records => None
      | Some((ti, t_inner)) =>
        // Set this to "never" so it does not get printed.
        // Still keeping the name around could be useful later though
        CCArray.set(
          current_type_list,
          ti,
          TypeDeclaration({...t_inner, annot: Basic(Never), params: [||]}),
        );
        Hashtbl.remove(scope.refs, qualified_target);
        Some(t_inner);
      | _ => None
      };
    } else {
      None;
    };
  };

  let maybe_replace_type_declaration =
      (
        ~mapper=?,
        ~records=false,
        ~scope,
        ~current_type_list,
        ~source_path,
        target_path,
      ) => {
    switch (current_type_list |> find_td(source_path)) {
    | None => ()
    | Some((source_id, source)) =>
      switch (
        get_replaceable_ref_target(
          ~records,
          ~scope,
          ~current_type_list,
          ~source_path,
          target_path,
        )
      ) {
      | None => ()
      | Some(target) =>
        CCArray.set(
          current_type_list,
          source_id,
          TypeDeclaration({
            ...target,
            name: source.name,
            annot:
              mapper
              |> CCOpt.map_or(~default=target.annot, mapper =>
                   mapper(target.annot)
                 ),
          }),
        )
      }
    };
  };
};

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
  (runtime, scope^, root_module);
};

let rule_move_only_once_referenced_types_back_into_record_types_and_functions =
    (
      ~runtime,
      ~scope: scope,
      root_module: Node.node(Node.Constraint.exactlyModule),
    ) => {
  let rec walk:
    type t.
      (~current_type_list: array(Node.node('a))=?, Node.node(t)) => unit =
    (~current_type_list=[||], node) =>
      switch (node) {
      | Node.Module({types, _}) =>
        types |> CCArray.iter(t => walk(~current_type_list=types, t))
      | TypeDeclaration({
          annot: Function({return_type: Reference({target, _}), _}),
          path,
          _,
        }) =>
        switch (
          Helpers.get_replaceable_ref_target(
            ~scope,
            ~current_type_list,
            ~source_path=path,
            target,
          )
        ) {
        | None => ()
        | Some(target) =>
          let (source_id, source) =
            current_type_list |> Helpers.find_td_unsafe(path);
          CCArray.set(
            current_type_list,
            source_id,
            TypeDeclaration({...target, name: source.name}),
          );
        }

      | TypeDeclaration({annot: Record(fields), path, _}) =>
        fields
        |> CCArray.iteri((i, field) =>
             switch (field) {
             | Node.Parameter(
                 {type_: Reference({target, _}), _} as field_inner,
               ) =>
               switch (
                 Helpers.get_replaceable_ref_target(
                   ~scope,
                   ~current_type_list,
                   ~source_path=path,
                   target,
                 )
               ) {
               | None => ()
               | Some({annot, params, _}) =>
                 CCArray.set(
                   fields,
                   i,
                   Parameter({...field_inner, type_: annot}),
                 )
               }
             | _ => ()
             }
           )
      | _ => ()
      };

  walk(root_module);

  (runtime, scope, root_module);
};

let rule_move_only_once_referenced_types_into_their_respective_type_declaration =
    (
      ~runtime,
      ~scope: scope,
      root_module: Node.node(Node.Constraint.exactlyModule),
    ) => {
  let rec walk:
    type t.
      (~current_type_list: array(Node.node('a))=?, Node.node(t)) => unit =
    (~current_type_list=[||], node) =>
      switch (node) {
      | Node.Module({types, _}) =>
        types |> CCArray.iter(t => walk(~current_type_list=types, t))
      | TypeDeclaration({annot: Reference({target, _}), path, _}) =>
        Helpers.maybe_replace_type_declaration(
          ~records=true,
          ~scope,
          ~current_type_list,
          ~source_path=path,
          target,
        )
      | _ => ()
      };

  walk(root_module);

  (runtime, scope, root_module);
};

let rule_move_only_once_referenced_types_into_arrays_and_optionals =
    (
      ~runtime,
      ~scope: scope,
      root_module: Node.node(Node.Constraint.exactlyModule),
    ) => {
  let rec walk:
    type t.
      (~current_type_list: array(Node.node('a))=?, Node.node(t)) => unit =
    (~current_type_list=[||], node) => {
      switch (node) {
      | Node.Module({types, _}) =>
        types |> CCArray.iter(t => walk(~current_type_list=types, t))
      | TypeDeclaration({
          annot: Array(Reference({target, _})) as annot,
          path,
          _,
        })
      | TypeDeclaration({
          annot:
            Reference({
              target: [|TypeName("Array")|],
              params: [(_, Node.Reference({target, _}))],
            }) as annot,
          path,
          _,
        })
      | TypeDeclaration({
          annot: Optional(Reference({target, _})) as annot,
          path,
          _,
        })
      | TypeDeclaration({
          annot: Nullable(Reference({target, _})) as annot,
          path,
          _,
        }) =>
        let mapper =
          switch (annot) {
          | Reference({target: [|TypeName("Array")|], _})
          | Array(_) => (a => Node.Array(a))
          | Optional(_) => (a => Node.Optional(a))
          | Nullable(_) => (a => Node.Nullable(a))
          | _ => raise(Failure("This is actually impossible"))
          };

        Helpers.maybe_replace_type_declaration(
          ~scope,
          ~current_type_list,
          ~source_path=path,
          ~mapper,
          target,
        );
      | _ => ()
      };
    };

  walk(root_module);

  (runtime, scope, root_module);
};

let rule_transform_single_literals_into_union_types =
    (
      ~runtime,
      ~scope: scope,
      root_module: Node.node(Node.Constraint.exactlyModule),
    ) => {
  let rec walk:
    type t.
      (~current_type_list: array(Node.node('a))=?, Node.node(t)) => unit =
    (~current_type_list=[||], node) =>
      switch (node) {
      | Node.Module({types, _}) =>
        types |> CCArray.iter(t => walk(~current_type_list=types, t))
      | TypeDeclaration({annot: Literal(_) as l, path, _} as td) =>
        let union_type = Parser_union.determine_union_type([|l|]);
        let replace_annot =
          switch (union_type) {
          | Some(StringLiteral(literals)) =>
            Parser_generators.generate_string_literal_list(literals)
          | Some(NumericLiteral(literals)) =>
            Parser_generators.generate_number_literal_list(literals)
          | Some(_)
          | None => l
          };
        switch (current_type_list |> Helpers.find_td(path)) {
        | None => ()
        | Some((i, _)) =>
          CCArray.set(
            current_type_list,
            i,
            TypeDeclaration({...td, annot: replace_annot}),
          )
        };
      | _ => ()
      };

  walk(root_module);

  (runtime, scope, root_module);
};

let run = (~runtime, ~scope, node: Node.node(Node.Constraint.exactlyModule)) => {
  let (runtime, scope, node) =
    rule_keep_referenced_empty_records_as_any(~runtime, ~scope, node);
  let (runtime, scope, node) =
    rule_move_only_once_referenced_types_into_their_respective_type_declaration(
      ~runtime,
      ~scope,
      node,
    );
  let (runtime, scope, node) =
    rule_move_only_once_referenced_types_into_arrays_and_optionals(
      ~runtime,
      ~scope,
      node,
    );
  let (runtime, scope, node) =
    rule_move_only_once_referenced_types_back_into_record_types_and_functions(
      ~runtime,
      ~scope,
      node,
    );
  let (runtime, scope, node) =
    rule_transform_single_literals_into_union_types(~runtime, ~scope, node);
  (runtime, scope, node);
};

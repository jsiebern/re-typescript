open Ast;
open Parser_utils;

module Helpers = {
  let find_td_unsafe = (path, current_type_list) =>
    find_td(path, current_type_list) |> CCOpt.get_exn;

  let get_replaceable_ref_target =
      (~scope, ~records=false, ~current_type_list, ~source_path, target_path) => {
    // TODO: Needs refactoring, it's a very intedependant solution
    // scoped_path can lose one more module if the last sub part is a "_"
    // Line ~287 needs refactoring alongside this BADLY
    let scoped_path =
      source_path
      |> Path.make_sub_type_name
      |> CCString.rev
      |> CCString.take(1) == "_"
        ? Path.make_current_scope(source_path)
          |> CCArray.rev
          |> Path.strip_root_module
          |> CCArray.rev
        : Path.make_current_scope(source_path);

    let qualified_target = CCArray.append(scoped_path, target_path);
    let ref_num =
      CCHashtbl.get(scope.refs, qualified_target)
      |> CCOpt.map_or(~default=0, CCArray.length);

    let target_idx = current_type_list |> find_td(qualified_target);
    switch (target_idx) {
    | Some((ti, {annot: Record(_), _})) when !records => None
    | Some((_, {annot: Basic(_), _} as t_inner)) when ref_num > 1 =>
      // Basic types can be "desolved" over time as it doesn't make sense
      // to keep a base type alias around
      scope |> Scope.decrease_ref(qualified_target);
      Some(t_inner);
    | Some((_, {params: [||], annot: Reference({target, _}), _}))
        when
          CCArray.get_safe(target, 0)
          |> CCOpt.map_or(
               ~default=false,
               fun
               | Identifier.Module(_) => true
               | _ => false,
             ) =>
      // Do not unset module refs (usually artificial ones)
      // TODO: Might be an obsolete logic if we resolve recursive references before this point
      None
    | Some((ti, {params: [||], _} as t_inner)) when ref_num <= 1 =>
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
          types: CCArray.append([|Node.Fixture(AnyUnboxed, [||])|], types),
        })
      }
    | _ => root_module
    };
  (runtime, scope^, root_module);
};

let rule_move_only_once_referenced_types_into_their_respective_type_declaration =
    (
      ~runtime,
      ~scope: scope,
      root_module: Node.node(Node.Constraint.exactlyModule),
    ) => {
  // TODO: This whole function is extremely shady and frankly stupid, this needs some serious overhaul in the future
  let rec replace =
          (~scope, ~current_type_list, ~path, node)
          : (
              Node.node(Node.Constraint.global),
              array(Node.node(Node.Constraint.global)),
            ) => {
    let node = Node.Escape.toGlobal(node);
    switch (node) {
    | Node.Array(inner) =>
      let (node, lst) = replace(~scope, ~current_type_list, ~path, inner);
      (Node.Array(node |> Node.Escape.toAssignable), lst);
    | Optional(inner) =>
      let (node, lst) = replace(~scope, ~current_type_list, ~path, inner);
      (Optional(node |> Node.Escape.toAssignable), lst);
    | Nullable(inner) =>
      let (node, lst) = replace(~scope, ~current_type_list, ~path, inner);
      (Nullable(node |> Node.Escape.toAssignable), lst);
    | Parameter({type_, _} as inner) =>
      let (node, lst) = replace(~scope, ~current_type_list, ~path, type_);
      (Parameter({...inner, type_: node |> Node.Escape.toAssignable}), lst);
    | Tuple(members) =>
      let (nodes, lst) =
        members
        |> CCArray.fold_left(
             ((members, lst), member) => {
               let (node, lst) =
                 replace(~scope, ~current_type_list=lst, ~path, member);
               (
                 CCArray.append(
                   members,
                   [|node |> Node.Escape.toAssignable|],
                 ),
                 lst,
               );
             },
             ([||], current_type_list),
           );
      (Tuple(nodes), lst);
    | Reference({target, _}) as r
        when
          CCArray.get_safe(target, 0)
          |> CCOpt.map_or(
               ~default=false,
               fun
               | Identifier.Module(_) => true
               | _ => false,
             ) => (
        r,
        current_type_list,
      )
    | Reference({target, _}) as r =>
      let lst = ref(current_type_list);
      switch (
        Helpers.get_replaceable_ref_target(
          ~scope,
          ~current_type_list=lst^,
          ~source_path=path,
          target,
        )
      ) {
      | None => (r, lst^)
      | Some({annot, _}) => (annot |> Node.Escape.toGlobal, lst^)
      };

    | Record(fields) =>
      let (nodes, lst) =
        fields
        |> CCArray.fold_left(
             ((members, lst), member) => {
               let (node, lst) =
                 replace(
                   ~scope,
                   ~current_type_list=lst,
                   ~path,
                   member |> Node.Escape.toAssignable,
                 );
               (
                 CCArray.append(
                   members,
                   [|node |> Node.Escape.toParameterOnly|],
                 ),
                 lst,
               );
             },
             ([||], current_type_list),
           );

      (Record(nodes), lst);
    | TypeDeclaration({annot: Reference({target, _}), _} as inner) =>
      switch (
        Helpers.get_replaceable_ref_target(
          ~records=true,
          ~scope,
          ~current_type_list,
          ~source_path=
            path
            |> Path.make_sub_type_name
            |> CCString.rev
            |> CCString.take(1) == "_"
              ? inner.path |> Path.add(Identifier.SubName("_")) : inner.path,
          target,
        )
      ) {
      | None => (TypeDeclaration(inner), current_type_list)
      | Some({annot, params, _}) => (
          TypeDeclaration({...inner, annot, params}),
          current_type_list,
        )
      }
    | TypeDeclaration({annot, path, _} as inner) =>
      let (node, lst) = replace(~scope, ~current_type_list, ~path, annot);
      (
        TypeDeclaration({...inner, annot: node |> Node.Escape.toAssignable}),
        lst,
      );
    | Function({parameters, return_type}) =>
      let (nodes, lst) =
        parameters
        |> CCArray.fold_left(
             ((members, lst), member) => {
               let (node, lst) =
                 replace(
                   ~scope,
                   ~current_type_list=lst,
                   ~path,
                   member |> Node.Escape.toAssignable,
                 );
               (
                 CCArray.append(
                   members,
                   [|node |> Node.Escape.toAssignable|],
                 ),
                 lst,
               );
             },
             ([||], current_type_list),
           );
      let (return_type, lst) =
        replace(
          ~scope,
          ~current_type_list=lst,
          ~path,
          return_type |> Node.Escape.toAssignable,
        );
      (
        Function({
          parameters: nodes |> CCArray.map(Node.Escape.toParameterOnly),
          return_type: return_type |> Node.Escape.toAssignable,
        }),
        lst,
      );
    | Module({types, _} as m)
        when
          CCArray.get_safe(types, 0)
          |> CCOpt.map_or(~default=false, t =>
               switch (t) {
               | Node.Fixture(TUnboxed(_), _) => true
               | _ => false
               }
             ) =>
      let (nodes, lst) =
        types
        |> CCArray.fold_left(
             ((members, lst), member) => {
               let (node, lst) =
                 replace(
                   ~scope,
                   ~current_type_list=lst,
                   ~path=path |> Path.add(Identifier.SubName("_")),
                   member |> Node.Escape.toAssignable,
                 );
               (
                 CCArray.append(
                   members,
                   [|node |> Node.Escape.toAssignable|],
                 ),
                 lst,
               );
             },
             ([||], current_type_list),
           );
      (
        Module({
          ...m,
          types: nodes |> CCArray.map(Node.Escape.toModuleLevel),
        }),
        lst,
      );
    | Module({types, _} as m) =>
      let t = ref(types |> CCArray.map(Node.Escape.toGlobal));
      let (nodes, lst) =
        t^
        |> CCArray.fold_left(
             ((nodes, lst), typ) => {
               let (node, lst) =
                 replace(
                   ~scope,
                   ~current_type_list=lst,
                   ~path,
                   typ |> Node.Escape.toAssignable,
                 );
               (
                 CCArray.append(nodes, [|node |> Node.Escape.toModuleLevel|]),
                 lst,
               );
             },
             ([||], types |> CCArray.map(Node.Escape.toGlobal)),
           );
      lst
      |> CCArray.iteri((i, t) =>
           switch (t) {
           | Node.TypeDeclaration({annot: Basic(Never), _}) =>
             CCArray.set(nodes, i, t |> Node.Escape.toModuleLevel)
           | _ => ()
           }
         );
      (Module({...m, types: nodes}), [||]);
    | v => (v |> Node.Escape.toGlobal, current_type_list)
    };
  };
  let (root_module, _) =
    replace(
      ~scope,
      ~current_type_list=[||],
      ~path=[||],
      root_module |> Node.Escape.toAssignable,
    );
  (runtime, scope, root_module |> Node.Escape.toModuleOnly);
};

let rule_transform_single_literals_into_union_types =
    (
      ~runtime,
      ~scope: scope,
      root_module: Node.node(Node.Constraint.exactlyModule),
    ) => {
  let config = Re_typescript_config.getConfig();
  // TODO: Sub modules should be iterated on in walk_list as well
  let walk_list:
    type t. (Node.node(t), array(Node.node('a))) => array(Node.node('a)) =
    (node, current_type_list) => {
      switch (node) {
      | TypeDeclaration({annot: Literal(_) as l, path, _} as td) =>
        let union_type = Parser_union.determine_union_type([|l|]);
        let replace_annot =
          switch (union_type) {
          | Some(StringLiteral(literals)) =>
            let (_, scope, t) =
              Parser_generators.generate_string_literal_list(
                ~runtime,
                ~scope,
                literals,
              );
            (
              t,
              switch (config.unions.string_literal) {
              | Unboxed =>
                Some(
                  CCArray.get(
                    scope.root_declarations,
                    CCArray.length(scope.root_declarations) - 1,
                  ),
                )
              | Variant
              | PolymorphicVariant => None
              },
            );
          | Some(NumericLiteral(literals)) =>
            let (_, scope, t) =
              Parser_generators.generate_number_literal_list(
                ~runtime,
                ~scope,
                literals,
              );
            (
              t,
              switch (config.unions.number_literal) {
              | Unboxed =>
                Some(
                  CCArray.get(
                    scope.root_declarations,
                    CCArray.length(scope.root_declarations) - 1,
                  ),
                )
              | Variant
              | PolymorphicVariant => None
              },
            );
          | Some(_)
          | None => (l, None)
          };
        switch (current_type_list |> find_td(path)) {
        | None => current_type_list
        | Some((i, _)) =>
          let (replace_annot, before) = replace_annot;
          CCArray.set(
            current_type_list,
            i,
            TypeDeclaration({...td, annot: replace_annot}),
          );
          switch (before) {
          | None => current_type_list
          | Some(before) when i == 0 =>
            CCArray.append([|before|], current_type_list)
          | Some(before) =>
            CCArray.concat([
              CCArray.sub(current_type_list, 0, i - 1),
              [|before|],
              CCArray.sub(
                current_type_list,
                i,
                CCArray.length(current_type_list) - i - 1,
              ),
            ])
          };
        };
      | _ => current_type_list
      };
    };

  let walk:
    type t.
      (~current_type_list: ref(array(Node.node('a)))=?, Node.node(t)) =>
      Node.node(t) =
    (~current_type_list=ref([||]), node) =>
      switch (node) {
      | Node.Module({types, _})
          // We want to skip the entire module if it is a union type already
          when
            CCArray.get_safe(types, 0)
            |> CCOpt.map_or(~default=false, t =>
                 switch (t) {
                 | Node.Fixture(TUnboxed(_), _) => true
                 | _ => false
                 }
               ) => node
      | Node.Module({types, _} as m) =>
        Node.Module({
          ...m,
          types:
            types
            |> CCArray.fold_left(
                 (types, node) => walk_list(node, types),
                 types,
               ),
        })
      | _ => node
      };

  let root_module = walk(root_module);

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
    rule_transform_single_literals_into_union_types(~runtime, ~scope, node);
  (runtime, scope, node);
};

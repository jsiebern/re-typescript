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
    );
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

let rule_move_only_once_referenced_types_back_into_record_types =
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
      | TypeDeclaration({annot: Record(fields), path, _}) =>
        fields
        |> CCArray.iteri((i, field) =>
             switch (field) {
             | Node.Parameter(
                 {type_: Reference({target, _}), _} as field_inner,
               ) =>
               let scoped_path = Path.make_current_scope(path);
               let qualified_target = CCArray.append(scoped_path, target);
               let ref_num =
                 CCHashtbl.get(scope.refs, qualified_target)
                 |> CCOpt.map_or(~default=0, CCArray.length);
               if (ref_num <= 1) {
                 // TODO: Abstract the resolution of the path as it is used in many functions and will maybe break when using namespaces
                 let target_idx =
                   CCArray.find_idx(
                     node =>
                       switch (node) {
                       | Node.TypeDeclaration({annot: Record(_), _}) => false
                       | Node.TypeDeclaration({path: p, _})
                           when Path.eq(p, qualified_target) =>
                         true
                       | _ => false
                       },
                     current_type_list,
                   );
                 switch (target_idx) {
                 | Some((_, TypeDeclaration({annot: Record(_), _}))) => ()
                 | Some((ti, TypeDeclaration({annot, _} as t_inner))) =>
                   CCArray.set(
                     fields,
                     i,
                     Parameter({...field_inner, type_: annot}),
                   );
                   CCArray.set(
                     current_type_list,
                     ti,
                     TypeDeclaration({...t_inner, annot: Basic(Never)}),
                   );
                   Hashtbl.remove(scope.refs, qualified_target);
                 | Some(_)
                 | None => ()
                 };
               };
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
        // TODO: This will maybe break between different modules
        // Experiment with import statements and see how that affects scoping
        let scoped_path = Path.make_current_scope(path);
        let qualified_target = CCArray.append(scoped_path, target);
        let ref_num =
          CCHashtbl.get(scope.refs, qualified_target)
          |> CCOpt.map_or(~default=0, CCArray.length);
        if (ref_num <= 1) {
          // TODO: Abstract the resolution of the path as it is used in many functions and will maybe break when using namespaces
          let reference_idx =
            CCArray.find_idx(
              node =>
                switch (node) {
                | Node.TypeDeclaration({path: p, _}) when Path.eq(p, path) =>
                  true
                | _ => false
                },
              current_type_list,
            );
          let target_idx =
            CCArray.find_idx(
              node =>
                switch (node) {
                | Node.TypeDeclaration({path: p, _})
                    when Path.eq(p, qualified_target) =>
                  true
                | _ => false
                },
              current_type_list,
            );
          switch (reference_idx, target_idx) {
          | (
              Some((ri, TypeDeclaration({name, _}))),
              Some((ti, TypeDeclaration(t_inner))),
            ) =>
            CCArray.set(
              current_type_list,
              ri,
              TypeDeclaration({...t_inner, name}),
            );
            CCArray.set(
              current_type_list,
              ti,
              TypeDeclaration({
                ...t_inner,
                annot: Basic(Never),
                params: [||],
              }),
            );
            Hashtbl.remove(scope.refs, qualified_target);
          | _ => ()
          };
        };
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

        // TODO: This will maybe break between different modules
        // Experiment with import statements and see how that affects scoping
        let scoped_path = Path.make_current_scope(path);
        let qualified_target = CCArray.append(scoped_path, target);
        let ref_num =
          CCHashtbl.get(scope.refs, qualified_target)
          |> CCOpt.map_or(~default=0, CCArray.length);
        if (ref_num <= 1) {
          // TODO: Abstract the resolution of the path as it is used in many functions and will maybe break when using namespaces
          let reference_idx =
            CCArray.find_idx(
              node =>
                switch (node) {
                | Node.TypeDeclaration({path: p, _}) when Path.eq(p, path) =>
                  true
                | _ => false
                },
              current_type_list,
            );
          let target_idx =
            CCArray.find_idx(
              node =>
                switch (node) {
                | Node.TypeDeclaration({path: p, _})
                    when Path.eq(p, qualified_target) =>
                  true
                | _ => false
                },
              current_type_list,
            );
          switch (reference_idx, target_idx) {
          | (_, Some((_, TypeDeclaration({annot: Node.Record(_), _})))) =>
            ()
          | (
              Some((ri, TypeDeclaration({name, _}))),
              Some((ti, TypeDeclaration(t_inner))),
            ) =>
            CCArray.set(
              current_type_list,
              ri,
              TypeDeclaration({
                ...t_inner,
                name,
                annot: mapper(t_inner.annot),
              }),
            );
            CCArray.set(
              current_type_list,
              ti,
              TypeDeclaration({
                ...t_inner,
                annot: Basic(Never),
                params: [||],
              }),
            );
            Hashtbl.remove(scope.refs, qualified_target);
          | _ => ()
          };
        };
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
    rule_move_only_once_referenced_types_back_into_record_types(
      ~runtime,
      ~scope,
      node,
    );
  let (runtime, scope, node) =
    rule_transform_single_literals_into_union_types(~runtime, ~scope, node);
  (runtime, scope, node);
};

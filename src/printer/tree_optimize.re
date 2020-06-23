open Re_typescript_base;
open Tree_types;
open Tree_utils;
open Tree_data;

let rec optimize = (~ctx: Re_typescript_config.Config.config) => {
  optimize__remove_param_dependant_types();
  optimize__literal_unions();
  optimize__empty_obj_references();
  optimize__single_ref_inline_types();
  if (ctx.omit_extended_unreferenced_records) {
    optimize__omit_extended_unreferenced_records();
  };
}
and optimize__remove_param_dependant_types = () => {
  Type.order^
  |> CCList.iter(path => {
       switch (Type.get(~path)) {
       | Some(TypeDeclaration({td_type: ResolveWithParams(_), _})) =>
         // Remove the extra type def from order
         Type.order := Type.order^ |> CCList.remove_one(~eq=Path.eq, path);
         // And replace the reference inside of the field
         Type.remove(~path);
       | _ => ()
       }
     });
}
and optimize__omit_extended_unreferenced_records = () => {
  Type.order^
  |> CCList.iter(path => {
       switch (Type.get(~path)) {
       | Some(TypeDeclaration({td_type: Interface(_, true), _}))
           when {
             let refs = Ref.get_all(path);
             refs
             |> CCList.length <= 1
             || refs
             |> CCList.for_all(r_path =>
                  switch (Type.get(~path=r_path)) {
                  | Some(TypeDeclaration({td_type: Interface(_), _})) => true
                  | None => true
                  | Some(_) => false
                  }
                );
           } =>
         // Remove the extra type def from order
         Type.order := Type.order^ |> CCList.remove_one(~eq=Path.eq, path);
         // And replace the reference inside of the field
         Type.remove(~path);
       | _ => ()
       }
     });
}
and optimize__helper__resolve_reference = (tr: ts_type_reference) => {
  switch (tr) {
  | {tr_path_resolved: Some(resolved_path), _} =>
    Type.get(
      ~path=
        Ref.resolve_ref(
          ~recursive=true,
          ~remember=false,
          ~from=([], []),
          resolved_path,
        )
        |> CCOpt.value(~default=resolved_path),
    )
  | _ => None
  };
}
and optimize__literal_unions = () => {
  // If type unions are clean except for a reference to another union of the same type, they can be merged
  // The original type should be kept around though, in case that restricted value is ever needed somewhere
  // TODO: If mode is set to PolyVariants, this can be changed into an extended poly variant
  let optimize = (~td, ~map, ~path, ~members) => {
    let (non_literal_types, literal_types) =
      members
      |> CCList.fold_left(
           ((nl, l), member) => {
             switch (member.um_type) {
             | StringLiteral(_) as t
             | NumericLiteral(_) as t
             | MixedLiteral(_) as t => (nl, l @ [t])
             | Base(Boolean) as t => (nl, l @ [t])
             | other => (nl @ [other], l)
             }
           },
           ([], []),
         );
    let resolved_literals =
      non_literal_types
      |> CCList.filter_map(member =>
           switch (member) {
           | Reference(r) =>
             switch (r |> optimize__helper__resolve_reference) {
             | Some(TypeDeclaration({td_type: StringLiteral(_) as t, _}))
             | Some(TypeDeclaration({td_type: NumericLiteral(_) as t, _}))
             | Some(TypeDeclaration({td_type: MixedLiteral(_) as t, _})) =>
               Some(t)
             | Some(_)
             | None => None
             }
           | _ => None
           }
         );
    if (CCEqual.int(
          resolved_literals |> CCList.length,
          non_literal_types |> CCList.length,
        )) {
      let new_type =
        switch (
          literal_types
          @ resolved_literals
          |> CCList.fold_left(
               (p, t) =>
                 switch (t) {
                 | Base(Boolean) => {...p, bools: p.bools @ [true, false]}
                 | StringLiteral(l) => {...p, strings: p.strings @ l}
                 | NumericLiteral(l) => {...p, numbers: p.numbers @ l}
                 | MixedLiteral({strings, numbers, bools}) => {
                     strings: p.strings @ strings,
                     numbers: p.numbers @ numbers,
                     bools: p.bools @ bools,
                   }
                 | t =>
                   raise(
                     Exceptions.Optimizer_error(
                       Printf.sprintf(
                         "This case should not have happened! (%s)",
                         ts_to_string(t),
                       ),
                     ),
                   )
                 },
               {strings: [], numbers: [], bools: []},
             )
        ) {
        | {strings: l, numbers: [], bools: []} => StringLiteral(l)
        | {strings: [], numbers: l, bools: []} => NumericLiteral(l)
        | mixed => MixedLiteral(mixed)
        };
      Type.replace(~path, TypeDeclaration({...td, td_type: map(new_type)}));
    };
  };

  Type.order^
  |> CCList.iter(path => {
       switch (Type.get(~path)) {
       | Some(
           TypeDeclaration({td_type: Optional(Union(members)), _} as td),
         ) =>
         optimize(~td, ~map=v => Optional(v), ~path, ~members)
       | Some(
           TypeDeclaration({td_type: Nullable(Union(members)), _} as td),
         ) =>
         optimize(~td, ~map=v => Nullable(v), ~path, ~members)
       | Some(
           TypeDeclaration(
             {td_type: Optional(Nullable(Union(members))), _} as td,
           ),
         ) =>
         optimize(~td, ~map=v => Optional(Nullable(v)), ~path, ~members)
       | Some(TypeDeclaration({td_type: Array(Union(members)), _} as td)) =>
         optimize(~td, ~map=v => Array(v), ~path, ~members)
       | Some(
           TypeDeclaration(
             {td_type: Array(Optional(Union(members))), _} as td,
           ),
         ) =>
         optimize(~td, ~map=v => Array(Optional(v)), ~path, ~members)
       | Some(
           TypeDeclaration(
             {td_type: Array(Optional(Nullable(Union(members)))), _} as td,
           ),
         ) =>
         optimize(
           ~td,
           ~map=v => Array(Optional(Nullable(v))),
           ~path,
           ~members,
         )
       | Some(
           TypeDeclaration(
             {td_type: Array(Nullable(Union(members))), _} as td,
           ),
         ) =>
         optimize(~td, ~map=v => Array(Nullable(v)), ~path, ~members)
       | Some(TypeDeclaration({td_type: Union(members), _} as td)) =>
         optimize(~td, ~map=v => v, ~path, ~members)
       | _ => ()
       }
     });
}
and optimize__single_ref_inline_types = () => {
  // If record fields are the only reference to their respective inline types, the type of that field can be replaced with it
  // The same can be done for arrays
  // The same can be done for tuples
  // The same can be done for function returns
  // The same can be done for function parameters
  // The same can be done for reference parameters
  Type.order^
  |> CCList.iter(path => {
       switch (Type.get(~path)) {
       | Some(
           TypeDeclaration(
             {
               td_type:
                 Array(
                   Reference({tr_path_resolved: Some(resolved_path), _}),
                 ),
               _,
             } as td,
           ),
         )
           when
             snd(resolved_path)
             |> CCList.length > 0
             && Ref.get_all(resolved_path)
             |> CCList.length == 1 =>
         switch (Type.get(~path=resolved_path)) {
         | Some(TypeDeclaration({td_type, _}) as t)
             when !optimize__has_to_stay_inline(t) =>
           // Remove the extra type def from order
           Type.order :=
             Type.order^ |> CCList.remove_one(~eq=Path.eq, resolved_path);
           // An replace the reference inside of the field
           Type.replace(
             ~path,
             TypeDeclaration({...td, td_type: Array(td_type)}),
           );
         | Some(_) => ()
         | None => ()
         }
       | Some(
           TypeDeclaration(
             {
               td_type:
                 Optional(
                   Reference({tr_path_resolved: Some(resolved_path), _}),
                 ),
               _,
             } as td,
           ),
         )
           when
             snd(resolved_path)
             |> CCList.length > 0
             && Ref.get_all(resolved_path)
             |> CCList.length == 1 =>
         switch (Type.get(~path=resolved_path)) {
         | Some(TypeDeclaration({td_type, _}) as t)
             when !optimize__has_to_stay_inline(t) =>
           // Remove the extra type def from order
           Type.order :=
             Type.order^ |> CCList.remove_one(~eq=Path.eq, resolved_path);
           // An replace the reference inside of the field
           Type.replace(
             ~path,
             TypeDeclaration({...td, td_type: Optional(td_type)}),
           );
         | Some(_) => ()
         | None => ()
         }
       | Some(
           TypeDeclaration(
             {
               td_type:
                 Nullable(
                   Reference({tr_path_resolved: Some(resolved_path), _}),
                 ),
               _,
             } as td,
           ),
         )
           when
             snd(resolved_path)
             |> CCList.length > 0
             && Ref.get_all(resolved_path)
             |> CCList.length == 1 =>
         switch (Type.get(~path=resolved_path)) {
         | Some(TypeDeclaration({td_type, _}) as t)
             when !optimize__has_to_stay_inline(t) =>
           // Remove the extra type def from order
           Type.order :=
             Type.order^ |> CCList.remove_one(~eq=Path.eq, resolved_path);
           // And replace the reference inside of the field
           Type.replace(
             ~path,
             TypeDeclaration({...td, td_type: Nullable(td_type)}),
           );
         | Some(_) => ()
         | None => ()
         }
       // ----------------------------------
       // --- Interface Fields
       // ----------------------------------
       | Some(
           TypeDeclaration({td_type: Interface(fields, extended), _} as td),
         ) =>
         let new_fields =
           fields
           |> CCList.map(field => {
                switch (field) {
                | {
                    f_type:
                      Reference({tr_path_resolved: Some(resolved_path), _}),
                    _,
                  }
                    when
                      snd(resolved_path)
                      |> CCList.length > 0
                      && Ref.get_all(resolved_path)
                      |> CCList.length == 1 =>
                  switch (Type.get(~path=resolved_path)) {
                  | Some(TypeDeclaration({td_type, _}) as t)
                      when !optimize__has_to_stay_inline(t) =>
                    // Remove the extra type def from order
                    Type.order :=
                      Type.order^
                      |> CCList.remove_one(~eq=Path.eq, resolved_path);
                    // An replace the reference inside of the field
                    {...field, f_type: td_type};
                  | Some(_) => field
                  | None => field
                  }

                | field => field
                }
              });

         Type.replace(
           ~path,
           TypeDeclaration({
             ...td,
             td_type: Interface(new_fields, extended),
           }),
         );
       // ----------------------------------
       // --- Tuple Members
       // ----------------------------------
       | Some(TypeDeclaration({td_type: Tuple(members), _} as td)) =>
         let new_members =
           members
           |> CCList.map(member => {
                switch (member) {
                | Reference({tr_path_resolved: Some(resolved_path), _})
                    when
                      snd(resolved_path)
                      |> CCList.length > 0
                      && Ref.get_all(resolved_path)
                      |> CCList.length == 1 =>
                  switch (Type.get(~path=resolved_path)) {
                  | Some(TypeDeclaration({td_type, _}) as t)
                      when !optimize__has_to_stay_inline(t) =>
                    // Remove the extra type def from order
                    Type.order :=
                      Type.order^
                      |> CCList.remove_one(~eq=Path.eq, resolved_path);
                    // An replace the reference inside of the field
                    td_type;
                  | Some(_) => member
                  | None => member
                  }
                | member => member
                }
              });

         Type.replace(
           ~path,
           TypeDeclaration({...td, td_type: Tuple(new_members)}),
         );
       // ----------------------------------
       // --- Function Return / Parameters
       // ----------------------------------
       | Some(
           TypeDeclaration(
             {td_type: Function({fu_return, fu_params}), _} as td,
           ),
         ) =>
         let new_return =
           switch (fu_return) {
           | Reference({tr_path_resolved: Some(resolved_path), _})
               when
                 snd(resolved_path)
                 |> CCList.length > 0
                 && Ref.get_all(resolved_path)
                 |> CCList.length == 1 =>
             switch (Type.get(~path=resolved_path)) {
             | Some(TypeDeclaration({td_type, _}) as t)
                 when !optimize__has_to_stay_inline(t) =>
               // Remove the extra type def from order
               Type.order :=
                 Type.order^ |> CCList.remove_one(~eq=Path.eq, resolved_path);
               // An replace the reference inside of the field
               td_type;
             | Some(_) => fu_return
             | None => fu_return
             }
           | _ => fu_return
           };

         let new_params =
           fu_params
           |> CCList.map(param => {
                switch (param) {
                | {
                    fp_type:
                      Reference({tr_path_resolved: Some(resolved_path), _}),
                    _,
                  } as fp
                    when
                      snd(resolved_path)
                      |> CCList.length > 0
                      && Ref.get_all(resolved_path)
                      |> CCList.length == 1 =>
                  switch (Type.get(~path=resolved_path)) {
                  | Some(TypeDeclaration({td_type, _}) as t)
                      when !optimize__has_to_stay_inline(t) =>
                    // Remove the extra type def from order
                    Type.order :=
                      Type.order^
                      |> CCList.remove_one(~eq=Path.eq, resolved_path);
                    // An replace the reference inside of the field
                    {...fp, fp_type: td_type};
                  | Some(_) => param
                  | None => param
                  }
                | param => param
                }
              });

         Type.replace(
           ~path,
           TypeDeclaration({
             ...td,
             td_type:
               Function({fu_return: new_return, fu_params: new_params}),
           }),
         );
       | _ => ()
       }
     });
}
// --- TODO: Revisit this at some later stage and see if it's feasible to make this work (probably not necessary)
// and optimize__single_refs = () => {
//   Type.order^
//   |> CCList.iter(path =>
//        switch (Type.get(~path)) {
//        // ----------------------------------
//        // --- Reference Parameters
//        // ----------------------------------
//        | Some(
//            TypeDeclaration(
//              {td_type: Reference({tr_parameters, _} as re), _} as td,
//            ),
//          ) =>
//          let new_params =
//            tr_parameters
//            |> CCList.map(param => {
//                 switch (param) {
//                 | Reference({tr_path_resolved: Some(resolved_path), _})
//                     when
//                       snd(resolved_path)
//                       |> CCList.length > 0
//                       && Ref.get_all(resolved_path)
//                       |> CCList.length == 1 =>
//                   switch (Type.get(~path=resolved_path)) {
//                   | Some(TypeDeclaration({td_type, _}) as t)
//                       when !optimize__has_to_stay_inline(t) =>
//                     // Remove the extra type def from order
//                     Type.order :=
//                       Type.order^
//                       |> CCList.remove_one(~eq=Path.eq, resolved_path);
//                     // An replace the reference inside of the field
//                     td_type;
//                   | Some(_) => param
//                   | None => param
//                   }
//                 | param => param
//                 }
//               });

//          Type.replace(
//            ~path,
//            TypeDeclaration({
//              ...td,
//              td_type: Reference({...re, tr_parameters: new_params}),
//            }),
//          );
//        | _ => ()
//        }
//      );
// }
// TODO: Potentially react to the configuration here (poly variants don't need to stay inlined for example)
and optimize__has_to_stay_inline =
  fun
  | TypeDeclaration({
      td_type: Interface(_),
      _ // | Union(_) | StringLiteral(_) | NumericLiteral(_),
    }) =>
    true
  | _ => false
and optimize__empty_obj_references = () => {
  // Empty objects should be removed completely as there is no adequate representation in reason for them
  // If they are referenced by another type however, they will be inserted and marked as type "any"
  Type.order^
  |> CCList.iter(path => {
       switch (Type.get(~path)) {
       | Some(TypeDeclaration({td_type: Interface(fields, _), _} as td))
           when fields |> CCList.length == 0 =>
         if (Ref.get_all(path) |> CCList.length == 0) {
           Type.order := Type.order^ |> CCList.remove_one(~eq=Path.eq, path);
         } else {
           Type.replace(
             ~path,
             TypeDeclaration({...td, td_type: Base(Any)}),
           );
         }
       | _ => ()
       }
     });
};

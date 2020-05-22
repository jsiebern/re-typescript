open Re_typescript_base;
open Tree_types;
open Tree_utils;
open Tree_data;

let rec optimize = () => {
  optimize__empty_obj_references();
  optimize__single_ref_inline_types();
}
and optimize__single_ref_inline_types = () => {
  // If record fields are the only reference to their respective inline types, the type of that field can be replaced with it
  // The same can be done for arrays
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
         | Some(TypeDeclaration({td_type: Interface(_), _})) => ()
         | Some(TypeDeclaration({td_type, _})) =>
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
       | Some(TypeDeclaration({td_type: Interface(fields), _} as td)) =>
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
                  | Some(TypeDeclaration({td_type: Interface(_), _})) => field
                  | Some(TypeDeclaration({td_type, _})) =>
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
           TypeDeclaration({...td, td_type: Interface(new_fields)}),
         );
       | _ => ()
       }
     });
}
and optimize__empty_obj_references = () => {
  // Empty objects should be removed completely as there is no adequate representation in reason for them
  // If they are referenced by another type however, they will be inserted and marked as type "any"
  Type.order^
  |> CCList.iter(path => {
       switch (Type.get(~path)) {
       | Some(TypeDeclaration({td_type: Interface(fields), _} as td))
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

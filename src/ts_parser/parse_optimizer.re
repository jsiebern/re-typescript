module Types = Re_typescript_printer.Tree_types;
open Types;
open Parse_utils;

let debug_enabled = ref(false);
let debug = (value: 'a) => debug_enabled^ ? Console.log(value) : ();

type types = Hashtbl.t(identPath, ts_type);
let refCount: Hashtbl.t(identPath, list(identPath)) = Hashtbl.create(10);

let rec canBeInline = (t: ts_type) => {
  debug(Re_typescript_printer.Tree_utils.ts_to_string(t));
  switch (t) {
  | Interface(_) => false
  | Arg(_) => false
  | TypeDeclaration({td_type, _}) => canBeInline(td_type)
  | _ => true
  };
};

let optimizeReferences = (order: list(identPath), types: types) => {
  let rec walk = (t: ts_type) => {
    switch (t) {
    | TypeDeclaration({td_type, td_parameters, _}) =>
      [td_type, ...td_parameters |> CCList.filter_map(tp => tp.tp_default)]
      |> CCList.iter(walk)
    | Function({fu_return, fu_params}) =>
      [fu_return, ...fu_params |> CCList.map(p => p.fp_type)]
      |> CCList.iter(walk)
    | Interface(fields, _) =>
      fields |> CCList.map(f => f.f_type) |> CCList.iter(walk)
    | Union(members) =>
      members |> CCList.map(m => m.um_type) |> CCList.iter(walk)
    | Nullable(member)
    | Optional(member)
    | Set(member)
    | Array(member) => walk(member)
    | Tuple(members) => members |> CCList.iter(walk)
    | Module({m_types, _}) => m_types |> CCList.iter(walk)
    | Reference({tr_path, tr_from, tr_parameters, _}) =>
      tr_parameters |> CCList.iter(walk);
      Hashtbl.replace(
        refCount,
        tr_path,
        (Hashtbl.find_opt(refCount, tr_path) |> CCOpt.value(~default=[]))
        @ [tr_from],
      );
    | _ => ()
    };
  };
  Hashtbl.iter((_, t) => walk(t), types);

  let order = ref(order);

  order^
  |> CCList.iter(path => {
         debug((
           "> ",
           path,
           Hashtbl.find_opt(types, path)
           |> CCOpt.map(Re_typescript_printer.Tree_utils.ts_to_string),
         ));
       switch (Hashtbl.find_opt(types, path)) {
       // ----------------------------------
       // --- Array
       // ----------------------------------
       | Some(
           TypeDeclaration(
             {td_type: Array(Reference({tr_path, _})), _} as td,
           ),
         )
           when
             Hashtbl.find_opt(refCount, tr_path)
             |> CCOpt.map(CCList.length)
             |> CCOpt.value(~default=0) == 1 =>
         switch (Hashtbl.find_opt(types, tr_path)) {
         | Some(TypeDeclaration({td_type, _}) as t) when canBeInline(t) =>
           // Remove the extra type def from order
           order :=
             order^
             |> CCList.remove_one(~eq=CCList.equal(CCString.equal), tr_path);
           // An replace the reference inside of the field
           Hashtbl.replace(
             types,
             path,
             TypeDeclaration({...td, td_type: Array(td_type)}),
           );
         | Some(_) => ()
         | None => ()
         }

       // ----------------------------------
       // --- Optional
       // ----------------------------------
       | Some(
           TypeDeclaration(
             {td_type: Optional(Reference({tr_path, _})), _} as td,
           ),
         )
           when
             Hashtbl.find_opt(refCount, tr_path)
             |> CCOpt.map(CCList.length)
             |> CCOpt.value(~default=0) == 1 =>
         switch (Hashtbl.find_opt(types, tr_path)) {
         | Some(TypeDeclaration({td_type, _}) as t) when canBeInline(t) =>
           // Remove the extra type def from order
           order :=
             order^
             |> CCList.remove_one(~eq=CCList.equal(CCString.equal), tr_path);
           // An replace the reference inside of the field
           Hashtbl.replace(
             types,
             path,
             TypeDeclaration({...td, td_type: Optional(td_type)}),
           );
         | Some(_) => ()
         | None => ()
         }

       // ----------------------------------
       // --- Nullable
       // ----------------------------------
       | Some(
           TypeDeclaration(
             {td_type: Nullable(Reference({tr_path, _})), _} as td,
           ),
         )
           when
             Hashtbl.find_opt(refCount, tr_path)
             |> CCOpt.map(CCList.length)
             |> CCOpt.value(~default=0) == 1 =>
         switch (Hashtbl.find_opt(types, tr_path)) {
         | Some(TypeDeclaration({td_type, _}) as t) when canBeInline(t) =>
           // Remove the extra type def from order
           order :=
             order^
             |> CCList.remove_one(~eq=CCList.equal(CCString.equal), tr_path);
           // And replace the reference inside of the field
           Hashtbl.replace(
             types,
             path,
             TypeDeclaration({...td, td_type: Nullable(td_type)}),
           );
         | Some(_) => ()
         | None => ()
         }
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
           | Reference({tr_path, _})
               when
                 Hashtbl.find_opt(refCount, tr_path)
                 |> CCOpt.map(CCList.length)
                 |> CCOpt.value(~default=0) == 1 =>
             switch (Hashtbl.find_opt(types, tr_path)) {
             | Some(TypeDeclaration({td_type, _})) when canBeInline(td_type) =>
               // Remove the extra type def from order
               order :=
                 order^
                 |> CCList.remove_one(
                      ~eq=CCList.equal(CCString.equal),
                      tr_path,
                    );
               td_type;
             | _ => fu_return
             }
           | _ => fu_return
           };

         let new_params =
           fu_params
           |> CCList.map(param => {
                switch (param) {
                | {fp_type: Reference({tr_path, _}), _} as fp
                    when
                      Hashtbl.find_opt(refCount, tr_path)
                      |> CCOpt.map(CCList.length)
                      |> CCOpt.value(~default=0) == 1 =>
                  switch (Hashtbl.find_opt(types, tr_path)) {
                  | Some(TypeDeclaration({td_type, _}) as t)
                      when canBeInline(t) =>
                    // Remove the extra type def from order
                    order :=
                      order^
                      |> CCList.remove_one(
                           ~eq=CCList.equal(CCString.equal),
                           tr_path,
                         );
                    // An replace the reference inside of the field
                    {...fp, fp_type: td_type};
                  | Some(_) => param
                  | None => param
                  }
                | param => param
                }
              });

         Hashtbl.replace(
           types,
           path,
           TypeDeclaration({
             ...td,
             td_type:
               Function({fu_return: new_return, fu_params: new_params}),
           }),
         );

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
                | {f_type: Reference({tr_path, _}), _}
                    when
                      Hashtbl.find_opt(refCount, tr_path)
                      |> CCOpt.map(CCList.length)
                      |> CCOpt.value(~default=0) == 1 =>
                  switch (Hashtbl.find_opt(types, tr_path)) {
                  | Some(TypeDeclaration({td_type, _}) as t)
                      when canBeInline(t) =>
                    // Remove the extra type def from order
                    order :=
                      order^
                      |> CCList.remove_one(
                           ~eq=CCList.equal(CCString.equal),
                           tr_path,
                         );
                    // An replace the reference inside of the field
                    {...field, f_type: td_type};
                  | Some(_) => field
                  | None => field
                  }

                | field => field
                }
              });

         Hashtbl.replace(
           types,
           path,
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
                | Reference({tr_path, _})
                    when
                      Hashtbl.find_opt(refCount, tr_path)
                      |> CCOpt.map(CCList.length)
                      |> CCOpt.value(~default=0) == 1 =>
                  switch (Hashtbl.find_opt(types, tr_path)) {
                  | Some(TypeDeclaration({td_type, _}))
                      when canBeInline(td_type) =>
                    // Remove the extra type def from order
                    order :=
                      order^
                      |> CCList.remove_one(
                           ~eq=CCList.equal(CCString.equal),
                           tr_path,
                         );
                    // An replace the reference inside of the field
                    td_type;
                  | Some(_) => member
                  | None => member
                  }
                | member => member
                }
              });

         Hashtbl.replace(
           types,
           path,
           TypeDeclaration({...td, td_type: Tuple(new_members)}),
         );

       | _ => ()
       }
     });

  (order^, types);
};

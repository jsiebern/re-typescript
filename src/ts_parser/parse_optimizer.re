module Types = Re_typescript_printer.Tree_types;
open Types;
open Parse_utils;

type types = Hashtbl.t(identPath, ts_type);
let refCount: Hashtbl.t(identPath, list(identPath)) = Hashtbl.create(10);

let canBeInline = (t: ts_type) =>
  switch (t) {
  | Interface(_) => false
  | _ => true
  };

let optimizeReferences = (order: list(identPath), types: types) => {
  let rec walk = (t: ts_type) => {
    // Console.log(Re_typescript_printer.Tree_utils.ts_to_string(t));
    switch (t) {
    | TypeDeclaration({td_type, td_parameters, _}) =>
      [td_type, ...td_parameters |> CCList.filter_map(tp => tp.tp_default)]
      |> CCList.iter(walk)
    | Function({fu_return, fu_params}) =>
      [fu_return, ...fu_params |> CCList.map(p => p.fp_type)]
      |> CCList.iter(walk)
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
       switch (Hashtbl.find_opt(types, path)) {
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

       | _ => ()
       }
     });

  (order^, types);
};

// open Migrate_parsetree;
 // open Re_typescript_base;
 // open Ast_406;
 // open Parsetree;
 // open Ast_helper;
 // let prim_type = name => {
 //   Ast_helper.Typ.constr(
 //     {Location.txt: Longident.parse(name), loc: Location.none},
 //     [],
 //   );
 // };
 // let rec base_type =
 //   fun
 //   | `String => (Ptype_abstract, Some(prim_type("string")))
 //   | `Number => (Ptype_abstract, Some(prim_type("int")))
 //   | `Boolean => (Ptype_abstract, Some(prim_type("bool")))
 //   | `Obj(fields) => (
 //       Ptype_record(
 //         fields
 //         |> List.map((obj_field: Ts.obj_field) =>
 //              Type.field(
 //                Location.mknoloc(obj_field.key),
 //                switch (base_type(obj_field.type_)) {
 //                | (_, Some(t)) => t
 //                | _ => raise(Not_found)
 //                },
 //              )
 //            ),
 //       ),
 //       None,
 //     )
 //   | _ => (Ptype_abstract, Some(prim_type("unit")));
 // let generate = (toplevel: Ts.toplevel) => {
 //   [
 //     Str.type_(
 //       Recursive,
 //       List.concat(
 //         toplevel.types
 //         |> List.map(
 //              fun
 //              | (`TypeDef(name, t), _) => {
 //                  let (kind, manifest) = base_type(t);
 //                  [Type.mk(~kind, ~manifest?, Location.mknoloc(name))];
 //                }
 //              | (`InterfaceDef(name, _, fields), _) => {
 //                  let (kind, manifest) = base_type(`Obj(fields));
 //                  [Type.mk(~kind, ~manifest?, Location.mknoloc(name))];
 //                },
 //            ),
 //       ),
 //     ),
 // ];
 // };
 /***/

open Migrate_parsetree;
open Re_typescript_base;

open Ast_411;
open Parsetree;
open Ast_helper;

let rec base_type =
  fun
  | `String => (Ptype_abstract, Some([%type: string]))
  | `Number => (Ptype_abstract, Some([%type: int]))
  | `Boolean => (Ptype_abstract, Some([%type: bool]))
  | `Obj(fields) => (
      Ptype_record(
        fields
        |> List.map((obj_field: Ts.obj_field) =>
             Type.field(
               Location.mknoloc(obj_field.key),
               switch (base_type(obj_field.type_)) {
               | (_, Some(t)) => t
               | _ => raise(Not_found)
               },
             )
           ),
      ),
      None,
    )
  | _ => (Ptype_abstract, Some([%type: unit]));

let generate = (toplevel: Ts.toplevel) => {
  [
    Str.type_(
      Recursive,
      List.concat(
        toplevel.types
        |> List.map(
             fun
             | `TypeDef(name, t) => {
                 let (kind, manifest) = base_type(t);

                 [Type.mk(~kind, ~manifest?, Location.mknoloc(name))];
               }
             | `InterfaceDef(name, _, fields) => {
                 let (kind, manifest) = base_type(`Obj(fields));

                 [Type.mk(~kind, ~manifest?, Location.mknoloc(name))];
               },
           ),
      ),
    ),
  ];
};
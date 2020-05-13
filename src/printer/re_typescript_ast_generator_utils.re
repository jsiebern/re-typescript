open Migrate_parsetree;
open Ast_406;
open Parsetree;
open Ast_helper;

let generate_base_type = name => {
  Typ.constr({Location.txt: Longident.parse(name), loc: Location.none}, []);
};

let generate_record_kind = (fields: list((string, core_type))) =>
  Ptype_record(
    fields
    |> List.map(((name, type_)) =>
         Type.field(Location.mknoloc(name), type_)
       ),
  );

open Migrate_parsetree;
open Ast_406;
open Parsetree;
open Ast_helper;

let generate_base_type = name => {
  Typ.constr({Location.txt: Longident.parse(name), loc: Location.none}, []);
};

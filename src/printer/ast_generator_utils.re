open Migrate_parsetree;
open Ast_406;
open Parsetree;
open Ast_helper;

let generate_base_type = (~inner=[], name) => {
  Typ.constr(
    {Location.txt: Longident.parse(name), loc: Location.none},
    inner,
  );
};

let generate_record_kind = (fields: list((string, core_type))) =>
  Ptype_record(
    fields
    |> List.map(((name, type_)) =>
         Type.field(Location.mknoloc(name), type_)
       ),
  );

let generate_array_of = wrap_type => {
  generate_base_type(~inner=[wrap_type], "array");
};

let generate_list_of = wrap_type => {
  generate_base_type(~inner=[wrap_type], "list");
};

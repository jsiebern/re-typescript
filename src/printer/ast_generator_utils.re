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

let generate_record_kind =
    (fields: list((string, core_type, list(attribute)))) =>
  Ptype_record(
    fields
    |> List.map(((name, type_, attrs)) =>
         Type.field(~attrs, Location.mknoloc(name), type_)
       ),
  );

let generate_array_of = wrap_type => {
  generate_base_type(~inner=[wrap_type], "array");
};

let generate_list_of = wrap_type => {
  generate_base_type(~inner=[wrap_type], "list");
};

let generate_option_of = wrap_type => {
  generate_base_type(~inner=[wrap_type], "option");
};

let generate_tuple_of = types => {
  Typ.tuple(types);
};

let generate_variant_kind = (names: list(string)) => {
  Ptype_variant(
    names
    |> CCListLabels.map(~f=name => Type.constructor(Location.mknoloc(name))),
  );
};

let generate_poly_variant = (names: list(string)) => {
  Typ.variant(
    names
    |> CCListLabels.map(~f=name =>
         Rtag(Location.mknoloc(name), [], false, [])
       ),
    Closed,
    None,
  );
};

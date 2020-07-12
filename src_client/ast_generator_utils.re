open Migrate_parsetree;
open Ast_406;
open Parsetree;
open Ast_helper;

let loc = Location.none;

module Naming = {
  let moduleName = str => CCString.capitalize_ascii(str);
  let typeName = str => CCString.uncapitalize_ascii(str);

  let fromIdentifier = (i: Ast.Identifier.t(Ast.Identifier.Constraint.any)) =>
    switch (i) {
    | Module(str) => moduleName(str)
    | VariantIdentifier(str) => moduleName(str)
    | TypeName(str) => typeName(str)
    | PropertyName(str) => typeName(str)
    | SubName(str) => typeName(str)
    | SubIdent(num) => Printf.sprintf("_%i", num)
    };
};

let make_module = (moduleName, moduleItems): Parsetree.structure_item => {
  pstr_desc:
    Parsetree.Pstr_module({
      pmb_name: {
        txt: moduleName,
        loc,
      },
      pmb_expr: {
        pmod_desc: Parsetree.Pmod_structure(moduleItems),
        pmod_loc: loc,
        pmod_attributes: [],
      },
      pmb_attributes: [],
      pmb_loc: loc,
    }),
  pstr_loc: loc,
};

let make_type_declaration =
    (~aliasName: string, ~aliasType: core_type): Parsetree.structure_item => {
  pstr_desc:
    Parsetree.Pstr_type(
      Asttypes.Recursive,
      [
        {
          ptype_name: {
            txt: aliasName,
            loc,
          },
          ptype_params: [],
          ptype_cstrs: [],
          ptype_kind: Parsetree.Ptype_abstract,
          ptype_private: Asttypes.Public,
          ptype_manifest: Some(aliasType),
          ptype_attributes: [],
          ptype_loc: loc,
        },
      ],
    ),
  pstr_loc: loc,
};
let make_type_declaration_of_kind =
    (~aliasName: string, ~kind: type_kind): Parsetree.structure_item => {
  pstr_desc:
    Parsetree.Pstr_type(
      Asttypes.Recursive,
      [
        {
          ptype_name: {
            txt: aliasName,
            loc,
          },
          ptype_params: [],
          ptype_cstrs: [],
          ptype_kind: kind,
          ptype_private: Asttypes.Public,
          ptype_manifest: None,
          ptype_attributes: [],
          ptype_loc: loc,
        },
      ],
    ),
  pstr_loc: loc,
};

let make_type_constraint = (~inner=[], name) => {
  Typ.constr(Location.mknoloc(Longident.parse(name)), inner);
};

let make_array_of = wrap_type => {
  make_type_constraint(~inner=[wrap_type], "array");
};

let make_list_of = wrap_type => {
  make_type_constraint(~inner=[wrap_type], "list");
};

let make_option_of = wrap_type => {
  make_type_constraint(~inner=[wrap_type], "option");
};

let make_tuple_of = types => {
  Typ.tuple(types);
};

let make_any_helper_unboxed = () => [%str
  [@unboxed]
  type any =
    | Any('a): any
];

let make_variant_kind = (names: list(string)) => {
  Ptype_variant(
    names
    |> CCListLabels.map(~f=name => Type.constructor(Location.mknoloc(name))),
  );
};

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

let make_tuple_of = (types: array(core_type)) =>
  Typ.tuple(types |> CCArray.to_list);

// let rec make_arrow_for_params =
//         (
//           ~has_opt=false,
//           types: list((option(string), bool, core_type)),
//           final: core_type,
//         ) => {
//   switch (types) {
//   | [] =>
//     has_opt
//       ? generate_arrow_for_params(
//           ~has_opt=false,
//           [(None, false, generate_base_type("unit"))],
//           final,
//         )
//       : final
//   | [(label, optional, type_), ...rest] =>
//     Typ.arrow(
//       switch (label, optional) {
//       | (None, _) => Nolabel
//       | (Some(l), false) => Labelled(l)
//       | (Some(l), true) => Optional(l)
//       },
//       optional ? generate_base_type(~inner=[type_], "option") : type_,
//       generate_arrow_for_params(
//         ~has_opt=has_opt ? has_opt : optional,
//         rest,
//         final,
//       ),
//     )
//   };
// };
// let make_arrow =
//     (~params: list((option(string), bool, core_type)), ~return: core_type) => {
//   switch (params) {
//   | [] => make_arrow_for_params(~has_opt=true, [], return)
//   | lst => make_arrow_for_params(lst, return)
//   };
// };

let make_function_type = (params, returnType) =>
  CCArray.fold_right(
    ((name, is_optional, paramType), t) =>
      Parsetree.{
        ptyp_desc:
          Parsetree.Ptyp_arrow(
            switch (name, is_optional) {
            | (Some(name), true) => Asttypes.Optional(name)
            | (Some(name), false) => Asttypes.Labelled(name)
            | (None, false) => Asttypes.Nolabel
            | (None, true) =>
              raise(
                Failure("Cannot have unlabelled optional function argument"),
              )
            },
            paramType,
            t,
          ),
        ptyp_loc: loc,
        ptyp_attributes: [],
      },
    params,
    returnType,
  );

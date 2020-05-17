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
    |> Tablecloth.List.map(~f=name =>
         Type.constructor(Location.mknoloc(name))
       ),
  );
};

let generate_poly_variant = (names: list(string)) => {
  Typ.variant(
    names
    |> Tablecloth.List.map(~f=name =>
         Rtag(Location.mknoloc(name), [], false, [])
       ),
    Closed,
    None,
  );
};

let generate_any = () => [
  {
    pstr_loc: Ast_helper.default_loc.contents,
    pstr_desc:
      [@implicit_arity]
      Pstr_type(
        Ast_406.Asttypes.Recursive,
        [
          {
            ptype_loc: Ast_helper.default_loc.contents,
            ptype_attributes: [
              (
                {
                  Ast_406.Asttypes.loc: Ast_helper.default_loc.contents,
                  Ast_406.Asttypes.txt: "unboxed",
                },
                PStr([]),
              ),
            ],
            ptype_manifest: None,
            ptype_private: Ast_406.Asttypes.Public,
            ptype_kind:
              Ptype_variant([
                {
                  pcd_attributes: [],
                  pcd_loc: Ast_helper.default_loc.contents,
                  pcd_res:
                    Some({
                      ptyp_attributes: [],
                      ptyp_loc: Ast_helper.default_loc.contents,
                      ptyp_desc:
                        [@implicit_arity]
                        Ptyp_constr(
                          {
                            Ast_406.Asttypes.loc:
                              Ast_helper.default_loc.contents,
                            Ast_406.Asttypes.txt:
                              Ast_406.Longident.Lident("any"),
                          },
                          [],
                        ),
                    }),
                  pcd_args:
                    Pcstr_tuple([
                      {
                        ptyp_attributes: [],
                        ptyp_loc: Ast_helper.default_loc.contents,
                        ptyp_desc: Ptyp_var("a"),
                      },
                    ]),
                  pcd_name: {
                    Ast_406.Asttypes.loc: Ast_helper.default_loc.contents,
                    Ast_406.Asttypes.txt: "Any",
                  },
                },
              ]),
            ptype_cstrs: [],
            ptype_params: [],
            ptype_name: {
              Ast_406.Asttypes.loc: Ast_helper.default_loc.contents,
              Ast_406.Asttypes.txt: "any",
            },
          },
        ],
      ),
  },
];

open Migrate_parsetree;
open Ast_406;
open Parsetree;

let number_unboxed = [
  {
    pstr_loc: Ast_helper.default_loc.contents,
    pstr_desc:
      Pstr_module({
        pmb_loc: Ast_helper.default_loc.contents,
        pmb_attributes: [],
        pmb_expr: {
          pmod_attributes: [],
          pmod_loc: Ast_helper.default_loc.contents,
          pmod_desc:
            [@implicit_arity]
            Pmod_constraint(
              {
                pmod_attributes: [],
                pmod_loc: Ast_helper.default_loc.contents,
                pmod_desc:
                  Pmod_structure([
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
                                    Ast_406.Asttypes.loc:
                                      Ast_helper.default_loc.contents,
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
                                        ptyp_loc:
                                          Ast_helper.default_loc.contents,
                                        ptyp_desc:
                                          [@implicit_arity]
                                          Ptyp_constr(
                                            {
                                              Ast_406.Asttypes.loc:
                                                Ast_helper.default_loc.
                                                  contents,
                                              Ast_406.Asttypes.txt:
                                                Ast_406.Longident.Lident("t"),
                                            },
                                            [],
                                          ),
                                      }),
                                    pcd_args:
                                      Pcstr_tuple([
                                        {
                                          ptyp_attributes: [],
                                          ptyp_loc:
                                            Ast_helper.default_loc.contents,
                                          ptyp_desc: Ptyp_var("a"),
                                        },
                                      ]),
                                    pcd_name: {
                                      Ast_406.Asttypes.loc:
                                        Ast_helper.default_loc.contents,
                                      Ast_406.Asttypes.txt: "Number",
                                    },
                                  },
                                ]),
                              ptype_cstrs: [],
                              ptype_params: [],
                              ptype_name: {
                                Ast_406.Asttypes.loc:
                                  Ast_helper.default_loc.contents,
                                Ast_406.Asttypes.txt: "t",
                              },
                            },
                          ],
                        ),
                    },
                    {
                      pstr_loc: Ast_helper.default_loc.contents,
                      pstr_desc:
                        [@implicit_arity]
                        Pstr_value(
                          Ast_406.Asttypes.Nonrecursive,
                          [
                            {
                              pvb_loc: Ast_helper.default_loc.contents,
                              pvb_attributes: [],
                              pvb_expr: {
                                pexp_attributes: [],
                                pexp_loc: Ast_helper.default_loc.contents,
                                pexp_desc:
                                  [@implicit_arity]
                                  Pexp_fun(
                                    Ast_406.Asttypes.Nolabel,
                                    None,
                                    {
                                      ppat_attributes: [],
                                      ppat_loc:
                                        Ast_helper.default_loc.contents,
                                      ppat_desc:
                                        [@implicit_arity]
                                        Ppat_constraint(
                                          {
                                            ppat_attributes: [],
                                            ppat_loc:
                                              Ast_helper.default_loc.contents,
                                            ppat_desc:
                                              Ppat_var({
                                                Ast_406.Asttypes.loc:
                                                  Ast_helper.default_loc.
                                                    contents,
                                                Ast_406.Asttypes.txt: "v",
                                              }),
                                          },
                                          {
                                            ptyp_attributes: [],
                                            ptyp_loc:
                                              Ast_helper.default_loc.contents,
                                            ptyp_desc:
                                              [@implicit_arity]
                                              Ptyp_constr(
                                                {
                                                  Ast_406.Asttypes.loc:
                                                    Ast_helper.default_loc.
                                                      contents,
                                                  Ast_406.Asttypes.txt:
                                                    Ast_406.Longident.Lident(
                                                      "float",
                                                    ),
                                                },
                                                [],
                                              ),
                                          },
                                        ),
                                    },
                                    {
                                      pexp_attributes: [
                                        (
                                          {
                                            Ast_406.Asttypes.loc:
                                              Ast_helper.default_loc.contents,
                                            Ast_406.Asttypes.txt: "explicit_arity",
                                          },
                                          PStr([]),
                                        ),
                                      ],
                                      pexp_loc:
                                        Ast_helper.default_loc.contents,
                                      pexp_desc:
                                        [@implicit_arity]
                                        Pexp_construct(
                                          {
                                            Ast_406.Asttypes.loc:
                                              Ast_helper.default_loc.contents,
                                            Ast_406.Asttypes.txt:
                                              Ast_406.Longident.Lident(
                                                "Number",
                                              ),
                                          },
                                          Some({
                                            pexp_attributes: [],
                                            pexp_loc:
                                              Ast_helper.default_loc.contents,
                                            pexp_desc:
                                              Pexp_tuple([
                                                {
                                                  pexp_attributes: [],
                                                  pexp_loc:
                                                    Ast_helper.default_loc.
                                                      contents,
                                                  pexp_desc:
                                                    Pexp_ident({
                                                      Ast_406.Asttypes.loc:
                                                        Ast_helper.default_loc.
                                                          contents,
                                                      Ast_406.Asttypes.txt:
                                                        Ast_406.Longident.Lident(
                                                          "v",
                                                        ),
                                                    }),
                                                },
                                              ]),
                                          }),
                                        ),
                                    },
                                  ),
                              },
                              pvb_pat: {
                                ppat_attributes: [],
                                ppat_loc: Ast_helper.default_loc.contents,
                                ppat_desc:
                                  Ppat_var({
                                    Ast_406.Asttypes.loc:
                                      Ast_helper.default_loc.contents,
                                    Ast_406.Asttypes.txt: "float",
                                  }),
                              },
                            },
                          ],
                        ),
                    },
                    {
                      pstr_loc: Ast_helper.default_loc.contents,
                      pstr_desc:
                        [@implicit_arity]
                        Pstr_value(
                          Ast_406.Asttypes.Nonrecursive,
                          [
                            {
                              pvb_loc: Ast_helper.default_loc.contents,
                              pvb_attributes: [],
                              pvb_expr: {
                                pexp_attributes: [],
                                pexp_loc: Ast_helper.default_loc.contents,
                                pexp_desc:
                                  [@implicit_arity]
                                  Pexp_fun(
                                    Ast_406.Asttypes.Nolabel,
                                    None,
                                    {
                                      ppat_attributes: [],
                                      ppat_loc:
                                        Ast_helper.default_loc.contents,
                                      ppat_desc:
                                        [@implicit_arity]
                                        Ppat_constraint(
                                          {
                                            ppat_attributes: [],
                                            ppat_loc:
                                              Ast_helper.default_loc.contents,
                                            ppat_desc:
                                              Ppat_var({
                                                Ast_406.Asttypes.loc:
                                                  Ast_helper.default_loc.
                                                    contents,
                                                Ast_406.Asttypes.txt: "v",
                                              }),
                                          },
                                          {
                                            ptyp_attributes: [],
                                            ptyp_loc:
                                              Ast_helper.default_loc.contents,
                                            ptyp_desc:
                                              [@implicit_arity]
                                              Ptyp_constr(
                                                {
                                                  Ast_406.Asttypes.loc:
                                                    Ast_helper.default_loc.
                                                      contents,
                                                  Ast_406.Asttypes.txt:
                                                    Ast_406.Longident.Lident(
                                                      "int",
                                                    ),
                                                },
                                                [],
                                              ),
                                          },
                                        ),
                                    },
                                    {
                                      pexp_attributes: [
                                        (
                                          {
                                            Ast_406.Asttypes.loc:
                                              Ast_helper.default_loc.contents,
                                            Ast_406.Asttypes.txt: "explicit_arity",
                                          },
                                          PStr([]),
                                        ),
                                      ],
                                      pexp_loc:
                                        Ast_helper.default_loc.contents,
                                      pexp_desc:
                                        [@implicit_arity]
                                        Pexp_construct(
                                          {
                                            Ast_406.Asttypes.loc:
                                              Ast_helper.default_loc.contents,
                                            Ast_406.Asttypes.txt:
                                              Ast_406.Longident.Lident(
                                                "Number",
                                              ),
                                          },
                                          Some({
                                            pexp_attributes: [],
                                            pexp_loc:
                                              Ast_helper.default_loc.contents,
                                            pexp_desc:
                                              Pexp_tuple([
                                                {
                                                  pexp_attributes: [],
                                                  pexp_loc:
                                                    Ast_helper.default_loc.
                                                      contents,
                                                  pexp_desc:
                                                    Pexp_ident({
                                                      Ast_406.Asttypes.loc:
                                                        Ast_helper.default_loc.
                                                          contents,
                                                      Ast_406.Asttypes.txt:
                                                        Ast_406.Longident.Lident(
                                                          "v",
                                                        ),
                                                    }),
                                                },
                                              ]),
                                          }),
                                        ),
                                    },
                                  ),
                              },
                              pvb_pat: {
                                ppat_attributes: [],
                                ppat_loc: Ast_helper.default_loc.contents,
                                ppat_desc:
                                  Ppat_var({
                                    Ast_406.Asttypes.loc:
                                      Ast_helper.default_loc.contents,
                                    Ast_406.Asttypes.txt: "int",
                                  }),
                              },
                            },
                          ],
                        ),
                    },
                    {
                      pstr_loc: Ast_helper.default_loc.contents,
                      pstr_desc:
                        [@implicit_arity]
                        Pstr_type(
                          Ast_406.Asttypes.Recursive,
                          [
                            {
                              ptype_loc: Ast_helper.default_loc.contents,
                              ptype_attributes: [],
                              ptype_manifest: None,
                              ptype_private: Ast_406.Asttypes.Public,
                              ptype_kind:
                                Ptype_variant([
                                  {
                                    pcd_attributes: [],
                                    pcd_loc: Ast_helper.default_loc.contents,
                                    pcd_res: None,
                                    pcd_args:
                                      Pcstr_tuple([
                                        {
                                          ptyp_attributes: [],
                                          ptyp_loc:
                                            Ast_helper.default_loc.contents,
                                          ptyp_desc:
                                            [@implicit_arity]
                                            Ptyp_constr(
                                              {
                                                Ast_406.Asttypes.loc:
                                                  Ast_helper.default_loc.
                                                    contents,
                                                Ast_406.Asttypes.txt:
                                                  Ast_406.Longident.Lident(
                                                    "int",
                                                  ),
                                              },
                                              [],
                                            ),
                                        },
                                      ]),
                                    pcd_name: {
                                      Ast_406.Asttypes.loc:
                                        Ast_helper.default_loc.contents,
                                      Ast_406.Asttypes.txt: "Int",
                                    },
                                  },
                                  {
                                    pcd_attributes: [],
                                    pcd_loc: Ast_helper.default_loc.contents,
                                    pcd_res: None,
                                    pcd_args:
                                      Pcstr_tuple([
                                        {
                                          ptyp_attributes: [],
                                          ptyp_loc:
                                            Ast_helper.default_loc.contents,
                                          ptyp_desc:
                                            [@implicit_arity]
                                            Ptyp_constr(
                                              {
                                                Ast_406.Asttypes.loc:
                                                  Ast_helper.default_loc.
                                                    contents,
                                                Ast_406.Asttypes.txt:
                                                  Ast_406.Longident.Lident(
                                                    "float",
                                                  ),
                                              },
                                              [],
                                            ),
                                        },
                                      ]),
                                    pcd_name: {
                                      Ast_406.Asttypes.loc:
                                        Ast_helper.default_loc.contents,
                                      Ast_406.Asttypes.txt: "Float",
                                    },
                                  },
                                ]),
                              ptype_cstrs: [],
                              ptype_params: [],
                              ptype_name: {
                                Ast_406.Asttypes.loc:
                                  Ast_helper.default_loc.contents,
                                Ast_406.Asttypes.txt: "case",
                              },
                            },
                          ],
                        ),
                    },
                    {
                      pstr_loc: Ast_helper.default_loc.contents,
                      pstr_desc:
                        [@implicit_arity]
                        Pstr_value(
                          Ast_406.Asttypes.Nonrecursive,
                          [
                            {
                              pvb_loc: Ast_helper.default_loc.contents,
                              pvb_attributes: [],
                              pvb_expr: {
                                pexp_attributes: [],
                                pexp_loc: Ast_helper.default_loc.contents,
                                pexp_desc:
                                  [@implicit_arity]
                                  Pexp_fun(
                                    Ast_406.Asttypes.Nolabel,
                                    None,
                                    {
                                      ppat_attributes: [],
                                      ppat_loc:
                                        Ast_helper.default_loc.contents,
                                      ppat_desc:
                                        Ppat_var({
                                          Ast_406.Asttypes.loc:
                                            Ast_helper.default_loc.contents,
                                          Ast_406.Asttypes.txt: "v",
                                        }),
                                    },
                                    {
                                      pexp_attributes: [],
                                      pexp_loc:
                                        Ast_helper.default_loc.contents,
                                      pexp_desc:
                                        [@implicit_arity]
                                        Pexp_apply(
                                          {
                                            pexp_attributes: [],
                                            pexp_loc:
                                              Ast_helper.default_loc.contents,
                                            pexp_desc:
                                              Pexp_ident({
                                                Ast_406.Asttypes.loc:
                                                  Ast_helper.default_loc.
                                                    contents,
                                                Ast_406.Asttypes.txt:
                                                  Ast_406.Longident.Lident(
                                                    "&&",
                                                  ),
                                              }),
                                          },
                                          [
                                            (
                                              Ast_406.Asttypes.Nolabel,
                                              {
                                                pexp_attributes: [],
                                                pexp_loc:
                                                  Ast_helper.default_loc.
                                                    contents,
                                                pexp_desc:
                                                  [@implicit_arity]
                                                  Pexp_apply(
                                                    {
                                                      pexp_attributes: [],
                                                      pexp_loc:
                                                        Ast_helper.default_loc.
                                                          contents,
                                                      pexp_desc:
                                                        Pexp_ident({
                                                          Ast_406.Asttypes.loc:
                                                            Ast_helper.default_loc.
                                                              contents,
                                                          Ast_406.Asttypes.txt:
                                                            Ast_406.Longident.Lident(
                                                              "==",
                                                            ),
                                                        }),
                                                    },
                                                    [
                                                      (
                                                        Ast_406.Asttypes.Nolabel,
                                                        {
                                                          pexp_attributes: [],
                                                          pexp_loc:
                                                            Ast_helper.default_loc.
                                                              contents,
                                                          pexp_desc:
                                                            [@implicit_arity]
                                                            Pexp_apply(
                                                              {
                                                                pexp_attributes:
                                                                  [],
                                                                pexp_loc:
                                                                  Ast_helper.default_loc.
                                                                    contents,
                                                                pexp_desc:
                                                                  Pexp_ident({
                                                                    Ast_406.Asttypes.loc:
                                                                    Ast_helper.default_loc.
                                                                    contents,
                                                                    Ast_406.Asttypes.txt:
                                                                    [@implicit_arity]
                                                                    Ast_406.Longident.Ldot(
                                                                    Ast_406.Longident.Lident(
                                                                    "Js",
                                                                    ),
                                                                    "typeof",
                                                                    ),
                                                                  }),
                                                              },
                                                              [
                                                                (
                                                                  Ast_406.Asttypes.Nolabel,
                                                                  {
                                                                    pexp_attributes:
                                                                    [],
                                                                    pexp_loc:
                                                                    Ast_helper.default_loc.
                                                                    contents,
                                                                    pexp_desc:
                                                                    Pexp_ident({
                                                                    Ast_406.Asttypes.loc:
                                                                    Ast_helper.default_loc.
                                                                    contents,
                                                                    Ast_406.Asttypes.txt:
                                                                    Ast_406.Longident.Lident(
                                                                    "v",
                                                                    ),
                                                                    }),
                                                                  },
                                                                ),
                                                              ],
                                                            ),
                                                        },
                                                      ),
                                                      (
                                                        Ast_406.Asttypes.Nolabel,
                                                        {
                                                          pexp_attributes: [],
                                                          pexp_loc:
                                                            Ast_helper.default_loc.
                                                              contents,
                                                          pexp_desc:
                                                            Pexp_constant(
                                                              [@implicit_arity]
                                                              Pconst_string(
                                                                "number",
                                                                None,
                                                              ),
                                                            ),
                                                        },
                                                      ),
                                                    ],
                                                  ),
                                              },
                                            ),
                                            (
                                              Ast_406.Asttypes.Nolabel,
                                              {
                                                pexp_attributes: [],
                                                pexp_loc:
                                                  Ast_helper.default_loc.
                                                    contents,
                                                pexp_desc:
                                                  [@implicit_arity]
                                                  Pexp_apply(
                                                    {
                                                      pexp_attributes: [],
                                                      pexp_loc:
                                                        Ast_helper.default_loc.
                                                          contents,
                                                      pexp_desc:
                                                        Pexp_ident({
                                                          Ast_406.Asttypes.loc:
                                                            Ast_helper.default_loc.
                                                              contents,
                                                          Ast_406.Asttypes.txt:
                                                            Ast_406.Longident.Lident(
                                                              "&&",
                                                            ),
                                                        }),
                                                    },
                                                    [
                                                      (
                                                        Ast_406.Asttypes.Nolabel,
                                                        {
                                                          pexp_attributes: [],
                                                          pexp_loc:
                                                            Ast_helper.default_loc.
                                                              contents,
                                                          pexp_desc:
                                                            [@implicit_arity]
                                                            Pexp_apply(
                                                              {
                                                                pexp_attributes:
                                                                  [],
                                                                pexp_loc:
                                                                  Ast_helper.default_loc.
                                                                    contents,
                                                                pexp_desc:
                                                                  Pexp_ident({
                                                                    Ast_406.Asttypes.loc:
                                                                    Ast_helper.default_loc.
                                                                    contents,
                                                                    Ast_406.Asttypes.txt:
                                                                    [@implicit_arity]
                                                                    Ast_406.Longident.Ldot(
                                                                    [@implicit_arity]
                                                                    Ast_406.Longident.Ldot(
                                                                    Ast_406.Longident.Lident(
                                                                    "Js",
                                                                    ),
                                                                    "Float",
                                                                    ),
                                                                    "isFinite",
                                                                    ),
                                                                  }),
                                                              },
                                                              [
                                                                (
                                                                  Ast_406.Asttypes.Nolabel,
                                                                  {
                                                                    pexp_attributes:
                                                                    [],
                                                                    pexp_loc:
                                                                    Ast_helper.default_loc.
                                                                    contents,
                                                                    pexp_desc:
                                                                    Pexp_ident({
                                                                    Ast_406.Asttypes.loc:
                                                                    Ast_helper.default_loc.
                                                                    contents,
                                                                    Ast_406.Asttypes.txt:
                                                                    Ast_406.Longident.Lident(
                                                                    "v",
                                                                    ),
                                                                    }),
                                                                  },
                                                                ),
                                                              ],
                                                            ),
                                                        },
                                                      ),
                                                      (
                                                        Ast_406.Asttypes.Nolabel,
                                                        {
                                                          pexp_attributes: [],
                                                          pexp_loc:
                                                            Ast_helper.default_loc.
                                                              contents,
                                                          pexp_desc:
                                                            [@implicit_arity]
                                                            Pexp_apply(
                                                              {
                                                                pexp_attributes:
                                                                  [],
                                                                pexp_loc:
                                                                  Ast_helper.default_loc.
                                                                    contents,
                                                                pexp_desc:
                                                                  Pexp_ident({
                                                                    Ast_406.Asttypes.loc:
                                                                    Ast_helper.default_loc.
                                                                    contents,
                                                                    Ast_406.Asttypes.txt:
                                                                    Ast_406.Longident.Lident(
                                                                    "==",
                                                                    ),
                                                                  }),
                                                              },
                                                              [
                                                                (
                                                                  Ast_406.Asttypes.Nolabel,
                                                                  {
                                                                    pexp_attributes:
                                                                    [],
                                                                    pexp_loc:
                                                                    Ast_helper.default_loc.
                                                                    contents,
                                                                    pexp_desc:
                                                                    [@implicit_arity]
                                                                    Pexp_apply(
                                                                    {
                                                                    pexp_attributes:
                                                                    [],
                                                                    pexp_loc:
                                                                    Ast_helper.default_loc.
                                                                    contents,
                                                                    pexp_desc:
                                                                    Pexp_ident({
                                                                    Ast_406.Asttypes.loc:
                                                                    Ast_helper.default_loc.
                                                                    contents,
                                                                    Ast_406.Asttypes.txt:
                                                                    [@implicit_arity]
                                                                    Ast_406.Longident.Ldot(
                                                                    [@implicit_arity]
                                                                    Ast_406.Longident.Ldot(
                                                                    Ast_406.Longident.Lident(
                                                                    "Js",
                                                                    ),
                                                                    "Math",
                                                                    ),
                                                                    "floor_float",
                                                                    ),
                                                                    }),
                                                                    },
                                                                    [
                                                                    (
                                                                    Ast_406.Asttypes.Nolabel,
                                                                    {
                                                                    pexp_attributes:
                                                                    [],
                                                                    pexp_loc:
                                                                    Ast_helper.default_loc.
                                                                    contents,
                                                                    pexp_desc:
                                                                    Pexp_ident({
                                                                    Ast_406.Asttypes.loc:
                                                                    Ast_helper.default_loc.
                                                                    contents,
                                                                    Ast_406.Asttypes.txt:
                                                                    Ast_406.Longident.Lident(
                                                                    "v",
                                                                    ),
                                                                    }),
                                                                    },
                                                                    ),
                                                                    ],
                                                                    ),
                                                                  },
                                                                ),
                                                                (
                                                                  Ast_406.Asttypes.Nolabel,
                                                                  {
                                                                    pexp_attributes:
                                                                    [],
                                                                    pexp_loc:
                                                                    Ast_helper.default_loc.
                                                                    contents,
                                                                    pexp_desc:
                                                                    Pexp_ident({
                                                                    Ast_406.Asttypes.loc:
                                                                    Ast_helper.default_loc.
                                                                    contents,
                                                                    Ast_406.Asttypes.txt:
                                                                    Ast_406.Longident.Lident(
                                                                    "v",
                                                                    ),
                                                                    }),
                                                                  },
                                                                ),
                                                              ],
                                                            ),
                                                        },
                                                      ),
                                                    ],
                                                  ),
                                              },
                                            ),
                                          ],
                                        ),
                                    },
                                  ),
                              },
                              pvb_pat: {
                                ppat_attributes: [],
                                ppat_loc: Ast_helper.default_loc.contents,
                                ppat_desc:
                                  Ppat_var({
                                    Ast_406.Asttypes.loc:
                                      Ast_helper.default_loc.contents,
                                    Ast_406.Asttypes.txt: "isInteger",
                                  }),
                              },
                            },
                          ],
                        ),
                    },
                    {
                      pstr_loc: Ast_helper.default_loc.contents,
                      pstr_desc:
                        [@implicit_arity]
                        Pstr_value(
                          Ast_406.Asttypes.Nonrecursive,
                          [
                            {
                              pvb_loc: Ast_helper.default_loc.contents,
                              pvb_attributes: [],
                              pvb_expr: {
                                pexp_attributes: [],
                                pexp_loc: Ast_helper.default_loc.contents,
                                pexp_desc:
                                  [@implicit_arity]
                                  Pexp_fun(
                                    Ast_406.Asttypes.Nolabel,
                                    None,
                                    {
                                      ppat_attributes: [],
                                      ppat_loc:
                                        Ast_helper.default_loc.contents,
                                      ppat_desc:
                                        [@implicit_arity]
                                        Ppat_constraint(
                                          {
                                            ppat_attributes: [
                                              (
                                                {
                                                  Ast_406.Asttypes.loc:
                                                    Ast_helper.default_loc.
                                                      contents,
                                                  Ast_406.Asttypes.txt: "explicit_arity",
                                                },
                                                PStr([]),
                                              ),
                                            ],
                                            ppat_loc:
                                              Ast_helper.default_loc.contents,
                                            ppat_desc:
                                              [@implicit_arity]
                                              Ppat_construct(
                                                {
                                                  Ast_406.Asttypes.loc:
                                                    Ast_helper.default_loc.
                                                      contents,
                                                  Ast_406.Asttypes.txt:
                                                    Ast_406.Longident.Lident(
                                                      "Number",
                                                    ),
                                                },
                                                Some({
                                                  ppat_attributes: [],
                                                  ppat_loc:
                                                    Ast_helper.default_loc.
                                                      contents,
                                                  ppat_desc:
                                                    Ppat_tuple([
                                                      {
                                                        ppat_attributes: [],
                                                        ppat_loc:
                                                          Ast_helper.default_loc.
                                                            contents,
                                                        ppat_desc:
                                                          Ppat_var({
                                                            Ast_406.Asttypes.loc:
                                                              Ast_helper.default_loc.
                                                                contents,
                                                            Ast_406.Asttypes.txt: "v",
                                                          }),
                                                      },
                                                    ]),
                                                }),
                                              ),
                                          },
                                          {
                                            ptyp_attributes: [],
                                            ptyp_loc:
                                              Ast_helper.default_loc.contents,
                                            ptyp_desc:
                                              [@implicit_arity]
                                              Ptyp_constr(
                                                {
                                                  Ast_406.Asttypes.loc:
                                                    Ast_helper.default_loc.
                                                      contents,
                                                  Ast_406.Asttypes.txt:
                                                    Ast_406.Longident.Lident(
                                                      "t",
                                                    ),
                                                },
                                                [],
                                              ),
                                          },
                                        ),
                                    },
                                    {
                                      pexp_attributes: [],
                                      pexp_loc:
                                        Ast_helper.default_loc.contents,
                                      pexp_desc:
                                        [@implicit_arity]
                                        Pexp_match(
                                          {
                                            pexp_attributes: [],
                                            pexp_loc:
                                              Ast_helper.default_loc.contents,
                                            pexp_desc:
                                              [@implicit_arity]
                                              Pexp_apply(
                                                {
                                                  pexp_attributes: [],
                                                  pexp_loc:
                                                    Ast_helper.default_loc.
                                                      contents,
                                                  pexp_desc:
                                                    Pexp_ident({
                                                      Ast_406.Asttypes.loc:
                                                        Ast_helper.default_loc.
                                                          contents,
                                                      Ast_406.Asttypes.txt:
                                                        Ast_406.Longident.Lident(
                                                          "isInteger",
                                                        ),
                                                    }),
                                                },
                                                [
                                                  (
                                                    Ast_406.Asttypes.Nolabel,
                                                    {
                                                      pexp_attributes: [],
                                                      pexp_loc:
                                                        Ast_helper.default_loc.
                                                          contents,
                                                      pexp_desc:
                                                        [@implicit_arity]
                                                        Pexp_apply(
                                                          {
                                                            pexp_attributes: [],
                                                            pexp_loc:
                                                              Ast_helper.default_loc.
                                                                contents,
                                                            pexp_desc:
                                                              Pexp_ident({
                                                                Ast_406.Asttypes.loc:
                                                                  Ast_helper.default_loc.
                                                                    contents,
                                                                Ast_406.Asttypes.txt:
                                                                  [@implicit_arity]
                                                                  Ast_406.Longident.Ldot(
                                                                    Ast_406.Longident.Lident(
                                                                    "Obj",
                                                                    ),
                                                                    "magic",
                                                                  ),
                                                              }),
                                                          },
                                                          [
                                                            (
                                                              Ast_406.Asttypes.Nolabel,
                                                              {
                                                                pexp_attributes:
                                                                  [],
                                                                pexp_loc:
                                                                  Ast_helper.default_loc.
                                                                    contents,
                                                                pexp_desc:
                                                                  Pexp_ident({
                                                                    Ast_406.Asttypes.loc:
                                                                    Ast_helper.default_loc.
                                                                    contents,
                                                                    Ast_406.Asttypes.txt:
                                                                    Ast_406.Longident.Lident(
                                                                    "v",
                                                                    ),
                                                                  }),
                                                              },
                                                            ),
                                                          ],
                                                        ),
                                                    },
                                                  ),
                                                ],
                                              ),
                                          },
                                          [
                                            {
                                              pc_rhs: {
                                                pexp_attributes: [
                                                  (
                                                    {
                                                      Ast_406.Asttypes.loc:
                                                        Ast_helper.default_loc.
                                                          contents,
                                                      Ast_406.Asttypes.txt: "explicit_arity",
                                                    },
                                                    PStr([]),
                                                  ),
                                                ],
                                                pexp_loc:
                                                  Ast_helper.default_loc.
                                                    contents,
                                                pexp_desc:
                                                  [@implicit_arity]
                                                  Pexp_construct(
                                                    {
                                                      Ast_406.Asttypes.loc:
                                                        Ast_helper.default_loc.
                                                          contents,
                                                      Ast_406.Asttypes.txt:
                                                        Ast_406.Longident.Lident(
                                                          "Int",
                                                        ),
                                                    },
                                                    Some({
                                                      pexp_attributes: [],
                                                      pexp_loc:
                                                        Ast_helper.default_loc.
                                                          contents,
                                                      pexp_desc:
                                                        Pexp_tuple([
                                                          {
                                                            pexp_attributes: [],
                                                            pexp_loc:
                                                              Ast_helper.default_loc.
                                                                contents,
                                                            pexp_desc:
                                                              [@implicit_arity]
                                                              Pexp_constraint(
                                                                {
                                                                  pexp_attributes:
                                                                    [],
                                                                  pexp_loc:
                                                                    Ast_helper.default_loc.
                                                                    contents,
                                                                  pexp_desc:
                                                                    [@implicit_arity]
                                                                    Pexp_apply(
                                                                    {
                                                                    pexp_attributes:
                                                                    [],
                                                                    pexp_loc:
                                                                    Ast_helper.default_loc.
                                                                    contents,
                                                                    pexp_desc:
                                                                    Pexp_ident({
                                                                    Ast_406.Asttypes.loc:
                                                                    Ast_helper.default_loc.
                                                                    contents,
                                                                    Ast_406.Asttypes.txt:
                                                                    [@implicit_arity]
                                                                    Ast_406.Longident.Ldot(
                                                                    Ast_406.Longident.Lident(
                                                                    "Obj",
                                                                    ),
                                                                    "magic",
                                                                    ),
                                                                    }),
                                                                    },
                                                                    [
                                                                    (
                                                                    Ast_406.Asttypes.Nolabel,
                                                                    {
                                                                    pexp_attributes:
                                                                    [],
                                                                    pexp_loc:
                                                                    Ast_helper.default_loc.
                                                                    contents,
                                                                    pexp_desc:
                                                                    Pexp_ident({
                                                                    Ast_406.Asttypes.loc:
                                                                    Ast_helper.default_loc.
                                                                    contents,
                                                                    Ast_406.Asttypes.txt:
                                                                    Ast_406.Longident.Lident(
                                                                    "v",
                                                                    ),
                                                                    }),
                                                                    },
                                                                    ),
                                                                    ],
                                                                    ),
                                                                },
                                                                {
                                                                  ptyp_attributes:
                                                                    [],
                                                                  ptyp_loc:
                                                                    Ast_helper.default_loc.
                                                                    contents,
                                                                  ptyp_desc:
                                                                    [@implicit_arity]
                                                                    Ptyp_constr(
                                                                    {
                                                                    Ast_406.Asttypes.loc:
                                                                    Ast_helper.default_loc.
                                                                    contents,
                                                                    Ast_406.Asttypes.txt:
                                                                    Ast_406.Longident.Lident(
                                                                    "int",
                                                                    ),
                                                                    },
                                                                    [],
                                                                    ),
                                                                },
                                                              ),
                                                          },
                                                        ]),
                                                    }),
                                                  ),
                                              },
                                              pc_guard: None,
                                              pc_lhs: {
                                                ppat_attributes: [],
                                                ppat_loc:
                                                  Ast_helper.default_loc.
                                                    contents,
                                                ppat_desc:
                                                  [@implicit_arity]
                                                  Ppat_construct(
                                                    {
                                                      Ast_406.Asttypes.loc:
                                                        Ast_helper.default_loc.
                                                          contents,
                                                      Ast_406.Asttypes.txt:
                                                        Ast_406.Longident.Lident(
                                                          "true",
                                                        ),
                                                    },
                                                    None,
                                                  ),
                                              },
                                            },
                                            {
                                              pc_rhs: {
                                                pexp_attributes: [
                                                  (
                                                    {
                                                      Ast_406.Asttypes.loc:
                                                        Ast_helper.default_loc.
                                                          contents,
                                                      Ast_406.Asttypes.txt: "explicit_arity",
                                                    },
                                                    PStr([]),
                                                  ),
                                                ],
                                                pexp_loc:
                                                  Ast_helper.default_loc.
                                                    contents,
                                                pexp_desc:
                                                  [@implicit_arity]
                                                  Pexp_construct(
                                                    {
                                                      Ast_406.Asttypes.loc:
                                                        Ast_helper.default_loc.
                                                          contents,
                                                      Ast_406.Asttypes.txt:
                                                        Ast_406.Longident.Lident(
                                                          "Float",
                                                        ),
                                                    },
                                                    Some({
                                                      pexp_attributes: [],
                                                      pexp_loc:
                                                        Ast_helper.default_loc.
                                                          contents,
                                                      pexp_desc:
                                                        Pexp_tuple([
                                                          {
                                                            pexp_attributes: [],
                                                            pexp_loc:
                                                              Ast_helper.default_loc.
                                                                contents,
                                                            pexp_desc:
                                                              [@implicit_arity]
                                                              Pexp_constraint(
                                                                {
                                                                  pexp_attributes:
                                                                    [],
                                                                  pexp_loc:
                                                                    Ast_helper.default_loc.
                                                                    contents,
                                                                  pexp_desc:
                                                                    [@implicit_arity]
                                                                    Pexp_apply(
                                                                    {
                                                                    pexp_attributes:
                                                                    [],
                                                                    pexp_loc:
                                                                    Ast_helper.default_loc.
                                                                    contents,
                                                                    pexp_desc:
                                                                    Pexp_ident({
                                                                    Ast_406.Asttypes.loc:
                                                                    Ast_helper.default_loc.
                                                                    contents,
                                                                    Ast_406.Asttypes.txt:
                                                                    [@implicit_arity]
                                                                    Ast_406.Longident.Ldot(
                                                                    Ast_406.Longident.Lident(
                                                                    "Obj",
                                                                    ),
                                                                    "magic",
                                                                    ),
                                                                    }),
                                                                    },
                                                                    [
                                                                    (
                                                                    Ast_406.Asttypes.Nolabel,
                                                                    {
                                                                    pexp_attributes:
                                                                    [],
                                                                    pexp_loc:
                                                                    Ast_helper.default_loc.
                                                                    contents,
                                                                    pexp_desc:
                                                                    Pexp_ident({
                                                                    Ast_406.Asttypes.loc:
                                                                    Ast_helper.default_loc.
                                                                    contents,
                                                                    Ast_406.Asttypes.txt:
                                                                    Ast_406.Longident.Lident(
                                                                    "v",
                                                                    ),
                                                                    }),
                                                                    },
                                                                    ),
                                                                    ],
                                                                    ),
                                                                },
                                                                {
                                                                  ptyp_attributes:
                                                                    [],
                                                                  ptyp_loc:
                                                                    Ast_helper.default_loc.
                                                                    contents,
                                                                  ptyp_desc:
                                                                    [@implicit_arity]
                                                                    Ptyp_constr(
                                                                    {
                                                                    Ast_406.Asttypes.loc:
                                                                    Ast_helper.default_loc.
                                                                    contents,
                                                                    Ast_406.Asttypes.txt:
                                                                    Ast_406.Longident.Lident(
                                                                    "float",
                                                                    ),
                                                                    },
                                                                    [],
                                                                    ),
                                                                },
                                                              ),
                                                          },
                                                        ]),
                                                    }),
                                                  ),
                                              },
                                              pc_guard: None,
                                              pc_lhs: {
                                                ppat_attributes: [],
                                                ppat_loc:
                                                  Ast_helper.default_loc.
                                                    contents,
                                                ppat_desc:
                                                  [@implicit_arity]
                                                  Ppat_construct(
                                                    {
                                                      Ast_406.Asttypes.loc:
                                                        Ast_helper.default_loc.
                                                          contents,
                                                      Ast_406.Asttypes.txt:
                                                        Ast_406.Longident.Lident(
                                                          "false",
                                                        ),
                                                    },
                                                    None,
                                                  ),
                                              },
                                            },
                                          ],
                                        ),
                                    },
                                  ),
                              },
                              pvb_pat: {
                                ppat_attributes: [],
                                ppat_loc: Ast_helper.default_loc.contents,
                                ppat_desc:
                                  Ppat_var({
                                    Ast_406.Asttypes.loc:
                                      Ast_helper.default_loc.contents,
                                    Ast_406.Asttypes.txt: "classify",
                                  }),
                              },
                            },
                          ],
                        ),
                    },
                  ]),
              },
              {
                pmty_attributes: [],
                pmty_loc: Ast_helper.default_loc.contents,
                pmty_desc:
                  Pmty_signature([
                    {
                      psig_loc: Ast_helper.default_loc.contents,
                      psig_desc:
                        [@implicit_arity]
                        Psig_type(
                          Ast_406.Asttypes.Recursive,
                          [
                            {
                              ptype_loc: Ast_helper.default_loc.contents,
                              ptype_attributes: [],
                              ptype_manifest: None,
                              ptype_private: Ast_406.Asttypes.Public,
                              ptype_kind: Ptype_abstract,
                              ptype_cstrs: [],
                              ptype_params: [],
                              ptype_name: {
                                Ast_406.Asttypes.loc:
                                  Ast_helper.default_loc.contents,
                                Ast_406.Asttypes.txt: "t",
                              },
                            },
                          ],
                        ),
                    },
                    {
                      psig_loc: Ast_helper.default_loc.contents,
                      psig_desc:
                        Psig_value({
                          pval_loc: Ast_helper.default_loc.contents,
                          pval_attributes: [],
                          pval_prim: [],
                          pval_type: {
                            ptyp_attributes: [],
                            ptyp_loc: Ast_helper.default_loc.contents,
                            ptyp_desc:
                              [@implicit_arity]
                              Ptyp_arrow(
                                Ast_406.Asttypes.Nolabel,
                                {
                                  ptyp_attributes: [],
                                  ptyp_loc: Ast_helper.default_loc.contents,
                                  ptyp_desc:
                                    [@implicit_arity]
                                    Ptyp_constr(
                                      {
                                        Ast_406.Asttypes.loc:
                                          Ast_helper.default_loc.contents,
                                        Ast_406.Asttypes.txt:
                                          Ast_406.Longident.Lident("float"),
                                      },
                                      [],
                                    ),
                                },
                                {
                                  ptyp_attributes: [],
                                  ptyp_loc: Ast_helper.default_loc.contents,
                                  ptyp_desc:
                                    [@implicit_arity]
                                    Ptyp_constr(
                                      {
                                        Ast_406.Asttypes.loc:
                                          Ast_helper.default_loc.contents,
                                        Ast_406.Asttypes.txt:
                                          Ast_406.Longident.Lident("t"),
                                      },
                                      [],
                                    ),
                                },
                              ),
                          },
                          pval_name: {
                            Ast_406.Asttypes.loc:
                              Ast_helper.default_loc.contents,
                            Ast_406.Asttypes.txt: "float",
                          },
                        }),
                    },
                    {
                      psig_loc: Ast_helper.default_loc.contents,
                      psig_desc:
                        Psig_value({
                          pval_loc: Ast_helper.default_loc.contents,
                          pval_attributes: [],
                          pval_prim: [],
                          pval_type: {
                            ptyp_attributes: [],
                            ptyp_loc: Ast_helper.default_loc.contents,
                            ptyp_desc:
                              [@implicit_arity]
                              Ptyp_arrow(
                                Ast_406.Asttypes.Nolabel,
                                {
                                  ptyp_attributes: [],
                                  ptyp_loc: Ast_helper.default_loc.contents,
                                  ptyp_desc:
                                    [@implicit_arity]
                                    Ptyp_constr(
                                      {
                                        Ast_406.Asttypes.loc:
                                          Ast_helper.default_loc.contents,
                                        Ast_406.Asttypes.txt:
                                          Ast_406.Longident.Lident("int"),
                                      },
                                      [],
                                    ),
                                },
                                {
                                  ptyp_attributes: [],
                                  ptyp_loc: Ast_helper.default_loc.contents,
                                  ptyp_desc:
                                    [@implicit_arity]
                                    Ptyp_constr(
                                      {
                                        Ast_406.Asttypes.loc:
                                          Ast_helper.default_loc.contents,
                                        Ast_406.Asttypes.txt:
                                          Ast_406.Longident.Lident("t"),
                                      },
                                      [],
                                    ),
                                },
                              ),
                          },
                          pval_name: {
                            Ast_406.Asttypes.loc:
                              Ast_helper.default_loc.contents,
                            Ast_406.Asttypes.txt: "int",
                          },
                        }),
                    },
                    {
                      psig_loc: Ast_helper.default_loc.contents,
                      psig_desc:
                        [@implicit_arity]
                        Psig_type(
                          Ast_406.Asttypes.Recursive,
                          [
                            {
                              ptype_loc: Ast_helper.default_loc.contents,
                              ptype_attributes: [],
                              ptype_manifest: None,
                              ptype_private: Ast_406.Asttypes.Public,
                              ptype_kind:
                                Ptype_variant([
                                  {
                                    pcd_attributes: [],
                                    pcd_loc: Ast_helper.default_loc.contents,
                                    pcd_res: None,
                                    pcd_args:
                                      Pcstr_tuple([
                                        {
                                          ptyp_attributes: [],
                                          ptyp_loc:
                                            Ast_helper.default_loc.contents,
                                          ptyp_desc:
                                            [@implicit_arity]
                                            Ptyp_constr(
                                              {
                                                Ast_406.Asttypes.loc:
                                                  Ast_helper.default_loc.
                                                    contents,
                                                Ast_406.Asttypes.txt:
                                                  Ast_406.Longident.Lident(
                                                    "int",
                                                  ),
                                              },
                                              [],
                                            ),
                                        },
                                      ]),
                                    pcd_name: {
                                      Ast_406.Asttypes.loc:
                                        Ast_helper.default_loc.contents,
                                      Ast_406.Asttypes.txt: "Int",
                                    },
                                  },
                                  {
                                    pcd_attributes: [],
                                    pcd_loc: Ast_helper.default_loc.contents,
                                    pcd_res: None,
                                    pcd_args:
                                      Pcstr_tuple([
                                        {
                                          ptyp_attributes: [],
                                          ptyp_loc:
                                            Ast_helper.default_loc.contents,
                                          ptyp_desc:
                                            [@implicit_arity]
                                            Ptyp_constr(
                                              {
                                                Ast_406.Asttypes.loc:
                                                  Ast_helper.default_loc.
                                                    contents,
                                                Ast_406.Asttypes.txt:
                                                  Ast_406.Longident.Lident(
                                                    "float",
                                                  ),
                                              },
                                              [],
                                            ),
                                        },
                                      ]),
                                    pcd_name: {
                                      Ast_406.Asttypes.loc:
                                        Ast_helper.default_loc.contents,
                                      Ast_406.Asttypes.txt: "Float",
                                    },
                                  },
                                ]),
                              ptype_cstrs: [],
                              ptype_params: [],
                              ptype_name: {
                                Ast_406.Asttypes.loc:
                                  Ast_helper.default_loc.contents,
                                Ast_406.Asttypes.txt: "case",
                              },
                            },
                          ],
                        ),
                    },
                    {
                      psig_loc: Ast_helper.default_loc.contents,
                      psig_desc:
                        Psig_value({
                          pval_loc: Ast_helper.default_loc.contents,
                          pval_attributes: [],
                          pval_prim: [],
                          pval_type: {
                            ptyp_attributes: [],
                            ptyp_loc: Ast_helper.default_loc.contents,
                            ptyp_desc:
                              [@implicit_arity]
                              Ptyp_arrow(
                                Ast_406.Asttypes.Nolabel,
                                {
                                  ptyp_attributes: [],
                                  ptyp_loc: Ast_helper.default_loc.contents,
                                  ptyp_desc:
                                    [@implicit_arity]
                                    Ptyp_constr(
                                      {
                                        Ast_406.Asttypes.loc:
                                          Ast_helper.default_loc.contents,
                                        Ast_406.Asttypes.txt:
                                          Ast_406.Longident.Lident("t"),
                                      },
                                      [],
                                    ),
                                },
                                {
                                  ptyp_attributes: [],
                                  ptyp_loc: Ast_helper.default_loc.contents,
                                  ptyp_desc:
                                    [@implicit_arity]
                                    Ptyp_constr(
                                      {
                                        Ast_406.Asttypes.loc:
                                          Ast_helper.default_loc.contents,
                                        Ast_406.Asttypes.txt:
                                          Ast_406.Longident.Lident("case"),
                                      },
                                      [],
                                    ),
                                },
                              ),
                          },
                          pval_name: {
                            Ast_406.Asttypes.loc:
                              Ast_helper.default_loc.contents,
                            Ast_406.Asttypes.txt: "classify",
                          },
                        }),
                    },
                  ]),
              },
            ),
        },
        pmb_name: {
          Ast_406.Asttypes.loc: Ast_helper.default_loc.contents,
          Ast_406.Asttypes.txt: "Number",
        },
      }),
  },
];

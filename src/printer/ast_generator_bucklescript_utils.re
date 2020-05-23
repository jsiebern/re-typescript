open Migrate_parsetree;
open Ast_406;
open Parsetree;
open Ast_helper;
open Ast_generator_utils;
open Ast_convenience_406;
open Utils;

let number_unboxed = [%str
  module Number: {
    type t;
    let float: float => t;
    let int: int => t;
    type case =
      | Int(int)
      | Float(float);
    let classify: t => case;
  } = {
    [@unboxed]
    type t =
      | Number('a): t;
    let float = (v: float) => Number(v);
    let int = (v: int) => Number(v);
    type case =
      | Int(int)
      | Float(float);
    let isInteger = v =>
      Js.typeof(v) === "number"
      && Js.Float.isFinite(v)
      && Js.Math.floor_float(v) === v;
    let classify = (Number(v): t) =>
      isInteger(Obj.magic(v))
        ? Int(Obj.magic(v): int) : Float(Obj.magic(v): float);
  }
];

let bs_as_attribute: string => attribute =
  name => (
    Location.mknoloc("bs.as"),
    PStr([Str.eval(Ast_convenience_406.str(name))]),
  );
let generate_nullable_of = wrap_type => {
  generate_base_type(~inner=[wrap_type], "Js.Nullable.t");
};

let generate_any = () => [%str
  [@unboxed]
  type any =
    | Any('a): any
];

let generate_bs_inline_simple =
    (~module_name, ~t, members: list((string, 'a)), conv: 'a => expression) => {
  let make_str = ((name, value)) => {
    let name_p = Pat.var(Location.mknoloc(name));
    [%stri
      [@bs.inline]
      let [%p name_p] = [%e conv(value)]
    ];
  };
  let make_stri = ((name, value)) => {
    Sig.value(
      Val.mk(
        ~attrs=[
          (Location.mknoloc("bs.inline"), PStr([Str.eval(conv(value))])),
        ],
        Location.mknoloc(name),
        [%type: t],
      ),
    );
  };
  let str_members = members |> CCList.map(make_str);
  let stri_members = members |> CCList.map(make_stri);

  Str.module_(
    Mb.mk(
      Location.mknoloc(module_name),
      Mod.constraint_(
        Mod.mk(
          Pmod_structure(
            CCList.concat([[%str type t = [%t t]], str_members]),
          ),
        ),
        Mty.signature(
          CCList.concat([[%sig: type t = pri [%t t]], stri_members]),
        ),
      ),
    ),
  );
};

let generate_bs_inline_str = (~module_name, members: list((string, string))) => {
  generate_bs_inline_simple(
    ~module_name,
    ~t=generate_base_type("string"),
    members,
    str,
  );
};

let generate_bs_inline_int = (~module_name, members: list((string, int))) => {
  generate_bs_inline_simple(
    ~module_name,
    ~t=generate_base_type("int"),
    members,
    int,
  );
};

let generate_bs_unboxed = (~module_name, values: Tree_types.ts_mixed_literal) => {
  let members =
    CCList.concat([
      values.strings
      |> CCList.map(v => Tree_utils.Ident.(v |> ident, str(v |> value))),
      values.numbers
      |> CCList.map(v => Tree_utils.Ident.(v |> of_int |> ident, int(v))),
      values.bools
      |> CCList.uniq(~eq=(a, b) => a == b)
      |> CCList.map(v =>
           (
             (v ? "true" : "false") |> String_utils.to_valid_ident,
             constr(v ? "true" : "false", []),
           )
         ),
    ]);

  let str_members =
    members
    |> CCList.map(((name, value)) => {
         [%stri let [%p Pat.var(Location.mknoloc(name))] = Any([%e value])]
       });
  let stri_members =
    members
    |> CCList.map(((name, _)) => {
         Sig.value(Val.mk(Location.mknoloc(name), [%type: t]))
       });

  Str.module_(
    Mb.mk(
      Location.mknoloc(module_name),
      Mod.constraint_(
        Mod.mk(
          Pmod_structure(
            CCList.concat([
              [%str
                [@unboxed]
                type t =
                  | Any('a): t
              ],
              str_members,
            ]),
          ),
        ),
        Mty.signature(CCList.concat([[%sig: type t], stri_members])),
      ),
    ),
  );
};

let generate_union_unboxed =
    (~module_name, members: list((string, core_type))) => {
  let str_members =
    members
    |> CCList.map(((name, t)) => {
         [%stri
           let [%p Pat.var(Location.mknoloc(name))] = (v: [%t t]) => Any(v)
         ]
       });
  let stri_members =
    members
    |> CCList.map(((name, t)) => {
         Sig.value(Val.mk(Location.mknoloc(name), [%type: [%t t] => t]))
       });

  Str.module_(
    Mb.mk(
      Location.mknoloc(module_name),
      Mod.constraint_(
        Mod.mk(
          Pmod_structure(
            CCList.concat([
              [%str
                [@unboxed]
                type t =
                  | Any('a): t
              ],
              str_members,
            ]),
          ),
        ),
        Mty.signature(CCList.concat([[%sig: type t], stri_members])),
      ),
    ),
  );
};

let rec generate_arrow_for_params =
        (
          ~has_opt=false,
          types: list((option(string), bool, core_type)),
          final: core_type,
        ) => {
  switch (types) {
  | [] =>
    has_opt
      ? generate_arrow_for_params(
          ~has_opt=false,
          [(None, false, generate_base_type("unit"))],
          final,
        )
      : final
  | [(label, optional, type_), ...rest] =>
    Typ.arrow(
      switch (label, optional) {
      | (None, _) => Nolabel
      | (Some(l), false) => Labelled(l)
      | (Some(l), true) => Optional(l)
      },
      optional ? generate_base_type(~inner=[type_], "option") : type_,
      generate_arrow_for_params(
        ~has_opt=has_opt ? has_opt : optional,
        rest,
        final,
      ),
    )
  };
};
let generate_arrow =
    (~params: list((option(string), bool, core_type)), ~return: core_type) => {
  switch (params) {
  | [] => generate_arrow_for_params(~has_opt=true, [], return)
  | lst => generate_arrow_for_params(lst, return)
  };
};

let generate_type = (~args, ~td_kind, ~td_type, ~name) =>
  Type.mk(
    ~params=
      args
      |> CCList.map(
           fun
           | {Tree_types.tda_name, _} => (
               Typ.var(tda_name |> Tree_utils.Ident.value),
               Asttypes.Invariant,
             ),
         ),
    ~kind=td_kind,
    ~manifest=?td_type,
    Location.mknoloc(name),
  );

let generate_type_wrap =
  fun
  | [] => []
  | types => [Str.type_(Recursive, types)];

type generated_type_def = {
  td_kind: type_kind,
  td_type: option(core_type),
  td_append: option(structure),
  td_prepend: option(structure),
};

let map_td = (mapper, type_def) => {
  ...type_def,
  td_type: type_def.td_type |> CCOpt.map(mapper),
};

let get_td_for_list = (~mapper=?, type_defs) => {
  td_kind: Ptype_abstract,
  td_type:
    mapper
    |> CCOpt.map(mapper =>
         mapper(type_defs |> CCList.filter_map(({td_type, _}) => td_type))
       ),
  td_prepend:
    type_defs
    |> CCList.filter_map(({td_prepend, _}) => td_prepend)
    |> CCList.concat
    |> list_to_opt,
  td_append:
    type_defs
    |> CCList.filter_map(({td_append, _}) => td_append)
    |> CCList.concat
    |> list_to_opt,
};

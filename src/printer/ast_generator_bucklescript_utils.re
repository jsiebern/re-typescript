open Migrate_parsetree;
open Ast_406;
open Parsetree;
open Ast_helper;
open Ast_generator_utils;
open Ast_convenience_406;
open Decode_result;
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

let generate_bs_unboxed =
    (~module_name, members: list(Decode_result.value_type)) => {
  let members =
    members
    |> CCList.map(
         fun
         | U_String(s) => (s |> to_valid_ident |> fst, str(s))
         | U_Number(n) => (n |> to_int_variant_constructor |> fst, int(n))
         | U_Bool(b) => (
             b ? "true" : "false" |> to_valid_ident |> fst,
             constr(b ? "true" : "false", []),
           ),
       );
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

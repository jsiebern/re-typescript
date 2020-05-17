open Migrate_parsetree;
open Ast_406;
open Parsetree;
open Ast_helper;
open Ast_generator_utils;
open Ast_convenience_406;

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

let generate_bs_inline_str = (~module_name, members: list((string, string))) => {
  let make_str = ((name, value)) => {
    let name_p = Pat.var(Location.mknoloc(name));
    [%stri
      [@bs.inline]
      let [%p name_p] = [%e str(value)]
    ];
  };
  let make_stri = ((name, value)) => {
    Sig.value(
      Val.mk(
        ~attrs=[
          (Location.mknoloc("bs.inline"), PStr([Str.eval(str(value))])),
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
            CCList.concat([[%str type t = string], str_members]),
          ),
        ),
        Mty.signature(
          CCList.concat([[%sig: type t = pri string], stri_members]),
        ),
      ),
    ),
  );
};

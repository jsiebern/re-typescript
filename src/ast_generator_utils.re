open Migrate_parsetree;
open Ast_406;
open Parsetree;
open Ast_helper;

let loc = Location.none;

module Naming = {
  let to_valid_ident = ident =>
    (
      try(
        if (ident.[0] >= '0' && ident.[0] <= '9') {
          "_" ++ ident;
        } else {
          CCString.uncapitalize_ascii(
            // from gist of sgrove, source:
            // https://gist.github.com/sgrove/335bf1759d8d2f685dfea80d4e6afac7
            [
              "and",
              "as",
              "asr",
              "assert",
              "begin",
              "class",
              "constraint",
              "do",
              "done",
              "downto",
              "else",
              "end",
              "esfun",
              "exception",
              "external",
              "false",
              "for",
              "fun",
              "function",
              "functor",
              "if",
              "in",
              "include",
              "inherit",
              "initializer",
              "land",
              "lazy",
              "let",
              "lor",
              "lsl",
              "lsr",
              "lxor",
              "match",
              "method",
              "mod",
              "module",
              "mutable",
              "new",
              "nonrec",
              "object",
              "of",
              "open",
              "open!",
              "or",
              "pri",
              "private",
              "pub",
              "public",
              "rec",
              "sig",
              "struct",
              "switch",
              "then",
              "to",
              "true",
              "try",
              "type",
              "val",
              "virtual",
              "when",
              "while",
              "with",
            ]
            |> List.exists(reserved_word => ident == reserved_word)
              ? ident ++ "_" : ident,
          );
        }
      ) {
      | Invalid_argument(_) => "_"
      | e => raise(e)
      }
    )
    |> CCString.replace(~sub="$", ~by="_")
    |> CCString.replace(~sub=".", ~by="_");

  let to_valid_variant_constructor = (~forceUpperCase=false, ident) =>
    (forceUpperCase ? ident |> CCString.capitalize_ascii : ident)
    |> CCString.replace(~sub="$", ~by="_")
    |> CCString.replace(~sub=".", ~by="_");

  let to_int_variant_constructor = (i: int) => Printf.sprintf("_%#u", i);

  let moduleName = to_valid_variant_constructor(~forceUpperCase=true);
  let variantIdentifier = to_valid_variant_constructor(~forceUpperCase=true);
  let polyVariantIdentifier = to_valid_variant_constructor;
  let typeName = to_valid_ident;
  let propertyName = to_valid_ident;
  let subIdent = to_int_variant_constructor;

  let unwrap: type t. Ast.Identifier.t(t) => string =
    i =>
      switch (i) {
      | Module(str) => str
      | VariantIdentifier(str) => str
      | TypeName(str) => str
      | PropertyName(str) => str
      | SubName(str) => str
      | TypeParameter(str) => str
      | SubIdent(1) => "1st"
      | SubIdent(2) => "2nd"
      | SubIdent(3) => "3rd"
      | SubIdent(num) => string_of_int(num) ++ "th"
      };

  let fromIdentifier: type t. Ast.Identifier.t(t) => string =
    i =>
      switch (i) {
      | Module(str) => moduleName(str)
      | VariantIdentifier(str) => variantIdentifier(str)
      | TypeName(str) => typeName(str)
      | PropertyName(str) => typeName(str)
      | SubName(str) => typeName(str)
      | TypeParameter(str) => str
      | SubIdent(num) => subIdent(num)
      };

  let full_identifier_of_path = (p: Ast.Identifier.path) =>
    p
    |> CCArray.mapi((i, ident) =>
         switch (ident, CCArray.get_safe(p, i + 1)) {
         | _ when i === 0 && CCArray.length(p) === 1 => fromIdentifier(ident)
         | (_, None) => unwrap(ident)
         | (Ast.Identifier.Module(str), Some(_)) =>
           Printf.sprintf("%s.", str)
         | (_, Some(_)) => Printf.sprintf("%s_", unwrap(ident))
         }
       )
    |> CCArray.to_string(~sep="", a => a);
};

let make_bs_as_attribute: string => attribute =
  name => (
    Location.mknoloc("bs.as"),
    PStr([Str.eval(Ast_convenience_406.str(name))]),
  );

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
    (~params=[], ~aliasName: string, ~aliasType: core_type)
    : Parsetree.structure_item => {
  pstr_desc:
    Parsetree.Pstr_type(
      Asttypes.Recursive,
      [
        {
          ptype_name: {
            txt: aliasName,
            loc,
          },
          ptype_params:
            params
            |> CCList.map(param => (Typ.var(param), Asttypes.Invariant)),
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
    (~params=[], ~aliasName: string, ~kind: type_kind)
    : Parsetree.structure_item => {
  pstr_desc:
    Parsetree.Pstr_type(
      Asttypes.Recursive,
      [
        {
          ptype_name: {
            txt: aliasName,
            loc,
          },
          ptype_params:
            params
            |> CCList.map(param => (Typ.var(param), Asttypes.Invariant)),
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

let make_optional_of = wrap_type => {
  make_type_constraint(~inner=[wrap_type], "option");
};

let make_nullable_of = wrap_type => {
  make_type_constraint(~inner=[wrap_type], "Js.Null.t");
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
let make_polymorphic = (names: list(Ast.VariantConstructor.t)) => {
  Typ.mk(
    Ptyp_variant(
      names
      |> CCList.map(
           ({Ast.VariantConstructor.name: VariantIdentifier(name), _}) =>
           Naming.to_valid_variant_constructor(name)
         )
      |> CCList.map(name => Rtag(Location.mknoloc(name), [], true, [])),
      Closed,
      None,
    ),
  );
};

let make_tuple_of = (types: array(core_type)) =>
  Typ.tuple(types |> CCArray.to_list);

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

let make_record_kind =
    (fields: array((string, core_type, list(attribute)))) =>
  Ptype_record(
    fields
    |> CCArray.map(((name, type_, attrs)) =>
         Type.field(~attrs, Location.mknoloc(name), type_)
       )
    |> CCArray.to_list,
  );

module Unboxed = {
  let t = params =>
    Ast_helper.Typ.constr(
      Location.mknoloc(Longident.Lident("t")),
      params |> CCList.map(param => Typ.var(param)),
    );
  let make_unboxed_helper = () => (
    [%stri
      [@unboxed]
      type t =
        | Any('a): t
    ],
    [%sigi: type t],
  );
  let unboxed_func = (~params=[], name, ty: core_type) => (
    [%stri
      let [%p Pat.var(Location.mknoloc(name))] = (v: [%t ty]) => Any(v)
    ],
    Sig.value(
      Val.mk(Location.mknoloc(name), [%type: [%t ty] => [%t t(params)]]),
    ),
  );
};

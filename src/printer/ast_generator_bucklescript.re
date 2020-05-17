open Decode_result;
open Decode_config;
open Migrate_parsetree;
open Ast_406;
open Parsetree;
open Ast_helper;
open Ast_generator_utils;

let bs_as_attribute: string => attribute =
  name => (
    Location.mknoloc("bs.as"),
    PStr([Str.eval(Ast_convenience_406.str(name))]),
  );
let generate_nullable_of = wrap_type => {
  generate_base_type(~inner=[wrap_type], "Js.Nullable.t");
};

exception BS_Decode_Error(string, string);

type gen_config = {
  mutable has_any: bool,
  mutable has_unboxed_number: bool,
};
let gen_config = {has_any: false, has_unboxed_number: false};

let rec generate_type_def = (~ctx: config, type_def) =>
  switch (type_def) {
  | Base(base_type) => (
      Ptype_abstract,
      Some(
        switch (base_type) {
        | String => generate_base_type("string")
        | Number =>
          switch (ctx.number_mode) {
          | Float => generate_base_type("float")
          | Int => generate_base_type("int")
          | Unboxed =>
            gen_config.has_unboxed_number = true;
            generate_base_type("Number.t");
          }
        | Boolean => generate_base_type("bool")
        | Void => generate_base_type("unit")
        | Ref(ref_) => generate_base_type(ref_ |> fst)
        | Any =>
          gen_config.has_any = true;
          generate_base_type("any");
        },
      ),
    )
  | Record(fields) => (
      generate_record_kind(
        fields
        |> List.map(
             fun
             | RecordField((name, orig_name), type_, _) => (
                 name,
                 switch (type_ |> generate_type_def(~ctx)) {
                 | (_, None) =>
                   raise(
                     BS_Decode_Error(
                       "Invalid record field type",
                       BatPervasives.dump(type_),
                     ),
                   )
                 | (_, Some(t)) => t
                 },
                 name != orig_name ? [bs_as_attribute(orig_name)] : [],
               )
             | d =>
               raise(
                 BS_Decode_Error(
                   "Record only accepts children of type RecordField",
                   BatPervasives.dump(d),
                 ),
               ),
           ),
      ),
      None,
    )
  | Array(inner) => (
      Ptype_abstract,
      switch (ctx.array_mode, generate_type_def(~ctx, inner)) {
      | (Array, (_, inner)) =>
        inner |> Tablecloth.Option.map(~f=generate_array_of)
      | (List, (_, inner)) =>
        inner |> Tablecloth.Option.map(~f=generate_list_of)
      },
    )
  | Optional(inner) => (
      Ptype_abstract,
      generate_type_def(~ctx, inner)
      |> snd
      |> Tablecloth.Option.map(~f=generate_option_of),
    )
  | Nullable(inner) => (
      Ptype_abstract,
      generate_type_def(~ctx, inner)
      |> snd
      |> Tablecloth.Option.map(~f=generate_nullable_of),
    )
  | VariantEnum(keys) => (
      generate_variant_kind(keys |> Tablecloth.List.map(~f=fst)),
      None,
    )
  | VariantString(keys) => (
      Ptype_abstract,
      Some(generate_poly_variant(keys |> Tablecloth.List.map(~f=fst))),
    )
  | Tuple(types) => (
      Ptype_abstract,
      Some(
        generate_tuple_of(
          types
          |> Tablecloth.List.map(~f=generate_type_def(~ctx))
          |> Tablecloth.List.filter_map(~f=snd),
        ),
      ),
    )
  | RecordField(_) =>
    raise(
      BS_Decode_Error(
        "RecordField is not valid outside of type Record",
        BatPervasives.dump(type_def),
      ),
    )
  | TypeDeclaration(_) =>
    raise(
      BS_Decode_Error(
        "TypeDeclaration is not valid outside of root",
        BatPervasives.dump(type_def),
      ),
    )
  | t =>
    raise(BS_Decode_Error("Not yet implemented", BatPervasives.dump(t)))
  };

let generate = (~ctx, type_defs) => {
  gen_config.has_any = false;
  gen_config.has_unboxed_number = false;
  let types =
    !(type_defs |> Tablecloth.List.is_empty)
      ? [
        Str.type_(
          Recursive,
          List.concat(
            type_defs
            |> List.map(type_def =>
                 switch (type_def) {
                 | TypeDeclaration((name, _), type_) =>
                   let (kind, manifest) = generate_type_def(~ctx, type_);
                   [Type.mk(~kind, ~manifest?, Location.mknoloc(name))];
                 | d =>
                   raise(
                     BS_Decode_Error(
                       "Invalid data structure in root",
                       BatPervasives.dump(d),
                     ),
                   )
                 }
               ),
          ),
        ),
      ]
      : [];
  Tablecloth.List.concat([
    gen_config.has_unboxed_number
      ? Ast_generator_bucklescript_number_unboxed.number_unboxed : [],
    gen_config.has_any ? generate_any() : [],
    types,
  ]);
};

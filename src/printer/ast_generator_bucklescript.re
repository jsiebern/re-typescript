module type Config = {let config: Decode_config.bucklescript_config;};

module Make = (Config: Config) : Ast_generator.T => {
  open Config;
  open Decode_result;
  open Decode_config;
  open Migrate_parsetree;
  open Ast_406;
  open Parsetree;
  open Ast_helper;
  open Ast_generator_utils;
  open Ast_generator_bucklescript_utils;
  open Utils;

  exception BS_Decode_Error(string);

  type gen_config = {
    mutable has_any: bool,
    mutable has_unboxed_number: bool,
    mutable inject: list(structure_item),
  };
  let gen_config = {has_any: false, has_unboxed_number: false, inject: []};

  let rec generate_type_def = (~ctx: config, ~parent_name, type_def) =>
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
          | Arg(argName) => Typ.var(argName)
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
                   switch (type_ |> generate_type_def(~ctx, ~parent_name)) {
                   | (_, None) =>
                     Console.error(type_);
                     raise(BS_Decode_Error("Invalid record field type"));
                   | (_, Some(t)) => t
                   },
                   name != orig_name ? [bs_as_attribute(orig_name)] : [],
                 )
               | d =>
                 raise(
                   {
                     Console.error(d);
                     BS_Decode_Error(
                       "Record only accepts children of type RecordField",
                     );
                   },
                 ),
             ),
        ),
        None,
      )
    | Array(inner) => (
        Ptype_abstract,
        switch (ctx.array_mode, generate_type_def(~ctx, ~parent_name, inner)) {
        | (Array, (_, inner)) => inner |> CCOpt.map(generate_array_of)
        | (List, (_, inner)) => inner |> CCOpt.map(generate_list_of)
        },
      )
    | Optional(inner) => (
        Ptype_abstract,
        generate_type_def(~ctx, ~parent_name, inner)
        |> snd
        |> CCOpt.map(generate_option_of),
      )
    | Nullable(inner) => (
        Ptype_abstract,
        generate_type_def(~ctx, ~parent_name, inner)
        |> snd
        |> CCOpt.map(generate_nullable_of),
      )
    | VariantEnum(keys) => (
        generate_variant_kind(keys |> CCListLabels.map(~f=fst)),
        None,
      )
    | VariantString(keys) =>
      switch (config.string_variant_mode) {
      | `PolyVariant => (
          Ptype_abstract,
          Some(
            generate_poly_variant(
              keys
              |> CCListLabels.map(~f=to_valid_variant_constructor)
              |> CCListLabels.map(~f=fst),
            ),
          ),
        )
      | `Variant => (
          generate_variant_kind(
            keys
            |> CCListLabels.map(~f=to_valid_variant_constructor)
            |> CCListLabels.map(~f=fst),
          ),
          None,
        )
      | `BsInline =>
        let module_name = to_valid_module_name(parent_name) |> fst;
        gen_config.inject = [
          generate_bs_inline_str(
            ~module_name,
            keys |> CCList.map(to_valid_ident),
          ),
          ...gen_config.inject,
        ];
        (
          Ptype_abstract,
          Some(generate_base_type(Printf.sprintf("%s.t", module_name))),
        );
      }
    | VariantMixed(values) =>
      switch (config.mixed_variant_mode) {
      | `BsUnboxed =>
        let module_name = to_valid_module_name(parent_name) |> fst;
        gen_config.inject = [
          generate_bs_unboxed(~module_name, values),
          ...gen_config.inject,
        ];
        (
          Ptype_abstract,
          Some(generate_base_type(Printf.sprintf("%s.t", module_name))),
        );
      }
    | VariantInt(keys) =>
      let keys = pat =>
        keys
        |> CCList.map(v =>
             switch (pat) {
             | None =>
               switch (config.number_variant_mode) {
               | `BsInline(_) => (
                   to_valid_ident(v |> string_of_int) |> fst,
                   v,
                 )
               | `Variant(_)
               | `PolyVariant(_) => to_int_variant_constructor(v)
               }
             | Some(pat) => (
                 Printf.sprintf("%s%d%s", pat.prefix, v, pat.suffix),
                 v,
               )
             }
           );
      switch (config.number_variant_mode) {
      | `PolyVariant(pat) => (
          Ptype_abstract,
          Some(generate_poly_variant(keys(pat) |> CCList.map(fst))),
        )
      | `Variant(pat) => (
          generate_variant_kind(keys(pat) |> CCList.map(fst)),
          None,
        )
      | `BsInline(pat) =>
        let module_name = to_valid_module_name(parent_name) |> fst;
        gen_config.inject = [
          generate_bs_inline_int(~module_name, keys(pat)),
          ...gen_config.inject,
        ];
        (
          Ptype_abstract,
          Some(generate_base_type(Printf.sprintf("%s.t", module_name))),
        );
      };

    | Tuple(types) => (
        Ptype_abstract,
        Some(
          generate_tuple_of(
            types
            |> CCListLabels.map(~f=generate_type_def(~ctx, ~parent_name))
            |> CCListLabels.filter_map(~f=snd),
          ),
        ),
      )
    | RecordField(_) =>
      raise(
        {
          Console.error(type_def);
          BS_Decode_Error("RecordField is not valid outside of type Record");
        },
      )
    | TypeDeclaration(_) =>
      raise(
        {
          Console.error(type_def);
          BS_Decode_Error("TypeDeclaration is not valid outside of root");
        },
      )
    | t =>
      raise(
        {
          Console.error(t);
          BS_Decode_Error("Not yet implemented");
        },
      )
    };

  let generate = (~ctx, type_defs) => {
    gen_config.has_any = false;
    gen_config.has_unboxed_number = false;
    gen_config.inject = [];
    let types =
      !(type_defs |> CCListLabels.is_empty)
        ? [
          Str.type_(
            Recursive,
            List.concat(
              type_defs
              |> List.map(type_def => {
                   switch (type_def) {
                   | TypeDeclaration((name, _), type_, args) =>
                     let (kind, manifest) =
                       generate_type_def(~ctx, ~parent_name=name, type_);
                     [
                       Type.mk(
                         ~params=
                           args
                           |> CCList.map(
                                fun
                                | {name} => (
                                    Typ.var(name),
                                    Asttypes.Invariant,
                                  ),
                              ),
                         ~kind,
                         ~manifest?,
                         Location.mknoloc(name),
                       ),
                     ];
                   | d =>
                     Console.error(d);
                     raise(
                       BS_Decode_Error("Invalid data structure in root"),
                     );
                   }
                 }),
            ),
          ),
        ]
        : [];
    CCListLabels.concat([
      gen_config.has_unboxed_number ? number_unboxed : [],
      gen_config.has_any ? generate_any() : [],
      gen_config.inject,
      types,
    ]);
  };
};

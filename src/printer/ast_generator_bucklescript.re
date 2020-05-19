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

  let rec generate_type_def =
          (~ctx: config, ~parent_name, type_def): generated_type_def =>
    switch (type_def) {
    | Base(base_type) => {
        td_kind: Ptype_abstract,
        td_type:
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
            | Ref(ref_, applied_types) =>
              CCList.(
                generate_base_type(
                  ~inner=
                    applied_types
                    >|= generate_type_def(~ctx, ~parent_name)
                    |> CCList.filter_map(({td_type, _}) => td_type),
                  ref_ |> fst,
                )
              )
            | Arg(argName) => Typ.var(argName)
            | Any =>
              gen_config.has_any = true;
              generate_base_type("any");
            },
          ),
        td_prepend: None,
        td_append: None,
      }
    | Record(fields) =>
      let (field_struct, field_types) =
        fields
        |> CCList.map(
             fun
             | RecordField(a, b, c) => Some((a, b, c))
             | _ => None,
           )
        |> CCList.keep_some
        |> CCList.map((((name, orig_name), type_, _)) => {
             let parent_name = Printf.sprintf("%s_%s", parent_name, name);
             let {td_type, td_prepend, td_append, _} =
               type_ |> generate_type_def(~ctx, ~parent_name);
             (
               (td_prepend, td_append),
               (
                 name,
                 td_type,
                 name != orig_name ? [bs_as_attribute(orig_name)] : [],
               ),
             );
           })
        |> CCList.split;
      let (fields_prepend, fields_append) = field_struct |> CCList.split;

      {
        td_kind:
          generate_record_kind(
            field_types
            |> CCList.filter_map(((n, t, a)) =>
                 t |> CCOpt.map(t => (n, t, a))
               ),
          ),
        td_type: None,
        td_prepend:
          list_to_opt(fields_prepend |> CCList.keep_some |> CCList.concat),
        td_append:
          list_to_opt(fields_append |> CCList.keep_some |> CCList.concat),
      };
    | Array(inner) =>
      generate_type_def(~ctx, ~parent_name, inner)
      |> map_td(
           switch (ctx.array_mode) {
           | Array => generate_array_of
           | List => generate_list_of
           },
         )
    | Optional(inner) =>
      generate_type_def(~ctx, ~parent_name, inner)
      |> map_td(generate_option_of)
    | Nullable(inner) =>
      generate_type_def(~ctx, ~parent_name, inner)
      |> map_td(generate_nullable_of)
    | VariantEnum(keys) => {
        td_kind: generate_variant_kind(keys |> CCListLabels.map(~f=fst)),
        td_type: None,
        td_prepend: None,
        td_append: None,
      }
    | VariantString(keys) =>
      switch (config.string_variant_mode) {
      | `PolyVariant => {
          td_kind: Ptype_abstract,
          td_type:
            Some(
              generate_poly_variant(
                keys
                |> CCListLabels.map(~f=to_valid_variant_constructor)
                |> CCListLabels.map(~f=fst),
              ),
            ),
          td_prepend: None,
          td_append: None,
        }
      | `Variant => {
          td_kind:
            generate_variant_kind(
              keys
              |> CCListLabels.map(~f=to_valid_variant_constructor)
              |> CCListLabels.map(~f=fst),
            ),
          td_type: None,
          td_prepend: None,
          td_append: None,
        }
      | `BsInline =>
        let module_name = to_valid_module_name(parent_name) |> fst;
        {
          td_kind: Ptype_abstract,
          td_type:
            Some(generate_base_type(Printf.sprintf("%s.t", module_name))),
          td_prepend:
            Some([
              generate_bs_inline_str(
                ~module_name,
                keys |> CCList.map(to_valid_ident),
              ),
            ]),
          td_append: None,
        };
      }
    | VariantMixed(values) =>
      switch (config.mixed_variant_mode) {
      | `BsUnboxed =>
        let module_name = to_valid_module_name(parent_name) |> fst;
        {
          td_kind: Ptype_abstract,
          td_type:
            Some(generate_base_type(Printf.sprintf("%s.t", module_name))),
          td_prepend: Some([generate_bs_unboxed(~module_name, values)]),
          td_append: None,
        };
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
      | `PolyVariant(pat) => {
          td_kind: Ptype_abstract,
          td_type:
            Some(generate_poly_variant(keys(pat) |> CCList.map(fst))),
          td_prepend: None,
          td_append: None,
        }
      | `Variant(pat) => {
          td_kind: generate_variant_kind(keys(pat) |> CCList.map(fst)),
          td_type: None,
          td_prepend: None,
          td_append: None,
        }
      | `BsInline(pat) =>
        let module_name = to_valid_module_name(parent_name) |> fst;
        {
          td_kind: Ptype_abstract,
          td_type:
            Some(generate_base_type(Printf.sprintf("%s.t", module_name))),
          td_prepend:
            Some([generate_bs_inline_int(~module_name, keys(pat))]),
          td_append: None,
        };
      };
    | Tuple(types) =>
      let type_defs =
        types |> CCList.map(generate_type_def(~ctx, ~parent_name));
      get_td_for_list(~mapper=generate_tuple_of, type_defs);

    | Union(types) =>
      let module_name = to_valid_module_name(parent_name) |> fst;
      let type_defs =
        types
        |> CCList.map(({um_name, um_type, _}) =>
             (um_name, generate_type_def(~ctx, ~parent_name, um_type))
           );
      let (type_names, type_defs) = type_defs |> CCList.split;
      let temp_td = get_td_for_list(type_defs);

      {
        td_kind: Ptype_abstract,
        td_type:
          Some(generate_base_type(Printf.sprintf("%s.t", module_name))),
        td_prepend:
          Some(
            CCList.concat([
              temp_td.td_prepend |> CCOpt.get_or(~default=[]),
              [
                generate_union_unboxed(
                  ~module_name,
                  CCList.combine(type_names, type_defs)
                  |> CCList.filter_map(((tn, td)) =>
                       td.td_type |> CCOpt.map(t => (tn, t))
                     ),
                ),
              ],
            ]),
          ),
        td_append: None,
      };
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
    };

  let generate = (~ctx, type_defs) => {
    gen_config.has_any = false;
    gen_config.has_unboxed_number = false;
    gen_config.inject = [];

    let types =
      type_defs
      |> CCList.filter_map(
           fun
           | TypeDeclaration(a, b, c) => Some((a, b, c))
           | _ => None,
         )
      |> CCList.fold_left(
           ((buffer, carry), ((name, _), type_, args)) => {
             switch (generate_type_def(~ctx, ~parent_name=name, type_)) {
             | {td_prepend: Some(prepend), td_append: None, td_kind, td_type} => (
                 [],
                 carry
                 @ generate_type_wrap(buffer)
                 @ prepend
                 @ generate_type_wrap([
                     generate_type(~name, ~args, ~td_kind, ~td_type),
                   ]),
               )
             | {td_prepend: None, td_append: Some(append), td_kind, td_type} => (
                 [],
                 carry
                 @ generate_type_wrap(
                     buffer
                     @ [generate_type(~name, ~args, ~td_kind, ~td_type)],
                   )
                 @ append,
               )
             | {
                 td_prepend: Some(prepend),
                 td_append: Some(append),
                 td_kind,
                 td_type,
               } => (
                 [],
                 carry
                 @ prepend
                 @ generate_type_wrap(
                     buffer
                     @ [generate_type(~name, ~args, ~td_kind, ~td_type)],
                   )
                 @ append,
               )
             | {td_prepend: None, td_append: None, td_kind, td_type} => (
                 buffer @ [generate_type(~name, ~args, ~td_kind, ~td_type)],
                 carry,
               )
             }
           },
           ([], []),
         );
    let types = snd(types) @ generate_type_wrap(fst(types));

    CCListLabels.concat([
      gen_config.has_unboxed_number ? number_unboxed : [],
      gen_config.has_any ? generate_any() : [],
      gen_config.inject,
      types,
    ]);
  };
};

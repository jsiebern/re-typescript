module type Config = {
  let config: Re_typescript_config.Config.bucklescript_config;
};

module Make = (Config: Config) : Ast_generator.T => {
  open Config;
  open Tree_types;
  open Migrate_parsetree;
  open Ast_406;
  open Parsetree;
  open Ast_helper;
  open Ast_generator_utils;
  open Ast_generator_bucklescript_utils;
  open Utils;
  open Tree_utils;
  open Tree_types;

  exception BS_Decode_Error(string);

  type gen_config = {
    mutable has_any: bool,
    mutable has_unboxed_number: bool,
    mutable has_extended_record: bool,
    mutable inject: list(structure_item),
  };
  let gen_config = {
    has_any: false,
    has_extended_record: true,
    has_unboxed_number: false,
    inject: [],
  };

  let rec generate_type_def =
          (
            ~ctx: Re_typescript_config.Config.config,
            ~path: Path.t,
            type_: ts_type,
          )
          : generated_type_def =>
    switch (type_) {
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
            | Null =>
              generate_base_type(
                ~inner=[generate_base_type("unit")],
                "Js.null",
              )
            | Undefined =>
              generate_base_type(
                ~inner=[generate_base_type("unit")],
                "option",
              )
            | Any =>
              gen_config.has_any = true;
              generate_base_type("any");
            },
          ),
        td_prepend: None,
        td_append: None,
      }
    | Reference({tr_path_resolved, tr_parameters, tr_path}) =>
      switch (tr_path_resolved) {
      | None =>
        Console.warn(
          Printf.sprintf(
            "Warning: Could not resolve reference to %s. Applied 'any' instead.",
            tr_path |> Path.unscoped_to_string,
          ),
        );
        generate_type_def(~ctx, ~path, Base(Any));
      | Some(path_resolved) => {
          td_kind: Ptype_abstract,
          td_type:
            Some(
              CCList.(
                generate_base_type(
                  ~inner=
                    tr_parameters
                    >|= generate_type_def(~ctx, ~path)
                    |> CCList.filter_map(({td_type, _}: generated_type_def) =>
                         td_type
                       ),
                  path_resolved |> Path.to_typename,
                )
              ),
            ),
          td_prepend: None,
          td_append: None,
        }
      }

    | Arg(argName) => {
        td_kind: Ptype_abstract,
        td_type: Some(Typ.var(argName |> Ident.value)),
        td_prepend: None,
        td_append: None,
      }
    | Interface(fields, extended) =>
      if (extended) {
        gen_config.has_extended_record = true;
      };
      let (field_struct, field_types) =
        fields
        |> CCList.map(({f_name, f_type, _}) => {
             let path = path |> Path.add_sub(f_name |> Ident.ident);
             let {td_type, td_prepend, td_append, _} =
               generate_type_def(~ctx, ~path, f_type);
             (
               (td_prepend, td_append),
               (
                 f_name |> Ident.ident,
                 td_type,
                 f_name |> Ident.ident != (f_name |> Ident.value)
                   ? [bs_as_attribute(f_name |> Ident.value)] : [],
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
      generate_type_def(~ctx, ~path, inner)
      |> map_td(
           switch (ctx.array_mode) {
           | Array => generate_array_of
           | List => generate_list_of
           },
         )
    | Optional(inner) =>
      generate_type_def(~ctx, ~path, inner) |> map_td(generate_option_of)
    | Nullable(inner) =>
      generate_type_def(~ctx, ~path, inner) |> map_td(generate_nullable_of)
    | Enum(keys) => {
        td_kind: generate_variant_kind(keys |> CCList.map(Ident.variant)),
        td_type: None,
        td_prepend: None,
        td_append: None,
      }
    | StringLiteral(keys) =>
      switch (config.string_variant_mode) {
      | `PolyVariant => {
          td_kind: Ptype_abstract,
          td_type:
            Some(generate_poly_variant(keys |> CCList.map(Ident.variant))),
          td_prepend: None,
          td_append: None,
        }
      | `Variant => {
          td_kind: generate_variant_kind(keys |> CCList.map(Ident.variant)),
          td_type: None,
          td_prepend: None,
          td_append: None,
        }
      | `BsInline =>
        let module_name = path |> Path.to_ident |> Ident.module_;
        {
          td_kind: Ptype_abstract,
          td_type:
            Some(generate_base_type(Printf.sprintf("%s.t", module_name))),
          td_prepend:
            Some([
              generate_bs_inline_str(
                ~module_name,
                keys |> CCList.map(v => Ident.(v |> ident, v |> value)),
              ),
            ]),
          td_append: None,
        };
      }
    | MixedLiteral(values) =>
      switch (config.mixed_variant_mode) {
      | `BsUnboxed =>
        let module_name = path |> Path.to_ident |> Ident.module_;
        {
          td_kind: Ptype_abstract,
          td_type:
            Some(generate_base_type(Printf.sprintf("%s.t", module_name))),
          td_prepend: Some([generate_bs_unboxed(~module_name, values)]),
          td_append: None,
        };
      | `Variant
      | `PolyVariant =>
        raise(
          Exceptions.Parser_unsupported(
            "Variant / PolyVariant are not yet supported for mixed literals",
            Re_typescript_base.Parse_info.zero,
          ),
        )
      }
    | Function({fu_params, fu_return}) =>
      let params =
        fu_params
        |> CCList.filter_map(({fp_name, fp_optional, fp_type}) => {
             let path = path |> Path.add_sub(fp_name |> Ident.ident);
             let gen = generate_type_def(~ctx, ~path, fp_type);
             gen.td_type
             |> CCOpt.map(type_ =>
                  (
                    (Some(fp_name |> Ident.ident), fp_optional, type_),
                    (
                      opt_to_list(gen.td_prepend),
                      opt_to_list(gen.td_append),
                    ),
                  )
                );
           });
      let return =
        generate_type_def(
          ~ctx,
          ~path={
            path |> Path.add_sub("return");
          },
          fu_return,
        );
      let (params, additional) = params |> CCList.split;
      let (td_prepend, td_append) = additional |> CCList.split;
      {
        td_kind: Ptype_abstract,
        td_type:
          Some(
            generate_arrow(~params, ~return=return.td_type |> CCOpt.get_exn),
          ),
        td_prepend:
          list_to_opt(
            (td_prepend |> CCList.concat) @ opt_to_list(return.td_prepend),
          ),
        td_append:
          list_to_opt(
            (td_append |> CCList.concat) @ opt_to_list(return.td_append),
          ),
      };
    | NumericLiteral(keys) =>
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
                 Printf.sprintf(
                   "%s%d%s",
                   pat.Re_typescript_config.Config.prefix,
                   v,
                   pat.suffix,
                 ),
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
        let module_name = path |> Path.to_ident |> Ident.module_;
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
      let type_defs = types |> CCList.map(generate_type_def(~ctx, ~path));
      get_td_for_list(~mapper=generate_tuple_of, type_defs);

    | Union(types) =>
      let module_name = path |> Path.to_ident |> Ident.module_;
      let type_defs =
        types
        |> CCList.map(({um_ident, um_type, _}) =>
             (um_ident, generate_type_def(~ctx, ~path, um_type))
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
                  |> CCList.filter_map(((tn, td: generated_type_def)) =>
                       td.td_type |> CCOpt.map(t => (tn, t))
                     ),
                ),
              ],
            ]),
          ),
        td_append: None,
      };
    | TypeDeclaration(_) =>
      raise(
        {
          Console.error(type_);
          BS_Decode_Error("TypeDeclaration is not valid outside of root");
        },
      )
    | Import(_) =>
      raise(
        {
          Console.error(type_);
          BS_Decode_Error("Import is not valid outside of root");
        },
      )
    | Module(_) =>
      raise(
        {
          Console.error(type_);
          BS_Decode_Error("Module is not valid outside of root");
        },
      )
    };

  let generate =
      (
        ~ctx,
        (
          type_order: list(Tree_utils.Path.t),
          type_defs: Hashtbl.t(Tree_utils.Path.t, Tree_types.ts_type),
        ),
      ) => {
    gen_config.has_any = false;
    gen_config.has_unboxed_number = false;
    gen_config.has_extended_record = false;
    gen_config.inject = [];

    let types =
      type_order
      |> CCList.filter_map(path =>
           Hashtbl.find_opt(type_defs, path) |> CCOpt.map(td => (path, td))
         )
      |> CCList.filter_map(
           fun
           | (path, TypeDeclaration(type_declaration)) =>
             Some((path, type_declaration))
           | _ => None,
         )
      |> CCList.fold_left(
           ((buffer, carry), (path, {td_name, td_parameters, td_type, _})) => {
             let name = path |> Path.to_ident |> Ident.type_;
             switch (generate_type_def(~ctx, ~path, td_type)) {
             | {td_prepend: Some(prepend), td_append: None, td_kind, td_type} => (
                 [],
                 carry
                 @ generate_type_wrap(buffer)
                 @ prepend
                 @ generate_type_wrap([
                     generate_type(
                       ~name,
                       ~parameters=td_parameters,
                       ~td_kind,
                       ~td_type,
                     ),
                   ]),
               )
             | {td_prepend: None, td_append: Some(append), td_kind, td_type} => (
                 [],
                 carry
                 @ generate_type_wrap(
                     buffer
                     @ [
                       generate_type(
                         ~name,
                         ~parameters=td_parameters,
                         ~td_kind,
                         ~td_type,
                       ),
                     ],
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
                     @ [
                       generate_type(
                         ~name,
                         ~parameters=td_parameters,
                         ~td_kind,
                         ~td_type,
                       ),
                     ],
                   )
                 @ append,
               )
             | {td_prepend: None, td_append: None, td_kind, td_type} => (
                 buffer
                 @ [
                   generate_type(
                     ~name,
                     ~parameters=td_parameters,
                     ~td_kind,
                     ~td_type,
                   ),
                 ],
                 carry,
               )
             };
           },
           ([], []),
         );
    let types = snd(types) @ generate_type_wrap(fst(types));

    let append_warning_suppression =
      ctx.suppress_warning_for_extended_records
      && gen_config.has_extended_record;

    CCList.concat([
      append_warning_suppression ? extended_record_warning : [],
      gen_config.has_unboxed_number ? number_unboxed : [],
      gen_config.has_any ? generate_any() : [],
      gen_config.inject,
      types,
    ]);
  };
};

open Re_typescript_base;
open Utils;
open Tree_types;
open Tree_utils;
open Tree_data;

type parser_runtime = {
  mutable config: Re_typescript_config.Config.config,
  mutable parser: string => CCResult.t(list(Ts.declaration), string),
  mutable resolver: (module Re_typescript_fs.Resolver.T),
  mutable current_file: Fp.t(Fp.absolute),
};
let runtime = {
  config: Re_typescript_config.default_config,
  parser: _ => Error("Undefined parser"),
  resolver: Re_typescript_fs.default_resolver,
  current_file: Fp.absoluteExn("/"),
};
let current_position = ref(Parse_info.zero);

/**
    Type / Interface / Enum definitions
 */
let rec parse__type_def =
        (~inline=false, ~path=([], []), type_def: Ts.declaration) => {
  current_position := position_of_declaration(type_def);
  let decl_path = type_def |> path_of_declaration(~path);
  switch (type_def) {
  | d
      when
        (
          switch (decl_path) {
          | None => false
          | Some(decl_path) => Declarations.is_complete(~path=decl_path)
          }
        )
        || Type.get(~path)
        |> CCOpt.is_some =>
    ()
  | ExportDefault(IdentifierReference(i)) =>
    parse__type_def(
      ~inline,
      ~path,
      Type({
        pi: i.pi,
        item: {
          t_ident: {
            pi: i.pi,
            item: "default",
          },
          t_parameters: None,
          t_type:
            TypeReference((CCList.map(to_pi, fst(path)) @ [i], None)),
        },
      }),
    )
  | Export(declaration)
  | ExportDefault(declaration)
  | Ambient(declaration) => parse__type_def(~inline, ~path, declaration)
  | FunctionDec({
      item: {
        f_ident,
        f_call_signature: {
          cs_type_parameters,
          cs_parameter_list,
          cs_type_annotation,
        },
      },
      pi,
    }) =>
    parse__type_def(
      ~inline,
      ~path,
      Type({
        pi,
        item: {
          t_ident: f_ident,
          t_parameters: cs_type_parameters,
          t_type:
            Function({
              f_parameters: None,
              f_body: cs_parameter_list,
              f_ret: cs_type_annotation |> CCOpt.value(~default=Ts.Any(pi)),
            }),
        },
      }),
    )
  | Type({item: {t_ident, t_parameters, t_type}, _}) =>
    let ident = t_ident |> Ident.of_pi;
    let t_path = inline ? path : path |> Path.add_ident(ident);
    let _ =
      parse__type_parameters(
        ~path=t_path,
        t_parameters |> CCOpt.value(~default=[]),
      );

    let t = parse__type(~path=t_path, t_type);
    let t =
      TypeDeclaration({
        td_name: ident,
        td_type: t,
        td_parameters: Parameters.get_unpacked(~path=t_path),
      });
    Type.add_order(t_path);
    Type.add(~path=t_path, t);
  | Namespace({pi, item: {n_ident, n_declarations}}) =>
    let as_module: Ts.with_pi((string, list(Ts.declaration))) = {
      Ts.pi,
      item: (n_ident |> CCList.get_at_idx_exn(0) |> no_pi, n_declarations),
    };
    parse__type_def(~path, Module(as_module));
  | Module({item, _}) => parse__module(~path, item)
  | Interface({item: {i_ident, i_parameters, i_extends, i_members}, _}) =>
    let ident = i_ident |> Ident.of_pi;
    let t_path = inline ? path : path |> Path.add_ident(ident);

    // Parameters need to be parsed before extension so they can be applied to potentially extended fields
    let _ =
      parse__type_parameters(
        ~path=t_path,
        i_parameters |> CCOpt.value(~default=[]),
      );

    // TODO: Unclean & naively solved, replace later
    let add_fields =
      switch (i_extends) {
      | Some([])
      | None => []
      | Some(extends) =>
        extends
        |> CCList.fold_left(
             (p, ext) => {
               switch (ext) {
               | ([], _) => p
               | (ref_path, args: option(list(Ts.type_))) =>
                 let ref_path = ref_path |> CCList.map(no_pi);
                 switch (
                   Ref.resolve_ref(~from=path, (ref_path, []))
                   |> CCOpt.flat_map(v => Type.get(~path=v))
                 ) {
                 | None => p
                 | Some(ref_type) =>
                   switch (ref_type) {
                   | TypeDeclaration(
                       {td_type: Interface(add_fields, _), _} as td,
                     ) =>
                     let td_parameters =
                       Parameters.get_unpacked(~path=(ref_path, []));
                     // Now that this td has been extended it needs to be updated
                     Type.replace(
                       ~path=(ref_path, []),
                       TypeDeclaration({
                         ...td,
                         td_type: Interface(add_fields, true),
                       }),
                     );

                     let args =
                       args
                       |> CCOpt.map_or(
                            ~default=[],
                            CCList.mapi((i, param) => {
                              let path =
                                t_path |> Path.add_sub(string_of_int(i));
                              parse__type(~inline=true, ~path, param);
                            }),
                          );

                     let applied_types =
                       parse__apply_arguments(
                         ~path=(ref_path, []),
                         ~applied_types=args,
                       )
                       |> CCList.mapi((i, t) =>
                            (
                              CCList.get_at_idx_exn(i, td_parameters).tp_name,
                              t,
                            )
                          );

                     let add_fields =
                       add_fields
                       |> CCList.map(field =>
                            switch (field) {
                            | {f_type: Arg(arg_ident), _} =>
                              switch (
                                applied_types
                                |> CCList.find_opt(((ident, _)) =>
                                     Ident.eq(arg_ident, ident)
                                   )
                              ) {
                              | None =>
                                raise(
                                  Exceptions.Parser_unexpected(
                                    Printf.sprintf(
                                      "Could not satisfy an Arg constraint while extending a record. This should not be possible. Path: %s",
                                      t_path |> Path.to_string,
                                    ),
                                  ),
                                )
                              | Some((_, t)) => {...field, f_type: t}
                              }
                            | _ => field
                            }
                          );

                     p @ add_fields;
                   | _ => p
                   }
                 };
               }
             },
             [],
           )
      };

    let t =
      switch (parse__type(~path=t_path, Ts.Object(i_members))) {
      | Interface(has_fields, extended) =>
        Interface(
          add_fields
          @ has_fields
          |> CCList.uniq(
               ~eq=({f_name as ident_a, _}, {f_name as ident_b, _}) =>
               CCEqual.string(ident_a |> Ident.ident, ident_b |> Ident.ident)
             ),
          extended,
        )
      | v => v
      };
    let t =
      TypeDeclaration({
        td_name: ident,
        td_type: t,
        td_parameters: Parameters.get_unpacked(~path=t_path),
      });
    Type.add_order(t_path);
    Type.add(~path=t_path, t);
  | Enum({item: {e_ident, e_const, e_members}, _}) =>
    let ident = e_ident |> Ident.of_pi;
    let t_path = inline ? path : path |> Path.add_ident(ident);
    let t =
      TypeDeclaration({
        td_name: ident,
        td_type: parse__enum(~path=t_path, e_members, e_const),
        td_parameters: [],
      });
    Type.add_order(t_path);
    Type.add(~path=t_path, t);
  // | Import(_) =>
  //   raise(
  //     Exceptions.Parser_unexpected(
  //       "Imports should not be parsed in this function",
  //     ),
  //   )
  | d =>
    raise(
      Exceptions.Parser_unsupported(
        Printf.sprintf(
          "Declaration not implemented: %s",
          declaration_to_string(d),
        ),
        position_of_declaration(d),
      ),
    )
  };

  switch (decl_path) {
  | None => ()
  | Some(path) => Declarations.set_complete(~path)
  };
}
/**
  Unions
*/
and parse__union = (~path, ~left: Ts.type_, ~right: option(Ts.type_)) => {
  let rec to_tmp = (type_: Ts.type_): Ts.temp_union_member =>
    switch (type_) {
    | Ts.StringLiteral({item, _}) => `U_String(item)
    | Ts.NumberLiteral({item, _}) => `U_Number(item)
    | Ts.BoolLiteral({item, _}) => `U_Bool(item)
    | other => `U_Type(other)
    }
  and to_temp_union_list =
      (
        ~carry: list(Ts.temp_union_member)=[],
        ~left: Ts.type_,
        ~right: option(Ts.type_),
      ) => {
    let carry =
      switch (right) {
      | None => carry
      | Some(Union(left, right)) =>
        to_temp_union_list(~carry, ~left, ~right)
      | Some(t) => carry @ [to_tmp(t)]
      };

    switch (left) {
    | Union(left, right) => to_temp_union_list(~carry, ~left, ~right)
    | other => carry @ [to_tmp(other)]
    };
  };
  parse__union_prepared(
    ~path,
    to_temp_union_list(~carry=[], ~left, ~right) |> CCList.rev,
  );
}
and parse__union_prepared = (~path, members: list(Ts.temp_union_member)) => {
  // TODO: Switch on references in the simple unions to maybe get the same type
  switch (parse__union_undefined(~path, members)) {
  | Some(t) => t
  | None =>
    switch (parse__union_nullable(~path, members)) {
    | Some(t) => t
    | None =>
      switch (parse__string_literal(members)) {
      | Some(t) => t
      | None =>
        switch (parse__numeric_literal(members)) {
        | Some(t) => t
        | None =>
          switch (parse__mixed_literal(members)) {
          | Some(t) => t
          | None => parse__union_type(~path, members)
          }
        }
      }
    }
  };
}
and parse__union_type = (~path, members: list(Ts.temp_union_member)) => {
  let (strings, numbers, _, types) as b =
    members
    |> CCList.foldi(
         ((str, num, bool, types), i) =>
           fun
           | `U_String(_) as s => (str @ [s], num, bool, types)
           | `U_Number(_) as n => (str, num @ [n], bool, types)
           | `U_Bool(_) => (
               str,
               num,
               true,
               bool ? types : types @ [Base(Boolean)],
             )
           | `U_Type(n) => (
               str,
               num,
               bool,
               types
               @ [
                 parse__type(
                   ~path=path |> Path.add_sub(string_of_int(i)),
                   n,
                 ),
               ],
             ),
         ([], [], false, []),
       );

  let types =
    types
    @ (
      strings
      |> list_to_opt
      |> CCOpt.flat_map(parse__string_literal)
      |> CCOpt.to_list
    )
    @ (
      numbers
      |> list_to_opt
      |> CCOpt.flat_map(parse__numeric_literal)
      |> CCOpt.to_list
    );
  Union(
    types
    |> CCList.map(t =>
         {um_type: t, um_classifier: "", um_ident: get_union_type_name(t)}
       ),
  );
}
and parse__mixed_literal = (members: list(Ts.temp_union_member)) => {
  exception No_union_mixed;
  try(
    Some(
      MixedLiteral(
        members
        |> CCList.fold_left(
             p =>
               fun
               | `U_String(n) => {
                   ...p,
                   strings: p.strings @ [n |> Ident.of_string],
                 }
               | `U_Number(n) => {...p, numbers: p.numbers @ [n]}
               | `U_Bool(n) => {...p, bools: p.bools @ [n]}
               | _ => raise(No_union_mixed),
             {strings: [], numbers: [], bools: []},
           ),
      ),
    )
  ) {
  | No_union_mixed => None
  | e => raise(e)
  };
}
and parse__numeric_literal = (members: list(Ts.temp_union_member)) => {
  exception No_union_number;
  try(
    Some(
      NumericLiteral(
        members
        |> CCListLabels.map(
             ~f=
               fun
               | `U_Number(n) => n
               | _ => raise(No_union_number),
           ),
      ),
    )
  ) {
  | No_union_number => None
  | e => raise(e)
  };
}
and parse__string_literal = (members: list(Ts.temp_union_member)) => {
  exception No_union_string;
  try(
    Some(
      StringLiteral(
        members
        |> CCList.map(
             fun
             | `U_String(str) => str |> Ident.of_string
             | _ => raise(No_union_string),
           ),
      ),
    )
  ) {
  | No_union_string => None
  | e => raise(e)
  };
}
and parse__union_undefined = (~path, members: list(Ts.temp_union_member)) => {
  let extract_undefined =
    members
    |> CCList.fold_left(
         ((has_undefined, p), member) => {
           switch (member) {
           | `U_Type(Ts.Undefined(_)) => (true, p)
           | member => (has_undefined, [member, ...p])
           }
         },
         (false, []),
       );
  switch (extract_undefined) {
  | (true, [`U_Type(type_)]) => Some(parse__optional(~path, type_))
  | (true, members) => Some(parse__optional(~path, Ts.UnionTemp(members)))
  | (false, _) => None
  };
}
and parse__union_nullable = (~path, members: list(Ts.temp_union_member)) => {
  let extract_null =
    members
    |> CCList.fold_left(
         ((has_null, p), member) => {
           switch (member) {
           | `U_Type(Ts.Null(_)) => (true, p)
           | member => (has_null, [member, ...p])
           }
         },
         (false, []),
       );
  switch (extract_null) {
  | (true, [`U_Type(type_)]) => Some(parse__nullable(~path, type_))
  | (true, members) => Some(parse__nullable(~path, Ts.UnionTemp(members)))
  | (false, _) => None
  };
}
/**
  Type arguments
*/
and parse__type_parameters =
    (~path, parameters: Ts.type_parameters): list(ts_type_parameter) => {
  let parameters =
    parameters
    |> CCList.map(({tp_ident, tp_extends, tp_default}: Ts.type_parameter) =>
         (
           {
             let ident = tp_ident |> Ident.of_pi;
             let param_path = Path.(path |> add_sub_ident(ident));
             {
               tp_name: ident,
               tp_extends: None,
               // TODO: Re-Implement this & check passed args against it
               //  tp_extends
               //  |> CCOpt.map(parse__type(~inline=true, ~path=param_path)),
               tp_default:
                 tp_default
                 |> CCOpt.map(parse__type(~inline=true, ~path=param_path)),
             };
           }: ts_type_parameter
         )
       );
  Parameters.add_list_fixed(~path, parameters);
  parameters;
}
/**
  Type extraction
*/
and parse__type_extraction =
    (
      ~parsed_source=?,
      ~path,
      source: Ts.type_extract_source,
      access_fields: list(list(Ts.field_access_item)),
    ) => {
  // First, if this is a direct object extraction, we need to parse the source type
  // If it's a reference, we need to render that all the way through
  let lookup_base =
    switch (source) {
    | Ts.TeObject(fields) =>
      let local_path = path |> Path.add_sub("t");
      parse__type(~inline=true, ~path=local_path, Ts.Object(fields))
      |> ignore;
      local_path;
    | Ts.TeTypeReference(tr) =>
      switch (parsed_source) {
      | Some(parsed_source) => parsed_source.tr_path_resolved |> CCOpt.get_exn
      | None => resolve__type_reference_path(~path, tr)
      }
    };
  // Next, validate the format of the access_fields
  // Only the last element should contain multiple keys, if any
  // Also, all references should be resolved and validated as string / string literal types
  let access_path =
    access_fields
    |> CCList.take(CCList.length(access_fields) - 1)
    |> CCList.map(field =>
         switch (field) {
         | [Ts.FaString(str)] => no_pi(str)
         | [Ts.FaIdentifier(ident_path)] =>
           switch (
             resolve__to_final_type(
               ~path,
               Ts.TypeReference((ident_path, None)),
             )
           ) {
           | Some(StringLiteral([one])) => Ident.value(one)
           | Some(other) =>
             raise(
               Exceptions.Parser_parameter_error(
                 Printf.sprintf(
                   "Error: Resolved type is not a String Literal, but a %s (at path: %s -> %s)",
                   ts_to_string(other),
                   Path.pp(path),
                   ident_path |> CCList.to_string(~sep=".", no_pi),
                 ),
               ),
             )
           | None =>
             raise(
               Exceptions.Parser_parameter_error(
                 Printf.sprintf(
                   "Error: Could not resolve to final type for type extraction (at path: %s -> %s)",
                   Path.pp(path),
                   ident_path |> CCList.to_string(~sep=".", no_pi),
                 ),
               ),
             )
           }
         | v =>
           Console.error(v);
           raise(
             Exceptions.Parser_parameter_error(
               Printf.sprintf(
                 "Error: Cannot parse field access with multiple keys before the last element (at path: %s)",
                 Path.pp(path),
               ),
             ),
           );
         }
       );
  let access_fields =
    access_fields
    |> CCList.get_at_idx_exn(CCList.length(access_fields) - 1)
    |> CCList.fold_left(
         (p, field) =>
           switch (field) {
           | Ts.FaString(str) => p @ [no_pi(str)]
           | Ts.FaIdentifier(ident_path) =>
             switch (
               resolve__to_final_type(
                 ~path,
                 Ts.TypeReference((ident_path, None)),
               )
             ) {
             | Some(StringLiteral(vals)) => vals |> CCList.map(Ident.value)
             | Some(other) =>
               raise(
                 Exceptions.Parser_parameter_error(
                   Printf.sprintf(
                     "Error: Resolved type is not a String Literal, but a %s at access_fields (at path: %s -> %s -> %s)",
                     ts_to_string(other),
                     Path.pp(path),
                     Path.pp(lookup_base),
                     ident_path |> CCList.to_string(~sep=".", no_pi),
                   ),
                 ),
               )
             | None =>
               raise(
                 Exceptions.Parser_parameter_error(
                   Printf.sprintf(
                     "Error: Could not resolve to final type for type extraction at access_fields (at path: %s -> %s -> %s)",
                     Path.pp(path),
                     Path.pp(lookup_base),
                     ident_path |> CCList.to_string(~sep=".", no_pi),
                   ),
                 ),
               )
             }
           },
         [],
       );

  // Now that all necessary types are parsed, it should be easy to construct a path
  // That path should be all parts of access_fields but the last one
  let lookup_path =
    access_path
    |> CCList.fold_left((p, part) => p |> Path.add_sub(part), lookup_base);

  let get_field_type = (fields, one, history: list(Path.t)) =>
    switch (
      fields |> CCList.find_opt(({f_name, _}) => f_name.i_value == one)
    ) {
    | Some({f_type: Arg(ident) as t, _}) =>
      let applied_args =
        switch (parsed_source) {
        | None =>
          switch (source) {
          | Ts.TeTypeReference(tref) =>
            parse__apply_ref_arguments(
              ~path,
              ~resolved_ref=lookup_path,
              ~type_ref=tref,
            )
          | _ =>
            raise(
              Exceptions.Parser_unexpected(
                "Expected a type reference for this type extraction, but got an inline object",
              ),
            )
          }
        | Some({tr_parameters, _}) => tr_parameters
        };
      let params_path =
        switch (history |> CCList.last_opt) {
        | None => lookup_path
        | Some(p) => p
        };
      let params = Parameters.get_root_unpacked(~path=params_path);
      let arg =
        params
        |> CCList.find_idx(param => Ident.eq(param.tp_name, ident))
        |> CCOpt.map(fst)
        |> CCOpt.flat_map(CCList.get_at_idx(_, applied_args));
      arg |> CCOpt.value(~default=t);
    | Some({f_type, _}) => f_type
    | None =>
      Console.warn(
        Printf.sprintf(
          "Warning: Field %s not found in interface (at path: %s -> %s, %s)",
          one,
          Path.pp(path),
          Path.pp(lookup_path),
          ts_to_string(Interface(fields, false)),
        ),
      );
      Base(Any);
    };

  switch (follow__to_final_by_path(~path=lookup_path)) {
  | Ok((Interface(fields, _), history)) =>
    switch (access_fields) {
    | [one] =>
      Ref.add(~from=path, ~to_=lookup_path |> Path.add_sub(one));
      get_field_type(fields, one, history);
    | lst =>
      Union(
        lst
        |> CCList.map(one => {
             Ref.add(~from=path, ~to_=lookup_path |> Path.add_sub(one));
             let t = get_field_type(fields, one, history);
             {
               um_ident: get_union_type_name(t),
               um_type: t,
               um_classifier: "",
             };
           }),
      )
    }
  | Ok((other, history)) =>
    Console.warn(
      Printf.sprintf(
        "Warning: Interface type for field access not found. Got type '%s' instead (at path: %s -> %s, , followed through: %s)",
        ts_to_string(other),
        Path.pp(path),
        Path.pp(lookup_path),
        CCList.to_string(~sep=" | ", Path.pp, history),
      ),
    );
    Base(Any);
  | Error(history) =>
    Console.warn(
      Printf.sprintf(
        "Warning: Interface type for field access not found (at path: %s -> %s, followed through: %s)",
        Path.pp(path),
        Path.pp(lookup_path),
        CCList.to_string(~sep=" | ", Path.pp, history),
      ),
    );
    Base(Any);
  };
}
/**
  Array
*/
and parse__array = (~path, type_) => {
  Array(parse__type(~inline=true, ~path=path |> Path.add_sub("t"), type_));
}
and parse__nullable = (~path, type_) => {
  Nullable(
    parse__type(~inline=true, ~path=path |> Path.add_sub("t"), type_),
  );
}
and parse__optional = (~path, type_) => {
  Optional(
    parse__type(~inline=true, ~path=path |> Path.add_sub("t"), type_),
  );
}
/**
  Type reference
*/
and parse__type_reference = (~path, type_ref: Ts.type_reference): ts_type => {
  let ref_path = fst(type_ref) |> CCList.map(no_pi);
  let ref_path_p = (ref_path, []);

  // Try to render dynamically from the declaration table
  if (!Path.eq_unscoped(ref_path, fst(path))) {
    let paths =
      [
        Some(((path |> Path.to_scope) @ ref_path, [])),
        Some(ref_path_p),
        switch (ref_path) {
        | [one] =>
          NamedImports.find(
            ~path=(Path.to_scope(path), []),
            Ident.of_string(one),
          )
        | _ => None
        },
        switch (ref_path) {
        | [one, ...rest] =>
          switch (
            NamespaceImports.find(
              ~path=(Path.to_scope(path), []),
              Ident.of_string(one),
            )
          ) {
          | Some((ns_path, _)) => Some((ns_path @ rest, []))
          | None => None
          }
        | _ => None
        },
      ]
      |> CCList.keep_some;
    paths
    |> CCList.iter(check_path =>
         if (Declarations.has(~path=check_path)) {
           parse__type_def(
             ~path=check_path |> Path.cut,
             Declarations.get_type_declaration(~path=check_path),
           );
         }
       );
  };

  switch (parse__inline_type_parameter(~path, ref_path_p)) {
  | Some(arg) => arg
  | None =>
    let resolved_ref =
      Ref.resolve_ref(
        ~from=
          CCEqual.list(CCEqual.string, fst(path), ref_path)
            ? (fst(path), []) : path,
        ref_path_p,
      );

    let parameters =
      switch (resolved_ref) {
      | None => []
      | Some(resolved_ref) =>
        parse__apply_ref_arguments(~path, ~resolved_ref, ~type_ref)
      };

    let resolved = resolved_ref |> CCOpt.value(~default=ref_path_p);
    // Typeof Lazy Params needs to be executed on the spot
    switch (Type.get(~path=resolved) |> CCOpt.map(resolve__to_clean_ts)) {
    | Some(LazyParams(l)) =>
      let params =
        Parameters.get_unpacked(~path=resolved)
        |> CCList.map(({tp_name, _}) => tp_name);
      let params_with_idents = CCList.combine(params, parameters);
      Lazy.force(l, params_with_idents);
    | _ =>
      // If this is a sub field, make a reference to it
      if (Path.has_sub(path)) {
        RefInline.add(~path, resolved);
      };
      Reference({
        tr_path: ref_path,
        tr_path_resolved: Some(resolved),
        tr_parameters: parameters,
      });
    };
  };
}
/**
  Try to resolve to a parameter for an ident
*/
and parse__inline_type_parameter =
    (~path: Path.t, ref_path: Path.t): option(ts_type) => {
  switch (ref_path) {
  | ([r], []) =>
    let r_ident = r |> Ident.of_string;
    let has = Parameters.has(~fixed_only=false, ~path, r_ident);
    let has_root = Parameters.has_root(~fixed_only=false, ~path, r_ident);

    switch (has, has_root, Path.eq(path, (fst(path), []))) {
    | (true, _, true) => Some(parse__type_argument(~path, r_ident))
    | (false, true, _)
    | (true, true, false) =>
      Parameters.hoist_from_root_by_ident(~path, r_ident);
      Some(parse__type_argument(~path, r_ident));
    | _ => None
    };
  | _ => None
  };
}
/**
  Reference specific argument
*/
and parse__apply_ref_arguments =
    (~path, ~resolved_ref: Path.t, ~type_ref: Ts.type_reference)
    : list(ts_type) => {
  let ref_types = snd(type_ref);

  let ref_applied_types =
    ref_types
    |> CCOpt.map_or(
         ~default=[],
         CCList.mapi((i, param) => {
           switch (param) {
           | Ts.TypeReference(([{item, _}], _)) =>
             switch (parse__inline_type_parameter(~path, ([item], []))) {
             | Some(arg) => arg
             | None =>
               let path = path |> Path.add_sub(string_of_int(i));
               parse__type(~inline=true, ~path, param);
             }
           | _ =>
             let path = path |> Path.add_sub(string_of_int(i));
             parse__type(~inline=true, ~path, param);
           }
         }),
       );

  // Inherited params should just be passed on as they cannot be filled
  // TODO: Probably need to account for original default value for example
  let inherited = Parameters.get_inherited(~path=resolved_ref);

  Parameters.add_list_no_duplicates(
    ~path,
    inherited |> CCList.map(Parameters.wrap_child),
  );
  let inherited =
    inherited
    |> CCList.map(iparam => parse__type_argument(~path, iparam.tp_name));
  parse__apply_arguments(
    ~path=resolved_ref,
    ~applied_types=ref_applied_types @ inherited,
  );
}
/**
  Argument resolver
*/
and parse__apply_arguments =
    (~path: Path.t, ~applied_types: list(ts_type)): list(ts_type) => {
  // --- Duplicate Detection
  let l = CCList.length;
  let params = Parameters.get_unpacked(~path);
  let param_idents = params |> CCList.map(p => p.tp_name |> Ident.value);
  let param_idents_uniq = param_idents |> CCList.uniq(~eq=CCEqual.string);
  if (CCList.compare_lengths(param_idents, param_idents_uniq) > 0) {
    let duplicates =
      param_idents
      |> CCList.filter(i => !CCList.memq(i, param_idents_uniq))
      |> CCList.to_string(~sep=", ", v => v);
    raise(
      Exceptions.Parser_parameter_error(
        Printf.sprintf(
          "Invalid type reference: Duplicate type parameter names detected. The following ident(s) are duplicates: %s. Path: %s.",
          duplicates,
          Path.pp(path),
        ),
      ),
    );
  };

  // --- Applying
  let (n_required, append_defaults) =
    params
    |> CCList.fold_left(
         ((i, lst), arg_type) => {
           switch (lst, arg_type) {
           | ([], {tp_default: None, _}) => (i + 1, lst)
           | (lst, {tp_default: None, _}) =>
             raise(
               Exceptions.Parser_parameter_error(
                 Printf.sprintf(
                   "Invalid type reference: Cannot have a required type arg after the first optional (path: %s)",
                   Path.pp(path),
                 ),
               ),
             )
           | (lst, {tp_name, tp_default: Some(t), _}) => (i, lst @ [t])
           }
         },
         (0, []),
       );
  let n_required = n_required;
  let n_defaults = l(append_defaults);
  let n_total = n_required + n_defaults;
  let n_applied = l(applied_types);

  // --- Trouble Shooting
  switch (applied_types) {
  | applied when n_applied > 0 && CCList.length(params) === 0 =>
    Console.warn(
      Printf.sprintf(
        "Invalid type reference: Arguments passed to a type that is not in scope (%s)",
        path |> Path.pp,
      ),
    );
    applied;
  | applied when n_applied > n_total || n_applied < n_required =>
    raise(
      Exceptions.Parser_parameter_error(
        Printf.sprintf(
          "Invalid type reference: Applied %d arguments to a type where a minimum of %d and a maxiumum of %d are expected (%s)",
          n_applied,
          n_required,
          n_total,
          path |> Path.pp,
        ),
      ),
    )
  | applied when n_applied == n_total => applied
  | applied when n_applied > n_required =>
    applied @ CCList.drop(n_total - n_applied, append_defaults)
  | applied when n_applied == n_required => applied @ append_defaults
  | _ =>
    Console.error(path |> Path.pp);
    Console.error([n_required, n_total, n_applied]);
    raise(Exceptions.Parser_parameter_error("This should not be reachable"));
  };
}
and parse__type_argument = (~path, arg: ts_identifier) => {
  Arguments.add(~path=path |> Path.cut_sub, arg);
  Arg(arg);
}
/**
  Enums
*/
and parse__enum = (~path, members: list(Ts.enum_member), is_const: bool) => {
  let is_clean =
    members
    |> CCList.for_all((member: Ts.enum_member) =>
         !(member.em_value |> CCOpt.is_some)
         && (
           switch (member.em_property_name) {
           | PIdentifier(_) => true
           | _ => false
           }
         )
       );
  if (is_clean) {
    Enum(
      members
      |> CCList.map((member: Ts.enum_member) =>
           switch (member.em_property_name) {
           | PIdentifier(ident) => ident
           | _ => raise(Not_found)
           }
         )
      |> CCList.map(no_pi)
      |> CCList.map(Ident.of_string),
    );
  } else {
    raise(
      Exceptions.Parser_unsupported(
        "Complex enums are not yet supported",
        current_position^,
      ),
    );
  };
}
/**
  Tuples
*/
and parse__tuple = (~path, types) => {
  Tuple(
    types
    |> CCList.mapi((i, type_) => {
         let path = path |> Path.add_sub(string_of_int(i + 1));
         parse__type(~inline=true, ~path, type_);
       }),
  );
}
/**
  Interfaces
*/
and parse__interface =
    (~path, ~extended=false, members: list(Ts.type_member)) => {
  switch (members) {
  | [
      CallSignature({
        cs_type_parameters,
        cs_parameter_list,
        cs_type_annotation,
      }),
    ] =>
    parse__type(
      ~path,
      Ts.Function({
        f_parameters: cs_type_parameters,
        f_body: cs_parameter_list,
        f_ret:
          cs_type_annotation |> CCOpt.value(~default=Ts.Any(Parse_info.zero)),
      }),
    )
  | members =>
    Interface(
      members
      |> CCList.map((member: Ts.type_member) =>
           switch (member) {
           | PropertySignature({
               ps_property_name,
               ps_type_annotation,
               ps_optional,
             }) =>
             let ident =
               switch (ps_property_name) {
               | PIdentifier(ident)
               | PString(ident) => ident
               | PNumber({pi, item}) => {pi, item: item |> string_of_int}
               };
             let path = path |> Path.add_sub(ident |> no_pi);
             let parsed_type =
               parse__type(~inline=true, ~path, ps_type_annotation);
             {
               f_name: ident |> Ident.of_pi,
               f_type: ps_optional ? Optional(parsed_type) : parsed_type,
             };
           | MethodSignature({
               ms_property_name,
               ms_call_signature: {
                 cs_type_parameters,
                 cs_parameter_list,
                 cs_type_annotation,
               },
               ms_optional,
             }) =>
             let ident =
               switch (ms_property_name) {
               | PIdentifier(ident)
               | PString(ident) => ident
               | PNumber({pi, item}) => {pi, item: item |> string_of_int}
               };
             let path = path |> Path.add_sub(ident |> no_pi);
             let parsed_type =
               parse__type(
                 ~inline=true,
                 ~path,
                 Ts.Function({
                   f_parameters: cs_type_parameters,
                   f_body: cs_parameter_list,
                   f_ret:
                     cs_type_annotation
                     |> CCOpt.value(~default=Ts.Any(Parse_info.zero)),
                 }),
               );
             {
               f_name: ident |> Ident.of_pi,
               f_type: ms_optional ? Optional(parsed_type) : parsed_type,
             };
           | CallSignature(_) =>
             raise(
               Exceptions.Parser_unexpected(
                 "Don't know what to do with an unnamed call signature in a multi field interface",
               ),
             )
           | ConstructSignature(_) =>
             raise(
               Exceptions.Parser_unsupported(
                 "ConstructSignature not yet supported in interface",
                 current_position^,
               ),
             )
           | IndexSignature(_) as p =>
             raise(
               Exceptions.Parser_unsupported(
                 "IndexSignature not yet supported in interface",
                 position_of_type_member(p),
               ),
             )
           }
         ),
      extended,
    )
  };
}
/**
  All types
*/
and parse__type = (~inline=false, ~path, type_: Ts.type_) => {
  switch ((type_: Ts.type_)) {
  | String(_) => Base(String)
  | Number(_) => Base(Number)
  | Boolean(_) => Base(Boolean)
  | Void(_) => Base(Void)
  | Any(_) => Base(Any)
  | Null(_) => Base(Null)
  | Undefined(_) => Base(Undefined)
  | Union(left, right) as t =>
    inline ? parse__inline(~path, t) : parse__union(~path, ~left, ~right)
  | UnionTemp(members) as t =>
    inline ? parse__inline(~path, t) : parse__union_prepared(~path, members)
  | Tuple(types) as t =>
    inline ? parse__inline(~path, t) : parse__tuple(~path, types)
  | Array(t) => parse__array(~path, t)
  | TypeReference(ref_) => parse__type_reference(~path, ref_)
  | TypeExtract(type_ref, items) =>
    parse__type_extraction(~path, type_ref, items)
  | Object(members) as t =>
    inline
      ? parse__inline(~path, t)
      : parse__interface(~path, members |> CCOpt.value(~default=[]))
  | KeyOf({item, _}) as t =>
    inline ? parse__inline(~path, t) : parse__keyof(~path, item)
  | Function({f_body, f_ret, f_parameters}) as t =>
    inline
      ? parse__inline(~path, ~parameters=?f_parameters, t)
      : parse__function(~path, f_body, f_ret)
  | Constructor({f_body, f_ret, f_parameters}) as t =>
    inline
      ? parse__inline(~path, ~parameters=?f_parameters, t)
      : parse__function(~path, f_body, f_ret)
  | StringLiteral(_) as t
  | NumberLiteral(_) as t
  | BoolLiteral(_) as t => parse__type(~inline, ~path, Union(t, None))
  | Intersection(left, right) as t =>
    inline
      ? parse__inline(~path, t) : parse__intersection(~path, ~left, ~right)
  | Query(_) =>
    raise(
      Exceptions.Parser_unsupported(
        "Query type not yet supported",
        current_position^,
      ),
    )
  | MappedObject({item, _}) as t =>
    inline ? parse__inline(~path, t) : parse__mapped_object(~path, item)
  | Symbol(pi) =>
    raise(Exceptions.Parser_unsupported("Symbol type not yet supported", pi))
  | This(pi) =>
    raise(Exceptions.Parser_unsupported("This type not yet supported", pi))
  };
}
/**
  Inline Wrapper
*/
and parse__inline = (~path, ~parameters=?, type_) => {
  // If we have parameters in an inline parsing, we spread these params down the list
  // Important: We cannot forget to actually call `parse__type_parameters` here for the base type, so that defaults etc. get parsed correctly
  let parameters = parse__type_parameters(~path, list_of_opt(parameters));
  Parameters.spread_to_root(~path, parameters);

  parse__type_def(
    ~inline=true,
    ~path,
    Ts.Type(
      to_pi({
        Ts.t_ident: {
          pi: Parse_info.zero,
          item: "",
        },
        t_parameters: None, // Handled above, we can do that as we're now using the `Parameters` module as the single source of true
        t_type: type_,
      }),
    ),
  );

  let applied = Arguments.get(~path);
  let inherited =
    Parameters.get_inherited(~path) |> CCList.map(Parameters.ident_of_param);
  let args =
    switch (
      applied
      @ CCList.filter(
          param => !CCList.mem(~eq=Ident.eq, param, applied),
          inherited,
        )
    ) {
    | [] => []
    | args =>
      // If args have been used, we have to replace the type def we have just created
      // It can only happen after, as the args are only applied during parsing of the type_def
      switch (Type.get(~path)) {
      | Some(TypeDeclaration(td)) =>
        Type.replace(
          ~path,
          TypeDeclaration({
            ...td,
            td_parameters:
              CCList.filter_map(
                param => {
                  switch (args |> CCList.find_opt(Ident.eq(param.tp_name))) {
                  | None => Some(param)
                  | Some(_) => None
                  }
                },
                td.td_parameters,
              )
              @ CCList.map(
                  arg_ident =>
                    {tp_name: arg_ident, tp_extends: None, tp_default: None},
                  args,
                ),
          }),
        );
        args;
      | Some(_)
      | None =>
        raise(
          Exceptions.Parser_unexpected(
            Printf.sprintf(
              "Recently created type_def not found. This should never happen. Path: %s",
              path |> Path.to_string,
            ),
          ),
        )
      }
    };
  Ref.resolve_ref(~from=path, path) |> ignore;
  Reference({
    tr_path: fst(path),
    tr_path_resolved: Some(path),
    tr_parameters: args |> CCList.map(a => Arg(a)),
  });
}

/**
    Mapped object
*/
and parse__mapped_object = (~path, mapped_object: Ts.mapped_object) => {
  let {Ts.mo_ident, mo_type, mo_optional, mo_type_annotation, _} = mapped_object;

  let mo_ident_str = no_pi(mo_ident);
  let rec apply_type_annotation = (~maybe_params=?, ~ta, key) =>
    switch (ta, maybe_params) {
    | (
        Some(
          Ts.TypeExtract(
            Ts.TeTypeReference(([one], _)),
            [[Ts.FaIdentifier([{item, _}])]],
          ),
        ),
        Some(params),
      )
        when item == mo_ident_str =>
      switch (
        params
        |> CCList.assoc_opt(~eq=Ident.eq, Ident.of_string(no_pi(one)))
      ) {
      | Some(Reference({tr_path_resolved: Some(ref_path), _} as tr)) =>
        parse__type_extraction(
          ~parsed_source=tr,
          ~path,
          Ts.TeTypeReference((fst(ref_path) |> CCList.map(to_pi), None)),
          [[Ts.FaString(to_pi(key))]],
        )
      | Some(_)
      | None => apply_type_annotation(~maybe_params=?None, ~ta, key)
      }
    | (
        Some(Ts.TypeExtract(source, [[Ts.FaIdentifier([{item, _}])]])),
        _,
      )
        when item == mo_ident_str =>
      parse__type_extraction(~path, source, [[Ts.FaString(to_pi(key))]])
    | (Some(Ts.TypeReference(([{item, _}], _))), Some(params)) =>
      switch (
        params |> CCList.assoc_opt(~eq=Ident.eq, Ident.of_string(item))
      ) {
      | Some(param) => param
      | None => apply_type_annotation(~maybe_params=?None, ~ta, key)
      }
    | (Some(other), _) =>
      parse__type(~inline=true, ~path=path |> Path.add_sub("t"), other)
    | (None, _) => Base(Any)
    };
  let apply_optional = ts =>
    switch (mo_optional, ts) {
    | (Some(false), Optional(ts)) => ts
    | (Some(true), Optional(_) as ts)
    | (Some(false), ts)
    | (None, ts) => ts
    | (Some(true), ts) => Optional(ts)
    };

  let resolve = (~maybe_params=?) =>
    fun
    | StringLiteral(keys) =>
      Interface(
        keys
        |> CCList.map(key =>
             {
               f_name: key,
               f_type:
                 apply_type_annotation(
                   ~maybe_params?,
                   ~ta=mo_type_annotation,
                   Ident.value(key),
                 )
                 |> apply_optional,
             }
           ),
        false,
      )
    | other =>
      raise(
        Exceptions.Parser_unexpected(
          Printf.sprintf(
            "Unexpected lazy result for mapped_obj: %s",
            ts_to_string(other),
          ),
        ),
      );

  switch (resolve__to_final_type(~maybe_add=no_pi(mo_ident), ~path, mo_type)) {
  | Some(StringLiteral(_) as t) => resolve(t)
  | Some(Arg(ident)) =>
    LazyParams(
      lazy(
        {
          (
            params =>
              resolve(
                ~maybe_params=params,
                resolve__to_final_type_ts(
                  ~path,
                  params |> CCList.assoc(~eq=Ident.eq, ident),
                )
                |> CCOpt.get_exn,
              )
          );
        }
      ),
    )
  | Some(LazyParams(l)) =>
    LazyParams(
      lazy(
        {
          (params => resolve(~maybe_params=params, Lazy.force(l, params)));
        }
      ),
    )
  | Some(Lazy(l)) =>
    Lazy(
      lazy(
        {
          (() => resolve(Lazy.force(l, ())));
        }
      ),
    )
  | Some(other) =>
    Console.warn(
      Printf.sprintf(
        "Warning: Type '%s' cannot be used in a mapped object type. String or string literal required.",
        ts_to_string(other),
      ),
    );
    Base(Any);
  | None =>
    Console.warn(
      Printf.sprintf(
        "Warning: Raw type '%s' cannot be used in a mapped object type. String or string literal required.",
        type_to_string(mo_type),
      ),
    );
    Base(Any);
  };
}
and resolve__type_reference_path = (~path, tr: Ts.type_reference) => {
  switch (parse__type_reference(~path, tr)) {
  | Reference({tr_path_resolved: Some(ref_path), _}) => ref_path
  | other =>
    raise(
      Exceptions.Parser_unexpected(
        Printf.sprintf(
          "Unexpected type '%s' from a type reference parse (parse__type_extraction, path: %s)",
          ts_to_string(other),
          Path.pp(path),
        ),
      ),
    )
  };
}
/**
    Follow helpers
    (These will only try to resolve from already parsed types)
*/
and follow__to_final_by_path =
    (~path: Path.t): Result.t((ts_type, list(Path.t)), list(Path.t)) => {
  let rec follow = (~history=[], path) => {
    let path =
      switch (RefInline.get(~path)) {
      | None => path
      | Some(resolved) => resolved
      };
    switch (Type.get(~path) |> CCOpt.map(resolve__to_clean_ts)) {
    | Some(Reference({tr_path_resolved: Some(next_path), _})) =>
      follow(~history=history @ [path], next_path)
    | Some(other) => Ok((other, history))
    | None => Error(history)
    };
  };
  follow(path);
}
/**
    Resolve helpers
    (These will parse along the way)
*/
and resolve__to_clean_ts = (ts: ts_type) =>
  switch (ts) {
  | TypeDeclaration({td_type, _}) => resolve__to_clean_ts(td_type)
  | Optional(t)
  | Nullable(t)
  | Array(t) => resolve__to_clean_ts(t)
  | other => other
  }
and resolve__to_final_type_ts = (~path, ts: ts_type): option(ts_type) => {
  switch (ts) {
  | TypeDeclaration({td_type: Reference({tr_path_resolved: Some(p), _}), _})
  | Reference({tr_path_resolved: Some(p), _}) =>
    switch (Type.get(~path=p)) {
    | Some(t) => resolve__to_final_type_ts(~path, resolve__to_clean_ts(t))
    | None =>
      if (Path.eq_unscoped(fst(path), fst(p)) && Declarations.has(~path=p)) {
        Some(
          Lazy(
            lazy(
              {
                (() => resolve__to_final_type_ts(~path, ts) |> CCOpt.get_exn);
              }
            ),
          ),
        );
      } else {
        None;
      }
    }
  | final => Some(resolve__to_clean_ts(final))
  };
}
and resolve__to_final_type = (~maybe_add=?, ~path, type_): option(ts_type) => {
  let path =
    maybe_add
    |> CCOpt.map(add => path |> Path.add_sub(add))
    |> CCOpt.value(~default=path);
  resolve__to_final_type_ts(~path, parse__type(~inline=true, ~path, type_));
}
/**
    KeyOf
*/
and parse__keyof = (~path, type_) => {
  let rec chain_of_list = lst =>
    switch (lst) {
    | [] => raise(Not_found)
    | [l] => Ts.Union(l, None)
    | [l, r] => Ts.Union(l, Some(r))
    | [l, ...r] => Ts.Union(l, Some(chain_of_list(r)))
    };
  // It might happen, that we want to get a keyof in recursion. As the parent has not been fully parsed yet, it can't be referenced
  // So we'll do a parent check here & try to extract the keys later
  let resolved =
    switch (resolve__to_final_type(~maybe_add="t", ~path, type_)) {
    | Some(Interface(_) as t) => Ok(t)
    | Some(Arg(ident)) =>
      Error(
        LazyParams(
          lazy(
            {
              (
                params =>
                  resolve__to_final_type_ts(
                    ~path,
                    params |> CCList.assoc(~eq=Ident.eq, ident),
                  )
                  |> CCOpt.get_exn
              );
            }
          ),
        ),
      )
    | Some(Lazy(_) as t) => Error(t)
    | Some(other) =>
      raise(
        Exceptions.Parser_unexpected(
          Printf.sprintf(
            "Did not expect type '%s' in a keyof statement (path: %s)",
            ts_to_string(other),
            Path.pp(path),
          ),
        ),
      )
    | None =>
      raise(
        Exceptions.Parser_unexpected(
          Printf.sprintf(
            "Did not expect raw type '%s' in a keyof statement (path: %s)",
            type_to_string(type_),
            Path.pp(path),
          ),
        ),
      )
    };

  let finish = resolved => {
    switch (resolved) {
    | Interface(fields, _) =>
      fields
      |> CCList.map(field =>
           Ts.StringLiteral({
             pi: Parse_info.zero,
             item: field.f_name |> Ident.value,
           })
         )
      |> chain_of_list
      |> parse__type(~path)
    | _ => parse__type(~path, Ts.Any(Parse_info.zero))
    };
  };
  switch (resolved) {
  | Ok(t) => finish(t)
  | Error(Lazy(res)) =>
    Lazy(
      lazy(
        {
          (() => finish(Lazy.force(res, ())));
        }
      ),
    )
  | Error(LazyParams(l)) =>
    LazyParams(
      lazy(
        {
          (params => finish(Lazy.force(l, params)));
        }
      ),
    )
  | Error(_) =>
    raise(Exceptions.Parser_unexpected("Unexpected error in keyof"))
  };
}
/**
  Intersection
*/
and parse__intersection = (~path, ~left: Ts.type_, ~right: Ts.type_) => {
  let rec types_from_intersection =
          (~carry=[], ~left: Ts.type_, ~right: Ts.type_) => {
    let new_carry =
      switch (left) {
      | Ts.Intersection(left, right) =>
        types_from_intersection(~carry, ~left, ~right)
      | other => carry @ [other]
      };
    new_carry @ [right];
  };
  let parsed_types = types_from_intersection(~carry=[], ~left, ~right);
  switch (runtime.config.intersection_mode) {
  | {
      objects: Tuple,
      unions: Tuple,
      classes: Tuple,
      functions: Tuple,
      other: Tuple,
      tuple_members_optional,
    } =>
    parse__type(
      ~path,
      Tuple(
        tuple_members_optional
          ? parsed_types
            |> CCList.map(t =>
                 Ts.Union(t, Some(Ts.Undefined(Parse_info.zero)))
               )
          : parsed_types,
      ),
    )
  | _ =>
    raise(
      Exceptions.Parser_unsupported(
        "No intersection mode other than tuple is supported yet",
        current_position^,
      ),
    )
  };
}
/**
  Function
*/
and parse__function = (~path, body: Ts.parameter_list, return: Ts.type_) => {
  Function({
    fu_params:
      body
      |> CCList.map((param: Ts.parameter_type) => {
           switch (param) {
           | Parameter({
               p_optional,
               p_ibinding: IdentifierBinding(p_ident),
               p_type_annotation,
               _,
             }) =>
             let path = path |> Path.add_sub(p_ident |> no_pi);
             {
               fp_name: p_ident |> Ident.of_pi,
               fp_type:
                 p_type_annotation
                 |> CCOpt.map_or(
                      ~default=Base(Any),
                      parse__type(~inline=true, ~path),
                    ),
               fp_optional: p_optional,
             };
           | Parameter(_) =>
             raise(
               Exceptions.Parser_unsupported(
                 "Bindings other than IdentifierBinding in function not yet supported",
                 current_position^,
               ),
             )
           | RestParameter(_) =>
             raise(
               Exceptions.Parser_unsupported(
                 "RestParameter in function not yet supported",
                 current_position^,
               ),
             )
           }
         }),
    fu_return: {
      let path = path |> Path.add_sub("return");
      parse__type(~inline=true, ~path, return);
    },
  });
}
/**
  Module
*/
and parse__module =
    (
      ~prepare_only=false,
      ~path,
      (name, declarations): (string, list(Ts.declaration)),
    ) => {
  let path_with_prefix = path |> Path.add(name);

  // Imports need to be parsed always to push things into scope
  let declarations =
    declarations
    |> CCList.filter(
         fun
         | Ts.Import({item: import, _}) => {
             parse__import(~path=path_with_prefix, import);
             false;
           }
         | _ => true,
       );
  Declarations.add_list(~path=path_with_prefix, declarations);
  if (!prepare_only) {
    declarations |> CCList.iter(parse__type_def(~path=path_with_prefix));
  };
}
and parse__import = (~path, {i_from, i_clause}: Ts.declaration_import) => {
  switch (i_clause) {
  | TripleSlashReference
  | ImportModuleSpecifier =>
    parse__direct_reference_module(~path, i_from) |> ignore
  | ImportNamespace({item, _}) =>
    let binding_path =
      path |> Path.add(i_from |> Ident.of_string |> Ident.module_);
    parse__direct_reference_module(~path=binding_path, i_from) |> ignore;
    NamespaceImports.add(~path, (item |> Ident.of_string, binding_path));
  | ImportBinding({item, _}) =>
    let binding_path =
      path |> Path.add(i_from |> Ident.of_string |> Ident.module_);
    let raw_declarations =
      parse__direct_reference_module(~path=binding_path, i_from);
    switch (
      raw_declarations
      |> CCOpt.flat_map(
           CCList.find_opt(
             dec => {
               switch (dec) {
               | Ts.ExportDefault(_) => true
               | _ => false
               }
             },
             _,
           ),
         )
    ) {
    | None =>
      NamespaceImports.add(~path, (item |> Ident.of_string, binding_path))
    | Some(Ts.ExportDefault(IdentifierReference(idref))) =>
      let local = item |> Ident.of_string;
      let remote = idref.item |> Ident.of_string;
      NamedImports.add(~path, [(local, remote, binding_path)]);
    | Some(ExportDefault(default_dec)) =>
      let local = item |> Ident.of_string;
      NamedImports.add(
        ~path,
        [(local, "export_default" |> Ident.of_string, binding_path)],
      );
    | Some(d) =>
      raise(
        Exceptions.Parser_unexpected(
          Printf.sprintf(
            "Unexpected declaration: %s",
            declaration_to_string(d),
          ),
        ),
      )
    };
  | ImportNamed(bindings) =>
    let binding_path =
      path |> Path.add(i_from |> Ident.of_string |> Ident.module_);
    NamedImports.add(
      ~path,
      bindings
      |> CCList.map(
           fun
           | Ts.ImportNamedIdent({item, _}) => {
               let ident = Ident.of_string(item);
               (ident, ident, binding_path);
             }
           | ImportNamedAs(remote, local) =>
             Ident.(
               of_string(local.item),
               of_string(remote.item),
               binding_path,
             ),
         ),
    );
    parse__direct_reference_module(~path=binding_path, i_from) |> ignore;
  | ImportSplitNamed(import, bindings) =>
    parse__import(~path, {i_from, i_clause: ImportBinding(import)});
    parse__import(~path, {i_from, i_clause: ImportNamed(bindings)});
  | ImportSplitNamespace(import, ns) =>
    parse__import(~path, {i_from, i_clause: ImportBinding(import)});
    parse__import(~path, {i_from, i_clause: ImportNamespace(ns)});
  };
}
/**
  Direct reference module
*/
and parse__direct_reference_module =
    (~path, module_specifier: string): option(list(Ts.declaration)) => {
  module Resolver = (val runtime.resolver: Re_typescript_fs.Resolver.T);

  let file =
    Resolver.resolve(~current_file=runtime.current_file, module_specifier);
  let raw_declarations = ref(None);
  switch (file) {
  | Error(e) => raise(Exceptions.File_error(e))
  | Ok((contents, _)) =>
    raw_declarations :=
      Some(runtime.parser(contents) |> CCResult.get_or(~default=[]));
    parse__module(
      ~prepare_only=true,
      ~path,
      ("", raw_declarations^ |> CCOpt.get_exn),
    );
  };
  raw_declarations^;
}

/**
  Entry module
*/
and parse__entry =
    (
      ~ctx: Re_typescript_config.Config.config,
      ~resolver: (module Re_typescript_fs.Resolver.T),
      ~parser: string => CCResult.t(list(Ts.declaration), string),
      entry: Fp.t(Fp.absolute),
    ) => {
  // Cleanup
  runtime.config = ctx;
  runtime.resolver = resolver;
  runtime.parser = parser;
  runtime.current_file = entry;
  Type.clear();
  Ref.clear();
  RefInline.clear();
  Arguments.clear();
  Parameters.clear();

  // Entry
  module Resolver = (val runtime.resolver: Re_typescript_fs.Resolver.T);
  module Loader = (val Resolver.config.loader: Re_typescript_fs.Loader.T);
  let file = Loader.file_read(entry);
  switch (file) {
  | Error(e) => raise(Exceptions.File_error(e))
  | Ok(contents) =>
    parse__type_def(
      Module({
        pi: Parse_info.zero,
        item: (
          "",
          runtime.parser(contents) |> CCResult.get_or(~default=[]),
        ),
      }),
    )
  };

  // Resolve leftover lazies if there are any
  Type.resolve_lazies();
  // Directly manipulates Type & Ref modules
  Tree_optimize.optimize(~ctx);

  (Type.order^, Type.map);
};

open Re_typescript_base;
open Tree_types;
open Tree_utils;
open Tree_data;

let config = ref(Re_typescript_config.default_config);
let current_position = ref(Parse_info.zero);

/**
    Type / Interface / Enum definitions
 */
let rec parse__type_def =
        (~inline=false, ~path=([], []), type_def: Ts.declaration) => {
  current_position := position_of_declaration(type_def);
  switch (type_def) {
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
    let parameters =
      parse__type_parameters(
        ~path=t_path,
        t_parameters |> CCOpt.value(~default=[]),
      );
    Parameters.add(~path=fst(t_path), parameters);
    let t =
      TypeDeclaration({
        td_name: ident,
        td_type: parse__type(~path=t_path, t_type),
        td_parameters: parameters,
      });
    Type.add_order(t_path);
    Type.add(~path=t_path, t);
  | Namespace({pi, item: {n_ident, n_declarations}}) =>
    let x: Ts.with_pi((string, list(Ts.declaration))) = {
      Ts.pi,
      item: (n_ident |> CCList.get_at_idx_exn(0) |> no_pi, n_declarations),
    };
    parse__type_def(~path, Module(x));
  | Module(m) => parse__module(~path, m)
  | Interface({item: {i_ident, i_parameters, i_extends, i_members}, _}) =>
    let ident = i_ident |> Ident.of_pi;
    let t_path = inline ? path : path |> Path.add_ident(ident);

    // Parameters need to be parsed before extension so they can be applied to potentially extended fields
    let parameters =
      parse__type_parameters(
        ~path=t_path,
        i_parameters |> CCOpt.value(~default=[]),
      );
    Parameters.add(~path=fst(t_path), parameters);

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
                       {td_type: Interface(add_fields, _), td_parameters, _} as td,
                     ) =>
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
                         ~type_parameters=Some(td_parameters),
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
      TypeDeclaration({
        td_name: ident,
        td_type:
          switch (parse__type(~path=t_path, Ts.Object(i_members))) {
          | Interface(has_fields, extended) =>
            Interface(
              add_fields
              @ has_fields
              |> CCList.uniq(
                   ~eq=({f_name as ident_a, _}, {f_name as ident_b, _}) =>
                   CCEqual.string(
                     ident_a |> Ident.ident,
                     ident_b |> Ident.ident,
                   )
                 ),
              extended,
            )
          | v => v
          },
        td_parameters: parameters,
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
  let (strings, numbers, _, types) =
    members
    |> CCList.fold_left(
         ((str, num, bool, types)) =>
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
               types @ [parse__type(~inline=true, ~path, n)],
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
and parse__type_parameters = (~path, parameters: Ts.type_parameters) => {
  parameters
  |> CCList.map(({tp_ident, tp_extends, tp_default}: Ts.type_parameter) =>
       (
         {
           let ident = tp_ident |> Ident.of_pi;
           let param_path = Path.(path |> add_sub_ident(ident));
           {
             tp_name: ident,
             tp_extends:
               tp_extends
               |> CCOpt.map(parse__type(~inline=true, ~path=param_path)),
             tp_default:
               tp_default
               |> CCOpt.map(parse__type(~inline=true, ~path=param_path)),
           };
         }: ts_type_parameter
       )
     );
}
/**
  Type extraction
*/
and parse__type_extraction =
    (
      ~from_ref=?,
      ~path,
      type_ref: Ts.type_reference,
      fields: list(list(string)),
    ) => {
  // TODO: Unclean & naively solved, replace later
  let ref_path = fst(type_ref) |> CCList.map(no_pi);
  // let ref_types = snd(type_ref) |> CCOpt.value(~default=[]);
  let resolved_fields =
    switch (
      Ref.resolve_ref(
        ~recursive=true,
        ~from=path,
        from_ref |> CCOpt.value(~default=(ref_path, [])),
      )
      |> CCOpt.flat_map(v => Type.get(~path=v))
    ) {
    | None => []
    | Some(ref_type) =>
      switch (ref_type) {
      | TypeDeclaration({td_type: Interface(add_fields, _), _}) => add_fields
      | _ => []
      }
    };
  switch (fields) {
  | [] => raise(Not_found)
  | [[]] => raise(Not_found)
  | [[name]] =>
    switch (
      resolved_fields
      |> CCList.find_opt(({f_name, _}) =>
           CCEqual.string(f_name |> Ident.value, name)
         )
    ) {
    | Some({
        f_type:
          Reference({tr_path_resolved: Some(tr_path_resolved), _}) as f_type,
        _,
      }) =>
      Ref.add(~from=path, ~to_=tr_path_resolved);
      f_type;
    | Some({f_type, _}) => f_type
    | None =>
      // TODO: Decide wether to to just return any here instead of an error
      Console.error(fields);
      raise(
        Exceptions.Parser_error(
          Printf.sprintf(
            "Code 1 - Could not find field %s in %s",
            name,
            ref_path |> CCList.to_string(~sep="_", a => a),
          ),
        ),
      );
    }
  | [[_, ..._] as field_names] =>
    let field_types =
      field_names
      |> CCList.map(name =>
           parse__type_extraction(~path, type_ref, [[name]])
         );
    Union(
      field_types
      |> CCList.map(t =>
           {um_type: t, um_classifier: "", um_ident: get_union_type_name(t)}
         ),
    );
  | [[name], ...rest] =>
    switch (
      resolved_fields
      |> CCList.find_opt(({f_name, _}) =>
           CCEqual.string(f_name |> Ident.value, name)
         )
    ) {
    | Some({
        f_type: Reference({tr_path_resolved: Some(tr_path_resolved), _}),
        _,
      }) =>
      Ref.add(~from=path, ~to_=tr_path_resolved);
      parse__type_extraction(
        ~from_ref=tr_path_resolved,
        ~path,
        ([], None),
        rest,
      );
    | Some(_)
    | None =>
      raise(
        Exceptions.Parser_error(
          Printf.sprintf(
            "Code 2 - Could not find field %s in %s",
            name,
            ref_path |> CCList.to_string(~sep="_", a => a),
          ),
        ),
      )
    }
  | f =>
    Console.error(f);
    raise(Exceptions.Parser_error("Unknown type extraction structure"));
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
and parse__type_reference = (~path, type_ref: Ts.type_reference) => {
  let ref_path = fst(type_ref) |> CCList.map(no_pi);

  let ref_path_ident = ref_path |> Path.unscoped_to_string |> Ident.of_string;
  switch (Parameters.has_parameter(~path=fst(path), ~param=ref_path_ident)) {
  | Some(_) => parse__type_argument(~path, ref_path_ident)
  | None =>
    let resolved_ref =
      Ref.resolve_ref(
        ~from=
          CCEqual.list(CCEqual.string, fst(path), ref_path)
            ? (fst(path), []) : path,
        (ref_path, []),
      );
    let parameters =
      switch (resolved_ref) {
      | None => []
      | Some(resolved_ref) =>
        parse__apply_ref_arguments(~path, ~resolved_ref, ~type_ref)
      };

    Reference({
      tr_path: ref_path,
      tr_path_resolved: resolved_ref,
      tr_parameters: parameters,
    });
  };
}
/**
  Reference specific argument
*/
and parse__apply_ref_arguments =
    (~path, ~resolved_ref, ~type_ref: Ts.type_reference) => {
  let ref_types = snd(type_ref);

  let ref_applied_types =
    ref_types
    |> CCOpt.map_or(
         ~default=[],
         CCList.mapi((i, param) => {
           let path = path |> Path.add_sub(string_of_int(i));
           parse__type(~inline=true, ~path, param);
         }),
       );

  let resolved_type_parameters = Parameters.get(~path=fst(resolved_ref));
  parse__apply_arguments(
    ~path=resolved_ref,
    ~type_parameters=resolved_type_parameters,
    ~applied_types=ref_applied_types,
  );
}
/**
  Argument resolver
*/
and parse__apply_arguments =
    (
      ~path: Path.t,
      ~type_parameters: option(list(ts_type_parameter)),
      ~applied_types: list(ts_type),
    ) => {
  // --- Duplicate Detection
  let l = CCList.length;
  let param_idents =
    switch (type_parameters) {
    | None => []
    | Some(params) =>
      params |> CCList.map(param => param.tp_name |> Ident.value)
    };
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
          path |> Path.to_string,
        ),
      ),
    );
  };

  // --- Applying
  let (n_required, append_defaults) =
    type_parameters
    |> CCOpt.map_or(~default=(0, []), arg_types => {
         arg_types
         |> CCList.fold_left(
              ((i, lst), arg_type) => {
                switch (lst, arg_type) {
                | ([], {tp_default: None, _}) => (i + 1, lst)
                | (lst, {tp_default: None, _}) =>
                  raise(
                    Exceptions.Parser_parameter_error(
                      "Invalid type reference: Cannot have a required type arg after the first optional",
                    ),
                  )
                | (lst, {tp_name, tp_default: Some(t), _}) => (
                    i,
                    lst @ [t],
                  )
                }
              },
              (0, []),
            )
       });
  let n_required = n_required;
  let n_defaults = l(append_defaults);
  let n_total = n_required + n_defaults;
  let n_applied = l(applied_types);

  // --- Trouble Shooting
  switch (applied_types) {
  | applied when n_applied > 0 && type_parameters |> Option.is_none =>
    Console.warn(
      Printf.sprintf(
        "Invalid type reference: Arguments passed to a type that is not in scope (%s)",
        path |> Path.to_string,
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
          path |> Path.to_string,
        ),
      ),
    )
  | applied when n_applied == n_total => applied
  | applied when n_applied > n_required =>
    applied @ CCList.drop(n_total - n_applied, append_defaults)
  | applied when n_applied == n_required => applied @ append_defaults
  | _ =>
    Console.error(path |> Path.to_string);
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
  Interface(
    members
    |> CCList.map((member: Ts.type_member) =>
         switch (member) {
         | PropertySignature({
             ps_property_name: PIdentifier(ps_ident),
             ps_type_annotation,
             ps_optional,
           }) =>
           let path = path |> Path.add_sub(ps_ident |> no_pi);
           let parsed_type =
             parse__type(~inline=true, ~path, ps_type_annotation);
           {
             f_name: ps_ident |> Ident.of_pi,
             f_type: ps_optional ? Optional(parsed_type) : parsed_type,
           };
         | MethodSignature({
             ms_property_name: PIdentifier(ms_ident),
             ms_call_signature: {
               cs_type_parameters,
               cs_parameter_list,
               cs_type_annotation,
             },
             ms_optional,
           }) =>
           let path = path |> Path.add_sub(ms_ident |> no_pi);
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
             f_name: ms_ident |> Ident.of_pi,
             f_type: ms_optional ? Optional(parsed_type) : parsed_type,
           };
         | PropertySignature(_) as p =>
           raise(
             Exceptions.Parser_unsupported(
               "PropertySignature with anything but PIdentifier not yet supported in interface",
               position_of_type_member(p),
             ),
           )
         | MethodSignature(_) as p =>
           raise(
             Exceptions.Parser_unsupported(
               "MethodSignature with anything but PIdentifier not yet supported in interface",
               position_of_type_member(p),
             ),
           )
         | CallSignature(_) =>
           raise(
             Exceptions.Parser_unsupported(
               "CallSignature not yet supported in interface",
               current_position^,
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
  );
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
  | TypeExtract(type_ref, names) =>
    parse__type_extraction(~path, type_ref, names)
  | Object(members) as t =>
    inline
      ? parse__inline(~path, t)
      : parse__interface(~path, members |> CCOpt.value(~default=[]))
  | Function({f_body, f_ret, _}) as t =>
    inline ? parse__inline(~path, t) : parse__function(~path, f_body, f_ret)
  | Constructor({f_body, f_ret, _}) as t =>
    inline ? parse__inline(~path, t) : parse__function(~path, f_body, f_ret)
  | StringLiteral(_) as t
  | NumberLiteral(_) as t
  | BoolLiteral(_) as t => parse__type(~inline, ~path, Union(t, None))
  | Intersection(left, right) as t =>
    inline
      ? parse__inline(~path, t) : parse__intersection(~path, ~left, ~right)
  | Query([{pi, _}])
  | Query([{pi, _}, _]) =>
    raise(Exceptions.Parser_unsupported("Query type not yet supported", pi))
  | Query(_) =>
    raise(
      Exceptions.Parser_unsupported(
        "Query type not yet supported",
        current_position^,
      ),
    )
  | Symbol(pi) =>
    raise(Exceptions.Parser_unsupported("Symbol type not yet supported", pi))
  | This(pi) =>
    raise(Exceptions.Parser_unsupported("This type not yet supported", pi))
  };
}
/**
  Inline Wrapper
*/
and parse__inline = (~path, type_) => {
  parse__type_def(
    ~inline=true,
    ~path,
    Ts.Type({
      pi: Parse_info.zero,
      item: {
        t_ident: {
          pi: Parse_info.zero,
          item: "",
        },
        t_parameters: None,
        t_type: type_,
      },
    }),
  );
  let args =
    switch (Arguments.get(~path)) {
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
  switch (config^.intersection_mode) {
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
        "No intersection mode other than enum is supported yet",
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
      ~path,
      {item: (name, declarations), _}:
        Ts.with_pi((string, list(Ts.declaration))),
    ) => {
  let path_with_prefix = path |> Path.add(name);
  declarations |> CCList.iter(parse__type_def(~path=path_with_prefix));
}
/**
  Entry module
*/
and parse__entry_module =
    (
      ~ctx: Re_typescript_config.Config.config,
      declarations: list(Ts.declaration),
    ) => {
  // Cleanup
  config := ctx;
  Type.clear();
  Ref.clear();
  Arguments.clear();
  Parameters.clear();

  // Entry
  parse__type_def(Module({pi: Parse_info.zero, item: ("", declarations)}));

  // Directly manipulates Type & Ref modules
  Tree_optimize.optimize(~ctx);

  (Type.order^, Type.map);
};

open Re_typescript_base;
open Tree_types;
open Tree_utils;
open Tree_data;

/**
    Type / Interface / Enum definitions
 */
let rec parse__type_def =
        (~inline=false, ~path=([], []), type_def: Ts.type_def) => {
  switch (type_def) {
  | `TypeDef(name, type_, args) =>
    let ident = name |> Ident.of_string;
    let t_path = inline ? path : path |> Path.add_ident(ident);
    let arguments = parse__type_args(~path=t_path, args);
    Arguments.add(~path=fst(t_path), arguments);
    let t =
      TypeDeclaration({
        td_name: ident,
        td_type: parse__type(~path=t_path, type_),
        td_arguments: arguments,
      });
    Type.add_order(t_path);
    Type.add(~path=t_path, t);
  | `Module(m) => parse__module(~path, m)
  | `InterfaceDef(name, extends, fields, args) =>
    let ident = name |> Ident.of_string;
    let t_path = inline ? path : path |> Path.add_ident(ident);

    // TODO: Unclean & naively solved, replace later
    let (ref_path, ref_types) = CCList.split(extends);
    let add_fields =
      switch (
        Ref.resolve_ref(~from=path, (ref_path, []))
        |> CCOpt.flat_map(v => Type.get(~path=v))
      ) {
      | None => []
      | Some(ref_type) =>
        switch (ref_type) {
        | TypeDeclaration({td_type: Interface(add_fields), _}) => add_fields
        | _ => []
        }
      };

    let arguments = parse__type_args(~path=t_path, args);
    Arguments.add(~path=fst(t_path), arguments);

    let t =
      TypeDeclaration({
        td_name: ident,
        td_type:
          switch (parse__type(~path=t_path, `Obj(fields))) {
          | Interface(has_fields) =>
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
            )
          | v => v
          },
        td_arguments: arguments,
      });
    Type.add_order(t_path);
    Type.add(~path=t_path, t);
  | `EnumDef(name, members, is_const) =>
    let ident = name |> Ident.of_string;
    let t_path = inline ? path : path |> Path.add_ident(ident);
    let t =
      TypeDeclaration({
        td_name: ident,
        td_type: parse__type(~path=t_path, `Enum((members, is_const))),
        td_arguments: [],
      });
    Type.add_order(t_path);
    Type.add(~path=t_path, t);
  };
}
/**
    Unions
 */
and parse__union = (~path, members) => {
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
and parse__union_type = (~path, members: list(Ts.union_member)) => {
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
and parse__mixed_literal = (members: list(Ts.union_member)) => {
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
and parse__numeric_literal = (members: list(Ts.union_member)) => {
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
and parse__string_literal = (members: list(Ts.union_member)) => {
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
and parse__union_undefined = (~path, members: list(Ts.union_member)) => {
  let extract_undefined =
    members
    |> CCListLabels.fold_left(
         ~f=
           ((has_undefined, p), member) => {
             switch (member) {
             | `U_Type(`Undefined) => (true, p)
             | member => (has_undefined, [member, ...p])
             }
           },
         ~init=(false, []),
       );
  switch (extract_undefined) {
  | (true, [`U_Type(type_)]) => Some(parse__optional(~path, type_))
  | (true, members) => Some(parse__optional(~path, `Union(members)))
  | (false, _) => None
  };
}
and parse__union_nullable = (~path, members: list(Ts.union_member)) => {
  let extract_null =
    members
    |> CCListLabels.fold_left(
         ~f=
           ((has_null, p), member) => {
             switch (member) {
             | `U_Type(`Null) => (true, p)
             | member => (has_null, [member, ...p])
             }
           },
         ~init=(false, []),
       );
  switch (extract_null) {
  | (true, [`U_Type(type_)]) => Some(parse__nullable(~path, type_))
  | (true, members) => Some(parse__nullable(~path, `Union(members)))
  | (false, _) => None
  };
}
/**
    Type arguments
 */
and parse__type_args = (~path, args: list(Ts.type_arg)) => {
  args
  |> CCList.map(({a_name, a_constraint_, a_default}: Ts.type_arg) =>
       (
         {
           let ident = a_name |> Ident.of_string;
           let arg_path = Path.(path |> add_sub_ident(ident));
           {
             tda_name: ident,
             tda_extends:
               a_constraint_
               |> CCOpt.map(parse__type(~inline=true, ~path=arg_path)),
             tda_default:
               a_default
               |> CCOpt.map(parse__type(~inline=true, ~path=arg_path)),
           };
         }: ts_type_argument
       )
     );
}
/**
    Type extraction
 */
and parse__type_extraction =
    (~from_ref=?, ~path, type_ref: Ts.ref_, fields: list(list(string))) => {
  // TODO: Unclean & naively solved, replace later
  let (ref_path, ref_types) = CCList.split(type_ref);
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
      | TypeDeclaration({td_type: Interface(add_fields), _}) => add_fields
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
      parse__type_extraction(~from_ref=tr_path_resolved, ~path, [], rest);
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
and parse__type_reference = (~path, type_ref: Ts.ref_) => {
  let (ref_path, _) = CCList.split(type_ref);

  let ref_path_ident = ref_path |> Path.unscoped_to_string |> Ident.of_string;
  switch (Arguments.has_argument(~path=fst(path), ~arg=ref_path_ident)) {
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
        parse__apply_ref_parameters(~path, ~resolved_ref, ~type_ref)
      };

    Reference({
      tr_path: ref_path,
      tr_path_resolved: resolved_ref,
      tr_parameters: parameters,
    });
  };
}
and parse__apply_ref_parameters = (~path, ~resolved_ref, ~type_ref) => {
  let (_, ref_types) = CCList.split(type_ref);

  let ref_applied_types =
    ref_types
    |> CCList.last_opt
    |> CCOpt.map_or(
         ~default=[],
         CCList.mapi((i, param) => {
           let path = path |> Path.add_sub(string_of_int(i));
           parse__type(~inline=true, ~path, param);
         }),
       );

  let resolved_type_args = Arguments.get(~path=fst(resolved_ref));
  let (n_required, append_defaults) =
    resolved_type_args
    |> CCOpt.map_or(~default=(0, []), arg_types => {
         arg_types
         |> CCList.fold_left(
              ((i, lst), arg_type) => {
                switch (lst, arg_type) {
                | ([], {tda_default: None, _}) => (i + 1, lst)
                | (lst, {tda_default: None, _}) =>
                  raise(
                    Exceptions.Parser_error(
                      "Invalid type reference: Cannot have a required type arg after the first optional",
                    ),
                  )
                | (lst, {tda_name, tda_default: Some(t), _}) => (
                    i,
                    lst @ [t],
                  )
                }
              },
              (0, []),
            )
       });
  let l = CCList.length;
  let n_required = n_required;
  let n_defaults = l(append_defaults);
  let n_total = n_required + n_defaults;
  let n_applied = l(ref_applied_types);

  switch (ref_applied_types) {
  | applied when n_applied > 0 && resolved_type_args |> Option.is_none =>
    Console.warn(
      Printf.sprintf(
        "Invalid type reference: Arguments passed to a type that is not in scope (%s)",
        resolved_ref |> Path.to_string,
      ),
    );
    applied;
  | applied when n_applied > n_total || n_applied < n_required =>
    raise(
      Exceptions.Parser_error(
        Printf.sprintf(
          "Invalid type reference: Applied %d arguments to a type where a minimum of %d and a maxiumum of %d are expected (%s)",
          n_applied,
          n_required,
          n_total,
          resolved_ref |> Path.to_string,
        ),
      ),
    )
  | applied when n_applied == n_total => applied
  | applied when n_applied > n_required =>
    applied @ CCList.drop(n_total - n_applied, append_defaults)
  | applied when n_applied == n_required => applied @ append_defaults
  | _ =>
    Console.error(resolved_ref |> Path.to_string);
    Console.error([n_required, n_total, n_applied]);
    raise(Exceptions.Parser_error("This should not be reachable"));
  };
}
and parse__type_argument = (~path, arg: ts_identifier) => {
  Arg(arg);
}
/**
    Enums
 */
and parse__enum = (~path, members: list(Ts.enum_field), is_const: bool) => {
  let is_clean =
    members
    |> CCList.for_all((member: Ts.enum_field) =>
         !(member.Ts.default |> CCOpt.is_some)
       );
  if (is_clean) {
    Enum(
      members
      |> CCList.map((member: Ts.enum_field) => member.key)
      |> CCList.map(Ident.of_string),
    );
  } else {
    raise(Exceptions.Parser_error("Unclean enums are not yet supported"));
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
and parse__interface = (~path, fields: list(Ts.obj_field)) => {
  Interface(
    fields
    |> CCList.map(({f_key, f_type_, f_optional, f_readonly}: Ts.obj_field) => {
         let path = path |> Path.add_sub(f_key);
         let parsed_type = parse__type(~inline=true, ~path, f_type_);
         {
           f_name: f_key |> Ident.of_string,
           f_type: f_optional ? Optional(parsed_type) : parsed_type,
           f_readonly,
         };
       }),
  );
}
/**
    All types
 */
and parse__type = (~inline=false, ~path, type_: Ts.type_) => {
  switch (type_) {
  | `String => Base(String)
  | `Number => Base(Number)
  | `Boolean => Base(Boolean)
  | `Void => Base(Void)
  | `Any => Base(Any)
  | `Null => Base(Null)
  | `Undefined => Base(Undefined)
  | `Enum(members, is_const) => parse__enum(~path, members, is_const)
  | `Union(members) as t =>
    inline ? parse__inline(~path, t) : parse__union(~path, members)
  | `Tuple(types) as t =>
    inline ? parse__inline(~path, t) : parse__tuple(~path, types)
  | `Array(t) => parse__array(~path, t)
  | `Ref(ref_) => parse__type_reference(~path, ref_)
  | `TypeExtract(type_ref, names) =>
    parse__type_extraction(~path, type_ref, names)
  | `Obj(fields) as t =>
    inline ? parse__inline(~path, t) : parse__interface(~path, fields)
  | `Function(args, return) as t =>
    inline ? parse__inline(~path, t) : parse__function(~path, args, return)
  };
}
and parse__inline = (~path, type_) => {
  parse__type_def(~inline=true, ~path, `TypeDef(("", type_, [])));
  Ref.resolve_ref(~from=path, path) |> ignore;
  Reference({
    tr_path: fst(path),
    tr_path_resolved: Some(path),
    tr_parameters: [],
  });
}
/**
  Function
 */
and parse__function =
    (~path, args: list(Ts.function_arg), return: option(Ts.type_)) => {
  Function({
    fu_params:
      args
      |> CCList.map((arg: Ts.function_arg) => {
           let path = path |> Path.add_sub(arg.fa_name);
           {
             fp_name: arg.fa_name |> Ident.of_string,
             fp_type:
               arg.fa_type
               |> CCOpt.map_or(
                    ~default=Base(Any),
                    parse__type(~inline=true, ~path),
                  ),
             fp_optional: arg.fa_optional,
           };
         }),
    fu_return:
      return
      |> CCOpt.map_or(
           ~default=Base(Any),
           td => {
             let path = path |> Path.add_sub("return");
             parse__type(~inline=true, ~path, td);
           },
         ),
  });
}
/**
    Module
 */
and parse__module = (~path, module_: Ts.module_) => {
  module_.types
  |> CCList.iter(parse__type_def(~path=path |> Path.add(module_.name)));
}
/**
    Entry module
 */
and parse__entry_module = (module_: Ts.module_) => {
  Type.clear();
  Ref.clear();
  parse__type_def(`Module({...module_, name: ""}));

  // Directly manipulates Type & Ref modules
  Tree_optimize.optimize();

  (Type.order^, Type.map);
};

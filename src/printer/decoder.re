open Re_typescript_base;
open Decode_result;
open Decode_config;
open Decode_utils;
open Utils;

exception Decode_Error(string);

/**
  Utils
 */
let types: ref(list(Ts.type_def)) = ref([]);
let record_cache: Hashtbl.t(string, Decode_result.type_def) =
  Hashtbl.create(0);
let injected: Hashtbl.t(string, Decode_result.type_def) = Hashtbl.create(0);
let record_referenced = ref([]);
let has_been_referenced = name =>
  record_referenced^ |> List.find_opt(x => x == name) |> CCOpt.is_some;
let type_arg_ref: Hashtbl.t(string, (list(string), list(type_arg))) =
  Hashtbl.create(0);

// TODO: Bring the lex location information along with the Ts.* to throw meaningful errors here
// TODO: Implement a reference / dependency tree that is able hoist certain types up if necessary & avoids duplication of unnecessary types
// TODO: Properly keep track of the names of types and their dynamically generated subtypes
// TODO: Bring inline types closer to their call site

/**
  Functions
 */
let rec decode = (~ctx: config=default_config, toplevel: Ts.toplevel) => {
  Hashtbl.clear(injected);
  Hashtbl.clear(type_arg_ref);
  types := toplevel.types |> CCList.map(fst);
  record_referenced := [];
  let types = toplevel.types |> CCList.map(decode_type_def);

  types
  |> CCListLabels.append(
       Hashtbl.fold((_, v, p) => [v, ...p], injected, [])
       |> CCListLabels.rev_map(
            ~f=
              fun
              | TypeDeclaration(_) as type_dec => type_dec
              | _ =>
                raise(Decode_Error("Only TypeDeclarations can be injected")),
          )
       |> CCListLabels.rev,
     )
  |> CCListLabels.filter_map(
       ~f=
         fun
         | TypeDeclaration((name, _) as n, Record([]), _)
             when has_been_referenced(name) =>
           Some(TypeDeclaration(n, Base(Any), []))
         | TypeDeclaration(_, Record([]), _) => None
         | v => Some(v),
     );
}
and decode_type_def: ((Ts.type_def, bool)) => type_def =
  fun
  | (`TypeDef(name, `Obj(fields), args), _) => {
      let name = name |> to_valid_typename;
      let (arg_names, args) = decode_type_args(~name=fst(name), args);
      let record =
        Record(
          fields
          |> CCList.map(
               decode_obj_field(~parent_name=name, ~available_args=arg_names),
             ),
        );
      Hashtbl.add(record_cache, fst(name), record);

      TypeDeclaration(name, record, args);
    }
  | (`InterfaceDef(name, extends_ref, fields, args), _) => {
      let name = name |> to_valid_typename;
      let (arg_names, args) = decode_type_args(~name=fst(name), args);
      let record =
        Record(
          fields
          |> CCList.map(
               decode_obj_field(~parent_name=name, ~available_args=arg_names),
             )
          |> CCList.append(
               decode_extends_ref(
                 ~parent_name=name,
                 ~available_args=arg_names,
                 extends_ref,
               ),
             )
          |> CCListLabels.uniq(~eq=(a, b) =>
               switch (a, b) {
               | (RecordField((a, _), _, _), RecordField((b, _), _, _)) =>
                 a == b
               | _ => false
               }
             )
          |> CCList.rev,
        );
      Hashtbl.add(record_cache, fst(name), record);

      TypeDeclaration(name, record, args);
    }
  | (`TypeDef(name, type_, args), _) => {
      let name = name |> to_valid_typename;
      let (arg_names, args) = decode_type_args(~name=fst(name), args);
      TypeDeclaration(
        name,
        decode_type(~parent_name=name, ~available_args=arg_names, type_),
        args,
      );
    }
  | (`EnumDef(name, members, is_const), _) =>
    TypeDeclaration(
      name |> to_valid_typename,
      decode_enum(members, is_const),
      [],
    )
and decode_type_args = (~name: string, args: list(Ts.type_arg)) => {
  let names = args |> CCList.map((v: Ts.type_arg) => v.name);
  CCHashtbl.get_or_add(type_arg_ref, ~k=name, ~f=_ =>
    args
    |> CCList.map((arg: Ts.type_arg) => {
         let parent_name = name |> to_valid_typename;
         (
           arg.name,
           {
             name: arg.name,
             default:
               arg.default
               |> CCOpt.map(
                    fun
                    | `Obj(fields) =>
                      decode_inline_record(
                        ~parent_name,
                        ~available_args=names,
                        ~key=arg.name,
                        ~fields,
                      )
                    | t =>
                      decode_type(~parent_name, ~available_args=names, t),
                  ),
           },
         );
       })
    |> CCList.split
  );
}
and decode_enum = (members, is_const) => {
  let is_clean =
    members
    |> CCList.for_all((member: Ts.enum_field) =>
         !(member.Ts.default |> CCOpt.is_some)
       );
  if (is_clean) {
    VariantEnum(
      members
      |> CCListLabels.map(~f=(member: Ts.enum_field) => member.key)
      |> CCListLabels.map(~f=to_valid_variant_constructor),
    );
  } else {
    raise(Decode_Error("Unclean enums are not yet supported"));
  };
}
and decode_type:
  (
    ~parent_name: (string, string),
    ~available_args: list(string),
    Ts.type_
  ) =>
  type_def =
  (~parent_name, ~available_args) =>
    fun
    | `String => Base(String)
    | `Number => Base(Number)
    | `Boolean => Base(Boolean)
    | `Void => Base(Void)
    | `Any => Base(Any)
    | `Array(type_) => decode_array(~parent_name, ~available_args, type_)
    | `Tuple(types) => decode_tuple(~parent_name, ~available_args, types)
    | `Ref(ref_) => {
        let decoded_ref =
          decode_ref_type_name(~parent_name, ~available_args, ref_);
        let decoded_name = decoded_ref |> fst |> snd;
        switch (available_args |> CCList.find_opt(arg => arg == decoded_name)) {
        | Some(_) => Base(Arg(decoded_name))
        | None => Base(Ref(fst(decoded_ref), snd(decoded_ref)))
        };
      }
    | `Union(members) => decode_union(~parent_name, ~available_args, members)
    | `Undefined =>
      raise(Decode_Error("Undefined cannot exist outside of a union"))
    | `Null => raise(Decode_Error("Null cannot exist outside of a union"))
    | `Obj(o) => {
        Console.error(o);
        raise(Decode_Error("Obj should never be reached in this switch"));
      }
    | `TypeExtract(ref_, names) =>
      decode_type_extract(~parent_name, ~available_args, ref_, names)
and decode_union = (~parent_name, ~available_args, members) => {
  // TODO: Switch on references in the simple unions to maybe get the same type
  switch (decode_union_undefined(~parent_name, ~available_args, members)) {
  | Some(t) => t
  | None =>
    switch (decode_union_nullable(~parent_name, ~available_args, members)) {
    | Some(t) => t
    | None =>
      switch (decode_union_string(members)) {
      | Some(t) => t
      | None =>
        switch (decode_union_int(members)) {
        | Some(t) => t
        | None =>
          switch (decode_union_mixed(members)) {
          | Some(t) => t
          | None => decode_union_type(~parent_name, ~available_args, members)
          }
        }
      }
    }
  };
}
and decode_union_type =
    (~parent_name, ~available_args, members: list(Ts.union_member)) => {
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
               types @ [decode_type(~parent_name, ~available_args, n)],
             ),
         ([], [], false, []),
       );

  let types =
    types
    @ (
      strings
      |> list_to_opt
      |> CCOpt.flat_map(decode_union_string)
      |> CCOpt.to_list
    )
    @ (
      numbers
      |> list_to_opt
      |> CCOpt.flat_map(decode_union_int)
      |> CCOpt.to_list
    );
  Union(
    types
    |> CCList.map(t =>
         {um_type: t, um_classifier: "", um_name: decode_union_type_name(t)}
       ),
  );
}
and decode_union_type_name = (um_type: type_def) => {
  switch (um_type) {
  | Base(String) => "string"
  | Base(Number) => "number"
  | Base(Boolean) => "boolean"
  | Base(Void) => "void"
  | Base(Any) => "any"
  | Base(Ref((n, _), _)) => n
  | Base(Arg(_)) => "inferred"
  | Optional(t)
  | Nullable(t) => decode_union_type_name(t)
  | Array(t) => Printf.sprintf("array_%s", decode_union_type_name(t))
  | Tuple(_) => "tuple"
  | VariantMixed(_) => "variant"
  | VariantInt(_) => "num"
  | VariantString(_) => "literal"
  | VariantEnum(_) =>
    raise(Decode_Error("Union is not a valid union member"))
  | Union(_) => raise(Decode_Error("Union is not a valid union member"))
  | Record(_) => raise(Decode_Error("Record is not a valid union member"))
  | RecordField(_) =>
    raise(Decode_Error("Record is not a valid union member"))
  | TypeDeclaration(_) =>
    raise(Decode_Error("TypeDeclaration is not a valid union member"))
  };
}
and decode_union_mixed = (members: list(Ts.union_member)) => {
  exception No_union_mixed;
  try(
    Some(
      VariantMixed(
        members
        |> CCList.map(
             fun
             | `U_String(n) => U_String(n)
             | `U_Number(n) => U_Number(n)
             | `U_Bool(n) => U_Bool(n)
             | _ => raise(No_union_mixed),
           ),
      ),
    )
  ) {
  | No_union_mixed => None
  | e => raise(e)
  };
}
and decode_union_int = (members: list(Ts.union_member)) => {
  exception No_union_number;
  try(
    Some(
      VariantInt(
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
and decode_union_string = (members: list(Ts.union_member)) => {
  exception No_union_string;
  try(
    Some(
      VariantString(
        members
        |> CCListLabels.map(
             ~f=
               fun
               | `U_String(str) => str
               | _ => raise(No_union_string),
           ),
      ),
    )
  ) {
  | No_union_string => None
  | e => raise(e)
  };
}
and decode_union_nullable =
    (~parent_name, ~available_args, members: list(Ts.union_member)) => {
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
  | (true, [`U_Type(type_)]) =>
    Some(Nullable(decode_type(~parent_name, ~available_args, type_)))
  | (true, members) =>
    Some(Nullable(decode_union(~parent_name, ~available_args, members)))
  | (false, _) => None
  };
}
and decode_union_undefined =
    (~parent_name, ~available_args, members: list(Ts.union_member)) => {
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
  | (true, [`U_Type(type_)]) =>
    Some(Optional(decode_type(~parent_name, ~available_args, type_)))
  | (true, members) =>
    Some(Optional(decode_union(~parent_name, ~available_args, members)))
  | (false, _) => None
  };
}
and decode_array = (~parent_name, ~available_args, type_) => {
  Array(
    switch (type_) {
    | `Obj(fields) =>
      decode_inline_record(~parent_name, ~available_args, ~key="0", ~fields)
    | t => decode_type(~parent_name, ~available_args, t)
    },
  );
}
and decode_tuple = (~parent_name, ~available_args, types) => {
  Tuple(
    types
    |> CCListLabels.mapi(~f=i =>
         fun
         | `Obj(fields) =>
           decode_inline_record(
             ~parent_name,
             ~available_args,
             ~key=i + 1 |> string_of_int,
             ~fields,
           )
         | t => decode_type(~parent_name, ~available_args, t)
       ),
  );
}
and decode_inline_record = (~parent_name, ~available_args, ~key, ~fields) => {
  let sub_type_name =
    Printf.sprintf("%s_%s", fst(parent_name), key |> to_valid_ident |> fst);
  let record =
    Record(
      fields
      |> CCListLabels.map(
           ~f=
             decode_obj_field(
               ~parent_name=(sub_type_name, sub_type_name),
               ~available_args,
             ),
         ),
    );
  Hashtbl.add(
    injected,
    sub_type_name,
    TypeDeclaration((sub_type_name, sub_type_name), record, []),
  );
  Hashtbl.add(record_cache, sub_type_name, record);
  Base(Ref((sub_type_name, sub_type_name), []));
}
and decode_obj_field = (~parent_name, ~available_args) =>
  fun
  | {key, type_: `Obj(fields), optional, readonly} => {
      let t_ref =
        decode_inline_record(~parent_name, ~available_args, ~key, ~fields);
      RecordField(
        key |> to_valid_ident,
        optional ? Optional(t_ref) : t_ref,
        readonly,
      );
    }
  | {key, optional: true, readonly, type_} => {
      RecordField(
        key |> to_valid_ident,
        Optional(type_ |> decode_type(~parent_name, ~available_args)),
        readonly,
      );
    }
  | {key, optional: false, readonly, type_} =>
    RecordField(
      key |> to_valid_ident,
      type_ |> decode_type(~parent_name, ~available_args),
      readonly,
    )
and decode_type_extract =
    (~parent_name, ~available_args, ref_: Ts.ref_, fields: list(string)) => {
  let fields_in_type =
    decode_extends_ref(~parent_name, ~available_args, ref_);
  let find_field = name =>
    fields_in_type
    |> CCListLabels.find_opt(
         ~f=
           fun
           | RecordField((n, _), _, _) when n == name => true
           | _ => false,
       );
  switch (fields) {
  | [] => raise(Not_found)
  | [name] =>
    switch (find_field(name |> to_valid_typename |> fst)) {
    | Some(RecordField(_, type_, _)) => type_
    | Some(_)
    | None =>
      // TODO: Decide wether to to just return any here instead of an error
      Console.error(fields);
      raise(
        Decode_Error(
          Printf.sprintf(
            "Could not find field %s in %s",
            name,
            decode_ref_type_name(~parent_name, ~available_args, ref_)
            |> fst
            |> snd,
          ),
        ),
      );
    }
  | [name, ...rest] =>
    switch (find_field(name |> to_valid_typename |> fst)) {
    | Some(RecordField(_, Base(Ref((ref_name, _), _)), _)) =>
      decode_type_extract(
        ~parent_name,
        ~available_args,
        [(ref_name, [])],
        rest,
      )
    | Some(_)
    | None =>
      // TODO: Decide wether to to just return any here instead of an error
      Console.error(fields);
      raise(
        Decode_Error(
          Printf.sprintf(
            "Could not find field %s in %s",
            name,
            decode_ref_type_name(~parent_name, ~available_args, ref_)
            |> fst
            |> snd,
          ),
        ),
      );
    }
  };
}
and decode_extends_ref = (~parent_name, ~available_args, ref_: Ts.ref_) => {
  // Only implement this naively for now
  let lookup_name =
    decode_ref_type_name(~parent_name, ~available_args, ref_) |> fst |> fst;
  CCOpt.(
    Hashtbl.find_opt(record_cache, lookup_name)
    |> map(
         fun
         | Record(fields) => fields
         | _ => [],
       )
    |> get_or(~default=[])
  );
}
and decode_ref_type_name =
    (~mark_referenced=true, ~parent_name, ~available_args, ref_: Ts.ref_)
    : ((string, string), list(type_def)) => {
  let (ref_names, ref_types) = CCList.split(ref_);
  let ref_resolved =
    ref_names |> CCList.to_string(~sep=".", v => v) |> to_valid_typename;

  if (mark_referenced) {
    record_referenced := [fst(ref_resolved), ...record_referenced^];
  };

  let ref_applied_types =
    ref_types
    |> CCList.last_opt
    |> CCOpt.map(args =>
         args
         |> CCList.mapi(i =>
              fun
              | `Obj(fields) =>
                decode_inline_record(
                  ~parent_name=ref_resolved,
                  ~available_args,
                  ~key=string_of_int(i),
                  ~fields,
                )
              | t =>
                decode_type(~parent_name=ref_resolved, ~available_args, t)
            )
       )
    |> CCOpt.get_or(~default=[]);

  let resolved_type_args = CCHashtbl.get(type_arg_ref, fst(ref_resolved));
  let (n_required, append_defaults) =
    resolved_type_args
    |> CCOpt.map_or(~default=(0, []), ((_, arg_types)) => {
         arg_types
         |> CCList.fold_left(
              ((i, lst), arg_type) => {
                switch (lst, arg_type) {
                | ([], {default: None, _}) => (i + 1, lst)
                | (lst, {default: None, _}) =>
                  raise(
                    Decode_Error(
                      "Invalid type reference: Cannot have a required type arg after the first optional",
                    ),
                  )
                | (lst, {name, default: Some(t), _}) => (i, lst @ [t])
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

  let ref_applied_types =
    switch (ref_applied_types) {
    | applied when n_applied > 0 && resolved_type_args |> Option.is_none =>
      Console.warn(
        Printf.sprintf(
          "Invalid type reference: Arguments passed to a type that is not in scope (%s, %s)",
          fst(ref_resolved),
          snd(ref_resolved),
        ),
      );
      applied;
    | applied when n_applied > n_total || n_applied < n_required =>
      raise(
        Decode_Error(
          Printf.sprintf(
            "Invalid type reference: Applied %d arguments to a type where a minimum of %d and a maxiumum of %d are expected",
            n_applied,
            n_required,
            n_total,
          ),
        ),
      )
    | applied when n_applied == n_total => applied
    | applied when n_applied > n_required =>
      applied @ CCList.drop(n_total - n_applied, append_defaults)
    | applied when n_applied == n_required => applied @ append_defaults
    | _ =>
      Console.error(ref_resolved);
      Console.error([n_required, n_total, n_applied]);
      raise(Decode_Error("This should not be reachable"));
    };

  (ref_resolved, ref_applied_types);
};

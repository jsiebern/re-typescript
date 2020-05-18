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

// TODO: Add a type arg map and validate it against it resolving references
// TODO: Bring the lex location information along with the Ts.* to throw meaningful errors here

/**
  Functions
 */
let rec decode = (~ctx: config=default_config, toplevel: Ts.toplevel) => {
  Hashtbl.clear(injected);
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
      let record =
        Record(
          fields |> CCList.map(decode_obj_field(~parent_name=name, ~args)),
        );
      Hashtbl.add(record_cache, fst(name), record);
      TypeDeclaration(name, record, decode_type_args(args));
    }
  | (`InterfaceDef(name, extends_ref, fields, args), _) => {
      let name = name |> to_valid_typename;
      let record =
        Record(
          fields
          |> CCList.map(decode_obj_field(~parent_name=name, ~args))
          |> CCList.append(decode_extends_ref(extends_ref))
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
      TypeDeclaration(name, record, decode_type_args(args));
    }
  | (`TypeDef(name, type_, args), _) => {
      let name = name |> to_valid_typename;
      TypeDeclaration(
        name,
        decode_type(~parent_name=name, type_),
        decode_type_args(args),
      );
    }
  | (`EnumDef(name, members, is_const), _) =>
    TypeDeclaration(
      name |> to_valid_typename,
      decode_enum(members, is_const),
      [],
    )
and decode_type_args = (args: list(Ts.type_arg)) => {
  args |> CCList.map((arg: Ts.type_arg) => {name: arg.name});
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
and decode_type: (~parent_name: (string, string), Ts.type_) => type_def =
  (~parent_name) =>
    fun
    | `String => Base(String)
    | `Number => Base(Number)
    | `Boolean => Base(Boolean)
    | `Void => Base(Void)
    | `Any => Base(Any)
    | `Array(type_) => decode_array(~parent_name, type_)
    | `Tuple(types) => decode_tuple(~parent_name, types)
    | `Ref(ref_) => Base(Ref(ref_ |> decode_ref_type_name))
    | `Union(members) => decode_union(~parent_name, members)
    | `Undefined =>
      raise(Decode_Error("Undefined cannot exist outside of a union"))
    | `Null => raise(Decode_Error("Null cannot exist outside of a union"))
    | `Obj(_) =>
      raise(Decode_Error("Obj should never be reached in this switch"))
    | `TypeExtract(ref_, names) => decode_type_extract(ref_, names)
and decode_union = (~parent_name, members) => {
  switch (decode_union_undefined(~parent_name, members)) {
  | Some(t) => t
  | None =>
    switch (decode_union_nullable(~parent_name, members)) {
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
          | None =>
            Console.error(members);
            raise(Decode_Error("Complex unions are not yet implemented"));
          }
        }
      }
    }
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
and decode_union_nullable = (~parent_name, members: list(Ts.union_member)) => {
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
    Some(Nullable(decode_type(~parent_name, type_)))
  | (true, members) => Some(Nullable(decode_union(~parent_name, members)))
  | (false, _) => None
  };
}
and decode_union_undefined = (~parent_name, members: list(Ts.union_member)) => {
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
    Some(Optional(decode_type(~parent_name, type_)))
  | (true, members) => Some(Optional(decode_union(~parent_name, members)))
  | (false, _) => None
  };
}
and decode_array = (~parent_name, type_) => {
  Array(
    switch (type_) {
    | `Obj(fields) => decode_inline_record(~parent_name, ~key="t", ~fields)
    | t => decode_type(~parent_name, t)
    },
  );
}
and decode_tuple = (~parent_name, types) => {
  Tuple(
    types
    |> CCListLabels.mapi(~f=i =>
         fun
         | `Obj(fields) =>
           decode_inline_record(
             ~parent_name,
             ~key=i + 1 |> string_of_int,
             ~fields,
           )
         | t => decode_type(~parent_name, t)
       ),
  );
}
and decode_inline_record = (~parent_name, ~key, ~fields) => {
  let sub_type_name =
    Printf.sprintf("%s_%s", fst(parent_name), key |> to_valid_ident |> fst);
  let record =
    Record(
      fields
      |> CCListLabels.map(
           ~f=
             decode_obj_field(
               ~parent_name=(sub_type_name, sub_type_name),
               ~args=[],
             ),
         ),
    );
  Hashtbl.add(
    injected,
    sub_type_name,
    TypeDeclaration((sub_type_name, sub_type_name), record, []),
  );
  Hashtbl.add(record_cache, sub_type_name, record);
  Base(Ref((sub_type_name, sub_type_name)));
}
and decode_obj_field = (~parent_name, ~args: list(Ts.type_arg)) =>
  fun
  | {key, type_: `Obj(fields), optional, readonly} => {
      let t_ref = decode_inline_record(~parent_name, ~key, ~fields);
      RecordField(
        key |> to_valid_ident,
        optional ? Optional(t_ref) : t_ref,
        readonly,
      );
    }
  | {key, optional: true, readonly, type_} => {
      RecordField(
        key |> to_valid_ident,
        Optional(type_ |> decode_type(~parent_name)),
        readonly,
      );
    }
  | {key, optional: false, readonly, type_} =>
    RecordField(
      key |> to_valid_ident,
      switch (type_) {
      | `Ref(ref_) =>
        let resolved_ref =
          ref_ |> decode_ref_type_name(~mark_referenced=false) |> snd;
        switch (
          args
          |> CCList.find_opt((arg: Ts.type_arg) => arg.name == resolved_ref)
        ) {
        | Some(_) => Base(Arg(resolved_ref))
        | None => type_ |> decode_type(~parent_name)
        };
      | type_ => type_ |> decode_type(~parent_name)
      },
      readonly,
    )
and decode_type_extract = (ref_: Ts.ref_, fields: list(string)) => {
  let fields_in_type = decode_extends_ref(ref_);
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
            decode_ref_type_name(ref_) |> snd,
          ),
        ),
      );
    }
  | [name, ...rest] =>
    switch (find_field(name |> to_valid_typename |> fst)) {
    | Some(RecordField(_, Base(Ref((ref_name, _))), _)) =>
      decode_type_extract([(ref_name, [])], rest)
    | Some(_)
    | None =>
      // TODO: Decide wether to to just return any here instead of an error
      Console.error(fields);
      raise(
        Decode_Error(
          Printf.sprintf(
            "Could not find field %s in %s",
            name,
            decode_ref_type_name(ref_) |> snd,
          ),
        ),
      );
    }
  };
}
and decode_extends_ref = (ref_: Ts.ref_) => {
  // Only implement this naively for now
  let lookup_name = fst(decode_ref_type_name(ref_));
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
    (~mark_referenced=true, ref_: Ts.ref_): (string, string) => {
  let idents =
    ref_
    |> CCListLabels.map(~f=fst)
    |> CCListLabels.map(~f=v => ["_", v])
    |> CCListLabels.concat;
  let ref_resolved =
    (
      switch (idents) {
      | [] => ""
      | [_, ...rest] =>
        rest |> CCListLabels.fold_left(~init="", ~f=(p, e) => p ++ e)
      }
    )
    |> to_valid_typename;
  if (mark_referenced) {
    record_referenced := [ref_resolved |> fst, ...record_referenced^];
  };
  ref_resolved;
};

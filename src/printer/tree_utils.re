open String_utils;
open Tree_types;

module Ident = {
  type t = ts_identifier;

  let of_string = (str: string): t => {
    i_value: str,
    i_ident: str |> to_valid_ident,
    i_type: str |> to_valid_typename,
    i_module: str |> to_valid_module_name,
    i_variant: str |> to_valid_variant,
  };
  let of_pi = ({item, _}: Re_typescript_base.Ts.with_pi(string)) =>
    of_string(item);
  let of_int = (i: int): t => {
    let str = string_of_int(i);
    {
      i_value: str,
      i_ident: str |> to_valid_ident,
      i_type: str |> to_valid_typename,
      i_module: str |> to_valid_module_name,
      i_variant: str |> to_valid_variant,
    };
  };
  let value = ({i_value, _}: t) => i_value;
  let ident = ({i_ident, _}: t) => i_ident;
  let type_ = ({i_type, _}: t) => i_type;
  let module_ = ({i_module, _}: t) => i_module;
  let variant = ({i_variant, _}: t) => i_variant;
};

module Path = {
  type t = ts_path;
  let add = (str, (path, sub): t): t =>
    str != "" ? (path @ [str], sub) : (path, sub);
  let add_sub = (str, (path, sub): t): t => (path, sub @ [str]);
  let add_ident = (ident, (path, sub): t): t => (
    path @ [ident |> Ident.value],
    sub,
  );
  let add_sub_ident = (ident, (path, sub): t): t => (
    path,
    sub @ [ident |> Ident.value],
  );
  let add_ident = (ident, (path, sub): t): t => (
    path @ [ident |> Ident.value],
    sub,
  );
  let to_typename = ((path, sub): t): string =>
    path
    @ sub
    |> CCList.to_string(~sep="_", v => v)
    |> Ident.of_string
    |> Ident.type_;
  let to_scope = ((path, _): t): list(string) =>
    switch (path |> CCList.rev) {
    | []
    | [_] => []
    | [_, ...r] => r |> CCList.rev
    };
  let to_full_path = ((path, sub): t) => path @ sub;
  let to_unscoped_path = ((path, sub): t) => (path |> CCList.last(1)) @ sub;
  let unscoped_to_string = CCList.to_string(~sep="_", a => a);
  let to_string = ((path, sub): t) => path @ sub |> unscoped_to_string;
  let to_ident = path => path |> to_string |> Ident.of_string;
  let eq_unscoped = CCEqual.list(CCEqual.string);
  let eq = ((path_a, sub_a): t, (path_b, sub_b): t) => {
    eq_unscoped(path_a, path_b) && eq_unscoped(sub_a, sub_b);
  };
};

module Exceptions = {
  exception Parser_error(string);
};

let list_to_opt =
  fun
  | [] => None
  | a => Some(a);

let rec get_union_type_name = (um_type: ts_type) => {
  switch (um_type) {
  | Base(String) => "string"
  | Base(Number) => "number"
  | Base(Boolean) => "boolean"
  | Base(Void) => "void"
  | Base(Null) => "null"
  | Base(Undefined) => "undefined"
  | Base(Any) => "any"
  | Function(_) => "func"
  | Reference({tr_path, _}) =>
    tr_path |> CCList.last_opt |> CCOpt.value(~default="unknown")
  // | Base(Arg(_)) => "inferred"
  | Optional(t)
  | Nullable(t) => get_union_type_name(t)
  | Array(t) => Printf.sprintf("array_%s", get_union_type_name(t))
  | Tuple(_) => "tuple"
  | MixedLiteral(_) => "variant"
  | NumericLiteral(_) => "num"
  | StringLiteral(_) => "literal"
  | Enum(_) => "enum"
  | Arg(i) => i |> Ident.ident
  // | VariantEnum(_) =>
  //   raise(Decode_Error("Union is not a valid union member"))
  | Union(_) =>
    raise(Exceptions.Parser_error("Union is not a valid union member"))
  // | Record(_) => raise(Decode_Error("Record is not a valid union member"))
  // | RecordField(_) =>
  //   raise(Decode_Error("Record is not a valid union member"))
  | TypeDeclaration(_) =>
    raise(
      Exceptions.Parser_error("TypeDeclaration is not a valid union member"),
    )
  | Interface(_) =>
    raise(Exceptions.Parser_error("Interface is not a valid union member"))
  | Import(_) =>
    raise(Exceptions.Parser_error("Import is not a valid union member"))
  | Module(_) =>
    raise(Exceptions.Parser_error("Module is not a valid union member"))
  };
};

let type_to_string = (t: Re_typescript_base.Ts.type_) =>
  switch (t) {
  | Object(_) => "Object"
  | Tuple(_) => "Tuple"
  | Array(_) => "Array"
  | Function(_) => "Function"
  | Constructor(_) => "Constructor"
  | Intersection(_) => "Intersection"
  | Union(_) => "Union"
  | Query(_) => "Query"
  | StringLiteral(_) => "StringLiteral"
  | NumberLiteral(_) => "NumberLiteral"
  | BoolLiteral(_) => "BoolLiteral"
  | TypeReference(_) => "TypeReference"
  | TypeExtract(_) => "TypeExtract"
  | String(_) => "String"
  | Number(_) => "Number"
  | Boolean(_) => "Boolean"
  | Null(_) => "Null"
  | Undefined(_) => "Undefined"
  | Void(_) => "Void"
  | Any(_) => "Any"
  | Symbol(_) => "Symbol"
  | This(_) => "This"
  | UnionTemp(_) => "UnionTemp"
  };

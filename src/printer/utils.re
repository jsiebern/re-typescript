let string_replace = (input, output) =>
  Str.global_replace(Str.regexp_string(input), output);

let to_valid_ident = ident => (
  (
    try(
      if (ident.[0] >= '0' && ident.[0] <= '9') {
        "_" ++ ident;
      } else {
        CCString.uncapitalize_ascii(
          // from gist of sgrove, source:
          // https://gist.github.com/sgrove/335bf1759d8d2f685dfea80d4e6afac7
          [
            "and",
            "as",
            "asr",
            "assert",
            "begin",
            "class",
            "constraint",
            "do",
            "done",
            "downto",
            "else",
            "end",
            "esfun",
            "exception",
            "external",
            "false",
            "for",
            "fun",
            "function",
            "functor",
            "if",
            "in",
            "include",
            "inherit",
            "initializer",
            "land",
            "lazy",
            "let",
            "lor",
            "lsl",
            "lsr",
            "lxor",
            "match",
            "method",
            "mod",
            "module",
            "mutable",
            "new",
            "nonrec",
            "object",
            "of",
            "open",
            "open!",
            "or",
            "pri",
            "private",
            "pub",
            "public",
            "rec",
            "sig",
            "struct",
            "switch",
            "then",
            "to",
            "true",
            "try",
            "type",
            "val",
            "virtual",
            "when",
            "while",
            "with",
          ]
          |> List.exists(reserved_word => ident == reserved_word)
            ? ident ++ "_" : ident,
        );
      }
    ) {
    | Invalid_argument(_) => "_"
    | e => raise(e)
    }
  )
  |> string_replace("$", "_")
  |> string_replace(".", "_"),
  ident,
);

let to_valid_typename = to_valid_ident;

let to_int_variant_constructor = (i: int) => (Printf.sprintf("_%#u", i), i);

let to_valid_variant_constructor = ident => (
  ident |> CCString.capitalize_ascii |> string_replace("$", "_"),
  ident,
);

let to_valid_module_name = to_valid_variant_constructor;

let list_to_opt =
  fun
  | [] => None
  | a => Some(a);
let opt_to_list =
  fun
  | None => []
  | Some(lst) => lst;
let list_of_opt = opt_to_list;
let opt_of_list = list_to_opt;

let pp_bool = b => b ? "Y" : "N";

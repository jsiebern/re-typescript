let string_replace = (input, output) =>
  Str.global_replace(Str.regexp_string(input), output);

let escape_leading_num = ident =>
  ident |> CCString.length > 0
    ? ident.[0] >= '0' && ident.[0] <= '9' ? "_" ++ ident : ident : "_";

let to_valid_ident = ident =>
  (
    try(
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
      )
    ) {
    | Invalid_argument(_) => "_"
    | e => raise(e)
    }
  )
  |> string_replace("$", "_")
  |> string_replace(".", "_")
  |> escape_leading_num;

let to_valid_typename = to_valid_ident;

let to_valid_variant = ident =>
  ident
  |> escape_leading_num
  |> CCString.capitalize_ascii
  |> string_replace("$", "_")
  |> string_replace(".", "_");

let to_valid_module_name = name =>
  switch (Fp.relative(name) |> CCOpt.flat_map(Fp.baseName)) {
  | None => to_valid_variant(name)
  | Some(base_name) => base_name |> to_valid_variant
  };

// --- String literals
// (String literals have 3 config types: Variant, PolyVariant, BsInline. These will be handled differently for generating the encoder / decoder functions.)
type string_literal = "double" | "single" | "$_bad_string";
type string_literal_opt = "one" | "two" | undefined;

// --- Automatic Extension of union literals (keeps around the original)
type string_literal_ext = "x" | "y";
type string_literal_ext_base = "z" | string_literal_ext;

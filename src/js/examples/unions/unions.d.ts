// --- Type unions (Generates bs.unboxed by default)
// (Creating a variant alongside the unboxed value is planned. Still undecided on how to build the classify structure.)
type string_literal = "double" | "single";

type union_1 = string | number | undefined;
type union_2 = string_literal | string;

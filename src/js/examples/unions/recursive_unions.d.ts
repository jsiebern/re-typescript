// --- Recursive unions
type union_3 = string | Array<union_3>;
type union_4 = string | { field: union_4 };

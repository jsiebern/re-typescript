// --- Inline function definitions
interface i_inline_function {
  field: string;
  action: (a: string, b?: number) => void;
}
type i_inline_access = i_inline_function["action"];

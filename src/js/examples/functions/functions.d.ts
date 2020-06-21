// --- Function definitions
declare function some_function(): void;
declare function some_other_function(): { inline: string };
declare function some_function_args(a: string, b: number): string;
declare function some_function_opt_args(a: string, b?: number): string;
declare function some_function_inline_args(
  a: string | number,
  b?: { inline: number }
): string;
declare function some_function_as_any();
declare function some_function_params<A, B, C>(a: A, b: B): C;

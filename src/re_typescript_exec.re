let test = (
  "/test.d.ts",
  {|

type c = {a: string, b: number};
type d = c['a'][]



// interface method_signature {
//   func1: (x: number) => number;
//   func2(x: number): number;
//   func3(x: string, y: boolean): void;
// }
// type extracted_method_signature = method_signature['func3' | 'func2'];
|},
);

Console.log(
  "\n-------------------------------------------------------------------\n",
);

switch (Re_typescript_lib.quick_parse([|test|])) {
| Error(e) => raise(Failure(e))
| Ok(parsed) => Console.log(parsed)
};

Console.log(
  "\n-------------------------------------------------------------------\n",
);

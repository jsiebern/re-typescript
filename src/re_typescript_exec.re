let test = (
  "/test.d.ts",
  {|

// interface A {
//     x: string;
//     3: number;
//     y: boolean;
// }
// type keys = keyof A;

type numlit = 3 | 'x' | 8

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

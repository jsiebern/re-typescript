let test = (
  "/test.d.ts",
  {|


export function someFunction(a: number, b: [ number, { field: string } ]): [string, boolean];


|},
);
switch (Re_typescript_lib.quick_parse([|test|])) {
| Error(e) => raise(Failure(e))
| Ok(parsed) => Console.log(parsed)
};

Console.log(
  "\n-------------------------------------------------------------------\n",
);

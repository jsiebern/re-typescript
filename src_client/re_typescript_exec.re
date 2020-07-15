let test = (
  "/test.d.ts",
  {|


declare function someFunction(a: string | number, b?: { inline: number }): string;


|},
);
switch (Re_typescript_lib.quick_parse([|test|])) {
| Error(e) => raise(Failure(e))
| Ok(parsed) => Console.log(parsed)
};

Console.log(
  "\n-------------------------------------------------------------------\n",
);

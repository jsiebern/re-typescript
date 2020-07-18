let test = (
  "/test.d.ts",
  {|

interface KeyValueProcessor {
          (key: number, value: string): boolean;
      }


|},
);

switch (Re_typescript_lib.quick_parse([|test|])) {
| Error(e) => raise(Failure(e))
| Ok(parsed) => Console.log(parsed)
};

Console.log(
  "\n-------------------------------------------------------------------\n",
);

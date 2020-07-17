let test = (
  "/test.d.ts",
  {|

interface I_c { field_4: boolean, run(a: string, b: boolean): void }
interface I_b extends I_c { field: string, field3: number }
interface I_a extends I_b { field2: string }


|},
);
switch (Re_typescript_lib.quick_parse([|test|])) {
| Error(e) => raise(Failure(e))
| Ok(parsed) => Console.log(parsed)
};

Console.log(
  "\n-------------------------------------------------------------------\n",
);

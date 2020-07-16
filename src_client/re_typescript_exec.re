let test = (
  "/test.d.ts",
  {|

interface I_b { field: string }
interface I_a extends I_b {}


|},
);
switch (Re_typescript_lib.quick_parse([|test|])) {
| Error(e) => raise(Failure(e))
| Ok(parsed) => Console.log(parsed)
};

Console.log(
  "\n-------------------------------------------------------------------\n",
);

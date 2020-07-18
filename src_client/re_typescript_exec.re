let test = (
  "/test.d.ts",
  {|

interface I_a<A, B = boolean> {
  field_1: A;
  field_2: B;
  field_4: { xy: string };
}
interface I_b extends I_a<{another: boolean}> {
  field_3: number
}
interface c extends I_a<{xxx: boolean}> {
  field_3: number
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

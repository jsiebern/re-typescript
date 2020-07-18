let test = (
  "/test.d.ts",
  {|

interface I_a<A, B = boolean> {
        field_1: A;
        field_2: B;
        field_4: string;
      }
      interface I_b extends I_a<string> {
        field_3: number
      }

|},
);
let test2 = (
  "/test2.d.ts",
  {|

interface xI_b<A> { field: A, field3: number }
interface xI_a extends xI_b<string> { field2: boolean }
interface xI_c extends I_a<number, string>, xI_b<boolean> {}

|},
);
switch (Re_typescript_lib.quick_parse([|test, test2|])) {
| Error(e) => raise(Failure(e))
| Ok(parsed) => Console.log(parsed)
};

Console.log(
  "\n-------------------------------------------------------------------\n",
);

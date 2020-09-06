let test = (
  "/test.d.ts",
  {|

interface Person {
              name: string;
              age: number;
          }

          type NullablePerson = { [P in keyof Person]: Person[P] | null }
          type PartialPerson = { [P in keyof Person]?: Person[P] }

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

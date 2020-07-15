let test = (
  "/test.d.ts",
  {|


interface iTest {
    field: string;
    action: (a: string, b?: number) => void;
}
type access = iTest['action'];

|},
);
switch (Re_typescript_lib.quick_parse([|test|])) {
| Error(e) => raise(Failure(e))
| Ok(parsed) => Console.log(parsed)
};

Console.log(
  "\n-------------------------------------------------------------------\n",
);

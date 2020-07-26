let test = (
  "/test.d.ts",
  {|

interface Keys {
  key_1: string;
  key_2: number;
}
type with_keys = { [K in keyof Keys]: 'a' | 'b' };


// ------------------------------------------------------

|},
);

switch (Re_typescript_lib.quick_parse([|test|])) {
| Error(e) => raise(Failure(e))
| Ok(parsed) => Console.log(parsed)
};

Console.log(
  "\n-------------------------------------------------------------------\n",
);

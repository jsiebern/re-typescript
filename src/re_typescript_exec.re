let test = (
  "/test.d.ts",
  {|

type key = 'just_one_key';
type Flags = { [K in key]: boolean };

type key2 = 4;
type Flags2 = { [K in key2]: boolean };

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

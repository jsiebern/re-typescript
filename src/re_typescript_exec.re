let test = (
  "/test.d.ts",
  {|

interface KeysExt {
            key_3: boolean;
          }
          interface Keys extends KeysExt {
            key_1: string;
            key_2: number;
          }
          type with_keys = { [K in keyof Keys]: boolean };

// ------------------------------------------------------

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

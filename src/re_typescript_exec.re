let test = (
  "/test.d.ts",
  {|

type some_interface = { field: boolean } | null;

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

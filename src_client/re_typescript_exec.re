let test = (
  "/test.d.ts",
  {|


   declare enum Enum { red, Green, blue }

type tpl = [string, number, boolean];

type s = string;
|},
);
switch (Re_typescript_lib.quick_parse([|test|])) {
| Error(e) => raise(Failure(e))
| Ok(parsed) => Console.log(parsed)
};

Console.log(
  "\n-------------------------------------------------------------------\n",
);

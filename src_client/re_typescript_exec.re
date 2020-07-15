let test = (
  "/test.d.ts",
  {|


declare function someFunction(): string;
declare function someOtherFunction(): { inline: string };


|},
);
switch (Re_typescript_lib.quick_parse([|test|])) {
| Error(e) => raise(Failure(e))
| Ok(parsed) => Console.log(parsed)
};

Console.log(
  "\n-------------------------------------------------------------------\n",
);

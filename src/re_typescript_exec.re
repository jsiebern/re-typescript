let test = (
  "/test.d.ts",
  {|

export enum Enum1 { Red, Green, Blue }
export enum Enum2 { Red, Green, Blue }
declare enum Enum3 { red, Green, blue }

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

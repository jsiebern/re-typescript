let test = (
  "/test.d.ts",
  {|

export enum Keys {
    A,
    B,
    C
}
type Flags = { [K in Keys]: { x: K } }

// type kof = keyof string;

|},
);

switch (Re_typescript_lib.quick_parse([|test|])) {
| Error(e) => raise(Failure(e))
| Ok(parsed) => Console.log(parsed)
};

Console.log(
  "\n-------------------------------------------------------------------\n",
);

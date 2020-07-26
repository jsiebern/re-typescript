let test = (
  "/test.d.ts",
  {|

type Nothing<T> = {
  [P in keyof T]: T[P];
};
interface A {
  x: string;
  y?: number;
  z: boolean;
}
type same = Nothing<A>;
type req = Required<A>;
type part = Partial<A>;

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

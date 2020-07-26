let test = (
  "/test.d.ts",
  {|

interface A<X> {
  x: string;
  y: X;
  z: boolean;
}

type keys = 'x' | 'y';
type stripped = Pick<A<{obj: number}>, keys>;


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

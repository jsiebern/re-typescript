let test = (
  "/test.d.ts",
  {|

type PickX<T, K extends keyof T> = {
              [P in K]: T[P];
          };
          interface A {
            x: string;
            y: number;
            z: boolean;
          }

          type keys = 'x' | 'y';
          type stripped = PickX<A, keys>;


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

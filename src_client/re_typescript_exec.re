let test = (
  "/test.d.ts",
  {|


          interface A {
            x: string;
            y: number;
            z: boolean;
          }

          type keys = 'x';
          type stripped = Pick<A, keys>;


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

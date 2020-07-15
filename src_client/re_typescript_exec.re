let test = (
  "/test.d.ts",
  {|


// export function someFunction<A,B,C>(a: A, b: B): C;
type x<A> = A;
type y = x<string>;


|},
);
switch (Re_typescript_lib.quick_parse([|test|])) {
| Error(e) => raise(Failure(e))
| Ok(parsed) => Console.log(parsed)
};

Console.log(
  "\n-------------------------------------------------------------------\n",
);

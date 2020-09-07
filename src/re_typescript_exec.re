let test = (
  "/test.d.ts",
  {|

// can be generated inline
// interface obj {
//   field: 'red' | 'blue',
// }
// type in_arr = Array<obj['field']>;


// can use recursion in unions in combination with type params
type x<B = number> = string | B;
type y = x<boolean>

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

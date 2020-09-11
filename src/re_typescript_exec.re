let test = (
  "/test.d.ts",
  {|

interface I_a<A, B = boolean> {
  field_1: A;
  field_2: B;
  field_4: string;
}
interface I_b extends I_a<string> {
  field_3: number
}


// works with array / nullable on the original
// type x = 3 | 4;
// type y = Array<1 | x | 2 | null>;

// type animation = 'overlay' | 'push' | 'scale down' | 'uncover' | 'slide out' | 'slide along'

// export declare type Subset<T, U> = {
//   [key in keyof T]: key extends keyof U ? T[key] : never;
// };

// type xyz = Subset<animation, 'push' | 'scale down'>

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

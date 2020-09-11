let test = (
  "/test.d.ts",
  {|

// map over a single string literal
type key = 'just_one_key';
            type Flags = { [K in key]: boolean };

            type key2 = 4;
          type Flags2 = { [K in key2]: boolean };

// resolves nested params
// interface A<X> {
//   field_x: X;
// }
// interface B<Y, Z> {
//   field_y: Y;
//   field_z: Z;
// }
// type apply = B<A<boolean>, number>;



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

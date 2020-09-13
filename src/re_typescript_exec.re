let test = (
  "/test.d.ts",
  {|

// type animation = 'overlay' | 'push' | 'scale down' | 'uncover' | 'slide out' | 'slide along'

// export declare type Subset<T, U> = {
//   [key in keyof T]: key extends keyof U ? T[key] : never;
// };

// type xyz = Subset<'push' | 'scale down', animation>


// type BoxedValue<T> = { value: T };
// type BoxedArray<T> = { array: T[] };
// type Boxed<T> = T extends any[] ? BoxedArray<T[number]> : BoxedValue<T>;

// type T1 = Boxed<string>;
// type T2 = Boxed<number[]>;
// type T3 = Boxed<string | number[]>;

// declare namespace React {
//   export type ReactType<TProps> = TProps;
//   export type ReactNode = string;
//   export type ReactNodeArray = string[];
//   export type ReactElement<T> = ReactType<T>;
// }

// type node = React.ReactType<{color: string}>;
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

let test = (
  "/test.d.ts",
  {|

// type animation = 'overlay' | 'push' | 'scale down' | 'uncover' | 'slide out' | 'slide along'

// export declare type Subset<T, U> = {
//   [key in keyof T]: key extends keyof U ? T[key] : never;
// };

// type xyz = Subset<'push' | 'scale down', animation>

// type x<a> = a extends string ? string : number;
// type y = x<string>
// type z = x<boolean>

// type TypeName<T> = T extends string
//   ? "string"
//   : T extends number
//   ? "number"
//   : T extends boolean
//   ? "boolean"
//   : T extends undefined
//   ? "undefined"
//   : T extends Function
//   ? "function"
//   : "object";

// type T0 = TypeName<string>; // "string"
// type T1 = TypeName<"a">; // "string"
// type T2 = TypeName<true>; // "boolean"
// type T3 = TypeName<() => void>; // "function"
// type T4 = TypeName<string[]>; // "object"

declare namespace React {
  export type ReactType<TProps> = TProps;
  export type ReactNode = string;
  export type ReactNodeArray = string[];
  export type ReactElement<T> = ReactType<T>;
}

type node = React.ReactType<{color: string}>;
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

let test = (
  "/test.d.ts",
  {|


type Diff<T, U> = T extends U ? never : T;
type Filter<T, U> = T extends U ? T : never;

type T1 = Diff<"a" | "b" | "c" | "d", "a" | "c" | "f">;
type T2 = Filter<"a" | "b" | "c" | "d", "a" | "c" | "f">; // "a" | "c"
type T3 = Diff<string | number | (() => void), Function>; // string | number
type T4 = Filter<string | number | (() => void), Function>; // () => void

type NotNullable<T> = Diff<T, null | undefined>;

type T5 = NotNullable<string | number | undefined>;
type T6 = NotNullable<string | string[] | null | undefined>;

// type animation = 'overlay' | 'push' | 'scale down' | 'uncover' | 'slide out' | 'slide along'

// export declare type Subset<T, U> = {
//   [key in keyof T]: key extends keyof U ? T[key] : never;
// };

// type xyz = Subset<'push' | 'scale down', animation>

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

switch (Re_typescript_lib.quick_parse(~warnings=true, [|test|])) {
| Error(e) => raise(Failure(e))
| Ok(parsed) => Console.log(parsed)
};

Console.log(
  "\n-------------------------------------------------------------------\n",
);

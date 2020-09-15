let test = (
  "/test.d.ts",
  {|

// Discriminated
// type TimingEvent = { name: "start"; userStarted: boolean } | { name: "closed"; duration: number };

// ------------------------------------------

// Overloaded functions
// declare function getWidget(n: number): number;
// declare function getWidget(s: string): number[];

// ------------------------------------------

// type FunctionPropertyNames<T> = {
//   [K in keyof T]: T[K] extends Function ? K : never;
// }[keyof T];
// type FunctionProperties<T> = Pick<T, FunctionPropertyNames<T>>;

// type NonFunctionPropertyNames<T> = {
//   [K in keyof T]: T[K] extends Function ? never : K;
// }[keyof T];
// type NonFunctionProperties<T> = Pick<T, NonFunctionPropertyNames<T>>;

// interface Part {
//   id: number;
//   name: string;
//   subparts: Part[];
//   updatePart(newName: string): void;
// }

// type T1 = FunctionPropertyNames<Part>;
// type T2 = NonFunctionPropertyNames<Part>;
// type T3 = FunctionProperties<Part>;
// type T4 = NonFunctionProperties<Part>;

// ----------------------------

// type animation = 'overlay' | 'push' | 'scale down' | 'uncover' | 'slide out' | 'slide along'

// export declare type Subset<T, U> = {
//   [key in keyof T]: key extends keyof U ? T[key] : never;
// };

// type xyz = Subset<'push' | 'scale down', animation>
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

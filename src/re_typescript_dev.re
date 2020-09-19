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

type FunctionPropertyNames<T> = {
  [K in keyof T]: T[K] extends Function ? K : never;
}[keyof T];
type FunctionProperties<T> = Pick<T, FunctionPropertyNames<T>>;

type NonFunctionPropertyNames<T> = {
  [K in keyof T]: T[K] extends Function ? never : K;
}[keyof T];
type NonFunctionProperties<T> = Pick<T, NonFunctionPropertyNames<T>>;

interface Part {
  id: number;
  name: string;
  subparts: Part[];
  updatePart(newName: string): void;
}

export type T1 = FunctionPropertyNames<Part>;
export type T2 = NonFunctionPropertyNames<Part>;
export type T3 = FunctionProperties<Part>;
export type T4 = NonFunctionProperties<Part>;

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

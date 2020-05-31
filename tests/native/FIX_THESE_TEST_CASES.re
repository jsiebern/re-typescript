open TestFramework;

describe("failing tests that shouldn't", ({test, _}) => {
  // test("string literals with digits at start", ({expect, _}) => {
  //   expect.fn(() => print({|
  //           type c = '0ab';
  //       |})).toThrow()
  // });
  test(
    "declare keyword, dynamic objects, keyof keyword ternaries",
    ({expect, _})
    => {
      expect.fn(() =>
        print(
          {|
          export declare type Subset<T, U> = {
            [key in keyof T]: key extends keyof U ? T[key] : never;
          };
        |},
        )
      ).
        toThrow()
    })
    // ---
    // type x = 3 | 4;
    // type y = (1 | x | 2)[];
    // ---
    // function someFunction<A,B,C>(a: A, b: B): [B,C];
    // ---
    // type union = string | number | (a: string, b?: number) => string;
    // ---
    // function (a: string, b: (a: string, b?: number) => string): void;
    // ---
    // type ValueOrArray<T> = T | Array<ValueOrArray<T>>;
    // ---
    // type Json =
    // | string
    // | number
    // | boolean
    // | null
    // | { [property: string]: Json }
    // | Json[];
    // ---
    //   {
    //     func1(x: number): number;         // Method signature
    //     func2: (x: number) => number;     // Function type literal
    //     func3: { (x: number): number };   // Object type literal
    // }
});

open TestFramework;

describe("failing tests that shouldn't", ({test, _}) => {
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
    // type obj = {
    //   type: string;
    // };
    // ---
    // type x = 3 | 4;
    // type y = (1 | x | 2)[];
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
    //  interface x {
    //     func3: { (x: number): number };   // Object type literal
    // }
});

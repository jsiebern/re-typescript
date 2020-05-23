open TestFramework;

describe("failing tests that shouldn't", ({test, _}) => {
  // test("string literals with digits at start", ({expect, _}) => {
  //   expect.fn(() => print({|
  //           type c = '0ab';
  //       |})).toThrow()
  // });
  test(
    "ref resolving in string literals (and other primitive unions)",
    ({expect, _}) => {
    expect.string(
      print(
        {|
            type x = 'x' | 'y';
            type y = 'z' | x;
        |},
      ),
    ).
      toMatchSnapshot()
  });
  test(
    "declare keyword, dynamic objects, keyof keyword ternaries",
    ({expect, _}) => {
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
  });
  // function someFunction<A,B,C>(a: A, b: B): [B,C];
  // type union = string | number | (a: string, b?: number) => string;
  // function (a: string, b: (a: string, b?: number) => string): void;
});

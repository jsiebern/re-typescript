open TestFramework;

describe("failing tests that shouldn't", ({test, _}) => {
  test("field access through references", ({expect, _}) => {
    expect.fn(() => {
      print(
        {|
            type y = {key_3:string};
            type x = y['key_3']
        |},
      )
      |> ignore;
      raise(Not_found);
    }).
      toThrow()
  });
  test("string literals in field access", ({expect, _}) => {
    expect.fn(() =>
      print(
        {|
            type c = {a: string, b: number};
            type d = c['a' | 'b']
        |},
      )
    ).
      toThrow()
  });
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
});

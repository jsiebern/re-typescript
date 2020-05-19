open TestFramework;

describe("failing tests that shouldn't", ({test, _}) => {
  test("type arg default string literal", ({expect, _}) => {
    expect.fn(() =>
      print(
        {|
            type with_arg2<a = 'one' | 'two'> = a;
            type call_args2 = with_arg2;
        |},
      )
    ).
      toThrow()
  });
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
  test(
    "ref resolving in string literals (and other primitive unions)",
    ({expect, _}) => {
    expect.fn(() =>
      print(
        {|
            type k = 'b';
            type d = c['a' | k]
        |},
      )
    ).
      toThrow()
  });
});

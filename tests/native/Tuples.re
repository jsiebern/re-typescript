include TestFramework;

describe("tuples", ({test, _}) => {
  test("generates base type tuple", ({expect, _}) => {
    expect.string(
      print({|
            type tpl = [string, number, boolean];
        |}),
    ).
      toMatchSnapshot()
  });

  test("can use tuple inside of an interface", ({expect, _}) => {
    expect.string(
      print(
        {|
            interface obj {
                field: [string, number, boolean];
            }
        |},
      ),
    ).
      toMatchSnapshot()
  });

  test("can generate an array of tuples", ({expect, _}) => {
    expect.string(
      print(
        {|
            type tpl_arr = [string, number, boolean][];
            type tpl_arr2 = Array<[string, number, boolean]>;
        |},
      ),
    ).
      toMatchSnapshot()
  });

  test("can use inline interfaces", ({expect, _}) => {
    expect.string(
      print(
        {|
            type tpl = [string, number, { field: boolean }];
        |},
      ),
    ).
      toMatchSnapshot()
  });

  test("merges extracted tuple members back if possible", ({expect, _}) => {
    expect.string(
      print(
        {|
            type tpl = [ string, [ number, boolean ] ];
        |},
      ),
    ).
      toMatchSnapshot()
  });
});

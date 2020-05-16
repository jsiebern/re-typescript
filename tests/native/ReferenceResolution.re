open TestFramework;

describe("reference resolution", ({test, _}) => {
  test("resolve simple definitions", ({expect, _}) => {
    expect.string(
      print({|
      type t_str = string;
      type t_ref = t_str;
    |}),
    ).
      toMatchSnapshot()
  });

  test("resolve in interface fields", ({expect, _}) => {
    expect.string(
      print(
        {|
      type t_str = string;
      interface I_Ref {
        f_num: number;
        f_str: t_str;
      };
      type r_ref = {
        inner: I_Ref
      }
    |},
      ),
    ).
      toMatchSnapshot()
  });

  test("resolve recursively", ({expect, _}) => {
    expect.string(
      print(
        {|
      interface I_Ref {
        f_num: number;
        f_inner: I_Ref
      };
    |},
      ),
    ).
      toMatchSnapshot()
  });
});

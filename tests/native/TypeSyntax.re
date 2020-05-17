open TestFramework;

describe("basic syntax for types", ({test, _}) => {
  test("basic type definition", ({expect, _}) => {
    expect.string(print("type t_string = string;")).toMatchSnapshot();
    expect.string(print("type t_number = number;")).toMatchSnapshot();
    expect.string(print("type t_bool = boolean;")).toMatchSnapshot();
  });

  test("basic type definition without semicolon", ({expect, _}) => {
    expect.string(print("type t_string = string")).toMatchSnapshot()
  });

  test("adds extra type for any", ({expect, _}) => {
    expect.string(print("type t_any = any")).toMatchSnapshot()
  });

  test("comments should not throw", ({expect, _}) => {
    expect.string(print({|// This is a comment|})).toEqual("");
    expect.string(print({|/* This is a comment */|})).toEqual("");
  });
});

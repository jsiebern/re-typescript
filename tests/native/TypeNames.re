open TestFramework;

describe("generates valid type names", ({test, _}) => {
  test("capitalized", ({expect, _}) => {
    expect.string(print("type T_string = string")).toMatchSnapshot()
  });
  test("reserved keyword", ({expect, _}) => {
    expect.string(print("type rec = string")).toMatchSnapshot();
    expect.string(print("type sig = string")).toMatchSnapshot();
  });
  test("invalid characters", ({expect, _}) => {
    expect.string(print("type st$ring = string")).toMatchSnapshot();
    expect.string(print("type t_$string = string")).toMatchSnapshot();
    expect.string(print("type $_string = string")).toMatchSnapshot();
    expect.string(print("type t_$string = string")).toMatchSnapshot();
    // Invalid TS typenames should not be parsed at all
    expect.fn(() => print("type 1_string = string")).toThrow();
    expect.fn(() => print("type st.ring = string")).toThrow();
  });
});

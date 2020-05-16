open TestFramework;

describe("config flags effects", ({test, _}) => {
  test("number_mode default value", ({expect, _}) => {
    let ts = "type t_number = number";
    expect.string(print(ts)).toMatch(
      print(~ctx={...config, number_mode: Float}, ts),
    );
  });

  test("number_mode", ({expect, _}) => {
    let ts = "type t_number = number";
    expect.string(print(~ctx={...config, number_mode: Float}, ts)).
      toMatchSnapshot();
    expect.string(print(~ctx={...config, number_mode: Int}, ts)).
      toMatchSnapshot();
    expect.string(print(~ctx={...config, number_mode: Unboxed}, ts)).
      toMatchSnapshot();
  });
});

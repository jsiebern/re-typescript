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

  test("suppress_warning_for_extended_records", ({expect, _}) => {
    let ts = "interface i_1 { field1: string }; interface i_2 extends i_1 { field2: boolean }";
    expect.string(
      print(
        ~ctx={...config, suppress_warning_for_extended_records: false},
        ts,
      ),
    ).
      toMatchSnapshot();
    expect.string(
      print(
        ~ctx={...config, suppress_warning_for_extended_records: true},
        ts,
      ),
    ).
      toMatchSnapshot();
  });

  test("omit_extended_unreferenced_records", ({expect, _}) => {
    let ts = "interface i_1 { field1: string }; interface i_2 extends i_1 { field2: boolean }";
    expect.string(
      print(~ctx={...config, omit_extended_unreferenced_records: false}, ts),
    ).
      toMatchSnapshot();
    expect.string(
      print(~ctx={...config, omit_extended_unreferenced_records: true}, ts),
    ).
      toMatchSnapshot();
  });
});

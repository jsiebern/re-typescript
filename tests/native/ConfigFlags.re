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

  test("omit_extended_unreferenced_records - off", ({expect, _}) => {
    let ts = "interface i_1 { field1: string }; interface i_2 extends i_1 { field2: boolean }";
    expect.string(
      print(~ctx={...config, omit_extended_unreferenced_records: false}, ts),
    ).
      toMatchSnapshot();
  });
  test("omit_extended_unreferenced_records - on", ({expect, _}) => {
    let ts = "interface i_1 { field1: string }; interface i_2 extends i_1 { field2: boolean }";
    expect.string(
      print(~ctx={...config, omit_extended_unreferenced_records: true}, ts),
    ).
      toMatchSnapshot();
  });
  test(
    "omit_extended_unreferenced_records - on (extension chain)",
    ({expect, _}) => {
    let ts = {|
      interface i_1 { field1: string };
      interface i_2 extends i_1 { field2: boolean }
      interface i_3 extends i_2 { field3: number }
    |};
    expect.string(
      print(~ctx={...config, omit_extended_unreferenced_records: true}, ts),
    ).
      toMatchSnapshot();
  });
  test(
    "omit_extended_unreferenced_records - on (double extension)",
    ({expect, _}) => {
    let ts = {|
      interface i_1 { field1: string };
      interface i_2 extends i_1 { field2: boolean }
      interface i_3 extends i_2 { field3: number }
      interface i_4 extends i_2 { field4: string }
    |};
    expect.string(
      print(~ctx={...config, omit_extended_unreferenced_records: true}, ts),
    ).
      toMatchSnapshot();
  });
  test(
    "omit_extended_unreferenced_records - on (double extension, + ref)",
    ({expect, _}) => {
    let ts = {|
      interface i_1 { field1: string };
      interface i_2 extends i_1 { field2: boolean }
      interface i_3 extends i_2 { field3: number }
      interface i_4 extends i_2 { field4: string }
      type holds_ref = i_2;
    |};
    expect.string(
      print(~ctx={...config, omit_extended_unreferenced_records: true}, ts),
    ).
      toMatchSnapshot();
  });
  test(
    "omit_extended_unreferenced_records - on (omits warning if enabled)",
    ({expect, _}) => {
    let ts = {|
      interface i_1 { field1: string };
      interface i_2 extends i_1 { field2: boolean }
    |};
    expect.string(
      print(
        ~ctx={
          ...config,
          omit_extended_unreferenced_records: true,
          suppress_warning_for_extended_records: true,
        },
        ts,
      ),
    ).
      toMatchSnapshot();
  });
  test(
    "omit_extended_unreferenced_records - on (displays warning if necessary after all)",
    ({expect, _}) => {
      let ts = {|
      interface i_1 { field1: string };
      interface i_2 extends i_1 { field2: boolean }
      type holds_ref = i_1;
    |};
      expect.string(
        print(
          ~ctx={
            ...config,
            omit_extended_unreferenced_records: true,
            suppress_warning_for_extended_records: true,
          },
          ts,
        ),
      ).
        toMatchSnapshot();
    },
  );
  test(
    "omit_extended_unreferenced_records - on (resolves type params correctly, even if omitting the interface)",
    ({expect, _}) => {
      let ts = {|
      interface i_1<A, B = boolean> { field1: A, field2: B };
      interface i_2<A, C> extends i_1<A> { field3: C }
    |};
      expect.string(
        print(
          ~ctx={...config, omit_extended_unreferenced_records: true},
          ts,
        ),
      ).
        toMatchSnapshot();
    },
  );
});

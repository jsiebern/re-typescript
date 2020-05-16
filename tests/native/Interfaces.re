include TestFramework;

describe("interfaces", ({test, _}) => {
  test("object & interface syntax create equal types", ({expect, _}) => {
    expect.string(print({|type Eqiv = { field: string }|})).toEqual(
      print({|
      interface Eqiv {
        field: string;
      }
    |}),
    )
  });

  test("nested interfaces", ({expect, _}) => {
    expect.string(
      print(
        {|
      interface I_a {
        field_1: string;
        field_2: string;
        field_3: {
          inline: boolean;
          nested: string;
        }
      }
    |},
      ),
    ).
      toMatchSnapshot()
  });
  //   test("empty obj / interfaces should throw ", ({expect, _}) => {
  //     expect.fn(() => print({|interface I_a {};|})).toThrow();
  //     expect.fn(() => print({|type o_a = {}|})).toThrow();
  //   });
  //   test("empty interface should not throw when extened", ({expect, _}) => {
  //     expect.string(
  //       print(
  //         {|
  //         interface I_b { field: string };
  //         interface I_a extends I_b {};
  //       |},
  //       ),
  //     ).
  //       toMatchSnapshot()
  //   });
  //   test(
  //     "empty interface should not throw when having only been used in an extension",
  //     ({expect, _}) => {
  //     expect.string(
  //       print(
  //         {|
  //         interface I_a {};
  //         interface I_b extends I_a { field: string };
  //       |},
  //       ),
  //     ).
  //       toMatchSnapshot()
  //   });
});

describe("interface extension", ({test, _}) => {
  test("adds fields from other interfaces", ({expect, _}) => {
    expect.string(
      print(
        {|
      interface I_a {
        field_1: string;
        field_2: string;
      }
      interface I_b extends I_a {
        field_3: number
      }
    |},
      ),
    ).
      toMatchSnapshot()
  });

  test("fields from current interface take priority", ({expect, _}) => {
    expect.string(
      print(
        {|
      interface I_a {
        field_1: string;
        field_2: string;
      }
      interface I_b extends I_a {
        field_1: boolean;
        field_3: number
      }
    |},
      ),
    ).
      toMatchSnapshot()
  });
});

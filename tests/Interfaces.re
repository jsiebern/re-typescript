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
  test(
    "empty obj / interfaces should be valid but not rendered", ({expect, _}) => {
    expect.string(
      print({|
      export interface I_a {}
      type o_a = {}
    |}),
    ).
      toMatch(
      "",
    )
  });
  test("empty objects should be referenced as any", ({expect, _}) => {
    expect.string(
      print({|
      export interface I_a {}
      type x = I_a
    |}),
    ).
      toMatchSnapshot()
  });
  test("empty interface should not be omitted when extened", ({expect, _}) => {
    expect.string(
      print(
        {|
          interface I_b { field: string }
          interface I_a extends I_b {}
        |},
      ),
    ).
      toMatchSnapshot()
  });
  test("empty interface can be target of an extension", ({expect, _}) => {
    expect.string(
      print(
        {|
          interface I_a {}
          interface I_b extends I_a { field: string }
        |},
      ),
    ).
      toMatchSnapshot()
  });
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
    expect.fn(() =>
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
      )
    ).
      toThrow()
  });

  test("incomplete type args throw on extension", ({expect, _}) => {
    expect.fn(() =>
      print(
        {|
      interface i_1<C, A = string> { field1: A, fieldx: C };
      interface i_2 extends i_1 {}
    |},
      )
    ).
      toThrow()
  });

  test("resolves extensions completely", ({expect, _}) => {
    expect.string(
      print(
        {|
      interface I_a<A, B = boolean> {
        field_1: A;
        field_2: B;
        field_4: string;
      }
      interface I_b extends I_a<string> {
        field_3: number
      }
    |},
      ),
    ).
      toMatchSnapshot()
  });

  test("can pass a type param on through an extension", ({expect, _}) => {
    expect.string(
      print(
        {|
      interface I_a<A, B = boolean> {
        field_1: A;
        field_2: B;
        field_4: string;
      }
      interface I_b<Local, Ext> extends I_a<string, Ext> {
        field_3: Local
      }
    |},
      ),
    ).
      toMatchSnapshot()
  });

  test("resolves single call signatures in an interface", ({expect, _}) => {
    expect.string(
      print(
        {|
      interface KeyValueProcessor {
          (key: number, value: string): boolean;
      }
    |},
      ),
    ).
      toMatchSnapshot()
  });

  test(
    "resolves single call signatures in an interface inline", ({expect, _}) => {
    expect.string(
      print(
        {|
      interface KeyValueProcessor {
          both: { (key: number, value: string): boolean }
      }
    |},
      ),
    ).
      toMatchSnapshot()
  });

  test(
    "throws an error if call signature is mixed with a field", ({expect, _}) => {
    expect.fn(() =>
      print(
        {|
      interface KeyValueProcessor {
          (key: number, value: string): boolean;
          field: string;
      }
    |},
      )
    ).
      toThrow()
  });
});

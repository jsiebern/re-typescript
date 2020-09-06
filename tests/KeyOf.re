open TestFramework;

let ctx = Lib.default_project_config;

describe("keyof operator", ({test, _}) => {
  test("throws for non object types", ({expect, _}) => {
    expect.string(print({|
        type kof = keyof string;
    |})).
      toMatchSnapshot()
  });
  test("extracts keys from an interface", ({expect, _}) => {
    expect.string(
      print(
        ~ctx,
        {|
            interface A {
                x: string;
                y: number;
            }
            type keys = keyof A;
        |},
      ),
    ).
      toMatchSnapshot()
  });
  test("extracts keys from an object", ({expect, _}) => {
    expect.string(
      print(
        ~ctx,
        {|
            type A = {
                x: string;
                y: number;
            }
            type keys = keyof A;
        |},
      ),
    ).
      toMatchSnapshot()
  });
  test("extracts keys from an inline object", ({expect, _}) => {
    expect.string(
      print(
        ~ctx,
        {|
            type keys = keyof {
                x: string;
                y: number;
            };
        |},
      ),
    ).
      toMatchSnapshot()
  });
  test("extracts keys from an extended interface", ({expect, _}) => {
    expect.string(
      print(
        ~ctx,
        {|
            interface A {
                x: string;
                y: number;
            }
            interface B extends A {
                z: boolean;
            }
            type keys = keyof B;
        |},
      ),
    ).
      toMatchSnapshot()
  });
  test("extracts mixed keys if necessary", ({expect, _}) => {
    expect.string(
      print(
        ~ctx,
        {|
            interface A {
                x: string;
                3: number;
                y: boolean;
            }
            type keys = keyof A;
        |},
      ),
    ).
      toMatchSnapshot()
  });
  test("extracts keys from a type extraction", ({expect, _}) => {
    expect.string(
      print(
        ~ctx,
        {|
            interface A {
                x: string;
                y: number;
                b: {
                    z: boolean,
                    f: string,
                }
            }
            type keys = keyof A['b'];
        |},
      ),
    ).
      toMatchSnapshot()
  });
  test("extracts keys from a type alias", ({expect, _}) => {
    expect.string(
      print(
        ~ctx,
        {|
              interface A {
                  x: string;
                  y: number;
              }
              type B = A;
              type keys = keyof B;
          |},
      ),
    ).
      toMatchSnapshot()
  });
  test("extracts keys from an optional wrapper", ({expect, _}) => {
    expect.string(
      print(
        ~ctx,
        {|
            interface A {
                x: string;
                y: number;
            }
            type B = A | undefined;
            type keys = keyof B;
            |},
      ),
    ).
      toMatchSnapshot()
  });
  test(
    "works together with parameterized mapped object types", ({expect, _}) => {
    expect.string(
      print(
        ~ctx,
        {|
            type PartialX<T> = {
                [P in keyof T]?: T[P];
            }
            interface A {
              x: string;
              y: number;
              z: boolean;
            }

            type a_partial = PartialX<A>;
            |},
      ),
    ).
      toMatchSnapshot()
  });
});

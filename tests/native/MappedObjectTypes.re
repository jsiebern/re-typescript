open TestFramework;

let ctx = {
  ...config,
  bucklescript_config: {
    ...config.bucklescript_config,
    string_variant_mode: `Variant,
    mixed_variant_mode: `Variant,
  },
};

describe("mapped object types", ({test, _}) => {
  test("map over predefined keys ", ({expect, _}) => {
    expect.string(
      print(
        ~ctx,
        {|
        type Keys = 'option1' | 'option2';
        type Flags = { [K in Keys]: boolean };
    |},
      ),
    ).
      toMatchSnapshot()
  });
  test("map over predefined keys through a reference", ({expect, _}) => {
    expect.string(
      print(
        ~ctx,
        {|
            type Keys = 'option1' | 'option2';
            type KeysRef = Keys;
            type Flags = { [K in KeysRef]: boolean };
        |},
      ),
    ).
      toMatchSnapshot()
  });
  test("map over inlined keys", ({expect, _}) => {
    expect.string(
      print(
        ~ctx,
        {|
            type Flags = { [K in 'option1' | 'option2']: boolean };
          |},
      ),
    ).
      toMatchSnapshot()
  });
  test("map over a keyof instruction", ({expect, _}) => {
    expect.string(
      print(
        ~ctx,
        {|
            type KeysObj = { option1: string, option2: number };
            type Flags = { [K in keyof KeysObj]: boolean };
          |},
      ),
    ).
      toMatchSnapshot()
  });
  test("map over an inline keyof instruction", ({expect, _}) => {
    expect.string(
      print(
        ~ctx,
        {|
            type Flags = { [K in keyof { option1: string, option2: number }]: boolean };
          |},
      ),
    ).
      toMatchSnapshot()
  });
  test("map over a single string literal", ({expect, _}) => {
    expect.string(
      print(
        ~ctx,
        {|
            type key = 'just_one_key';
            type Flags = { [K in key]: boolean };
          |},
      ),
    ).
      toMatchSnapshot()
  });
  test("map over a nested type", ({expect, _}) => {
    expect.string(
      print(
        ~ctx,
        {|
            type Keys = 'option1' | 'option2';
            interface A {
                key_1: string;
                key_2: number;
                flags: { [K in Keys]: boolean }
            };
          |},
      ),
    ).
      toMatchSnapshot()
  });
  test("can be used inline, for example in an intersection", ({expect, _}) => {
    expect.string(
      print(
        ~ctx,
        {|
          interface Keys {
            key_1: string;
            key_2: number;
          }
          type with_keys = { [K in keyof Keys]: boolean } & Keys;
        |},
      ),
    ).
      toMatchSnapshot()
  });
  test("maps over recursively defined interfaces", ({expect, _}) => {
    expect.string(
      print(
        ~ctx,
        {|
        interface A {
          key_1: string;
          key_2: number;
          flags: { [K in keyof A]: boolean };
        }
    |},
      ),
    ).
      toMatchSnapshot()
  });
  test("maps over extended interfaces", ({expect, _}) => {
    expect.string(
      print(
        ~ctx,
        {|
          interface KeysExt {
            key_3: boolean;
          }
          interface Keys extends KeysExt {
            key_1: string;
            key_2: number;
          }
          type with_keys = { [K in keyof Keys]: boolean };
        |},
      ),
    ).
      toMatchSnapshot()
  });
  test("maps optional types", ({expect, _}) => {
    expect.string(
      print(
        ~ctx,
        {|
          interface Keys {
            key_1: string;
            key_2: number;
          }
          type with_keys = { [K in keyof Keys]?: 'a' | 'b' };
        |},
      ),
    ).
      toMatchSnapshot()
  });
  test("maps over type parameters", ({expect, _}) => {
    expect.string(
      print(
        ~ctx,
        {|
          interface Keys<A> {
            key_1: string;
            key_2: number;
            key_3: A;
          }
          type with_keys = { [K in keyof Keys<{ key_4: boolean }>['key_3']]: boolean };
        |},
      ),
    ).
      toMatchSnapshot()
  });
  test(
    "omits parsing the mapped obj type if its args require it to be parsed on call",
    ({expect, _}) => {
    expect.string(
      print(
        ~ctx,
        {|
          type Pick<T, K extends keyof T> = {
              [P in K]: T[P];
          };
          interface A {
            x: string;
            y: number;
            z: boolean;
          }

          type keys = 'x' | 'y';
          type stripped = Pick<A, keys>;
        |},
      ),
    ).
      toMatchSnapshot()
  });
  test(
    "resolves nested type args when using mapped obj types", ({expect, _}) => {
    expect.string(
      print(
        ~ctx,
        {|
          type Pick<T, K extends keyof T> = {
              [P in K]: T[P];
          };
          interface A<X> {
            x: string;
            y: X;
            z: boolean;
          }

          type keys = 'x' | 'y';
          type stripped = Pick<A<{obj: number}>, keys>;
        |},
      ),
    ).
      toMatchSnapshot()
  });
});

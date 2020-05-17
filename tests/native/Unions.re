open TestFramework;

describe("nullable / undefined unions are around types", ({test, _}) => {
  test("nullable for single base types", ({expect, _}) => {
    expect.string(print({|type null_string = string | null;|})).
      toMatchSnapshot()
  });

  test("undefined for single base types", ({expect, _}) => {
    expect.string(print({|type opt_string = string | undefined;|})).
      toMatchSnapshot()
  });

  test("undefined & nullable can be nested", ({expect, _}) => {
    expect.string(
      print({|type opt_null_string = string | null | undefined;|}),
    ).
      toMatchSnapshot()
  });

  test(
    "nullable/undefined for single base types on record fields",
    ({expect, _}) => {
    expect.string(
      print(
        {|
        type obj = {
            opt_string: string | undefined,
            null_string: string | null
        }
        |},
      ),
    ).
      toMatchSnapshot()
  });

  test("nullable/undefined for single base types in arrays", ({expect, _}) => {
    expect.string(
      print(
        {|
        type opt_in_arr = (string | undefined)[];
        type null_in_arr = Array<boolean | null>;
        type arr_null = string[] | null;
        type arr_opt = Array<boolean> | undefined;
        |},
      ),
    ).
      toMatchSnapshot()
  });
});

describe("string only unions", ({test, _}) => {
  test("can generate simple string unions", ({expect, _}) => {
    expect.string(
      print({|
          type variant = 'str1' | 'Str1' | "X_$STR";
      |}),
    ).
      toMatchSnapshot()
  });
  test("recognizes single string as union", ({expect, _}) => {
    expect.string(print({|
        type variant = 'single';
      |})).
      toMatchSnapshot()
  });
  test("can be generated inline", ({expect, _}) => {
    expect.string(
      print(
        {|
      interface obj {
        field: 'red' | 'blue',
      }
      type in_arr = Array<obj['field']>;
    |},
      ),
    ).
      toMatchSnapshot()
  });
  test("can be generated inside optional", ({expect, _}) => {
    expect.string(
      print({|
      type variant = 'red' | 'blue' | undefined;
    |}),
    ).
      toMatchSnapshot()
  });
});

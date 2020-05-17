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

open TestFramework;

describe("type extractions", ({test, _}) => {
  test("can extract a base type from an obj / interface", ({expect, _}) => {
    expect.string(
      print(
        {|
            type obj = {
                field_1: string,
                field_2: boolean
            };
            type field_1 = obj['field_1'];
            type field_2 = obj["field_2"];
        |},
      ),
    ).
      toMatchSnapshot()
  });

  test("can extract a field from a reference", ({expect, _}) => {
    expect.string(
      print(
        {|
            interface I_int {
                i_field: number;
            }
            type obj = {
                field_1: string,
                if: I_int
            };
            type field_1 = obj['if']['i_field'];
        |},
      ),
    ).
      toMatchSnapshot()
  });

  test("properly converts key names to resolve references", ({expect, _}) => {
    expect.string(
      print(
        {|
            type obj = {
                I_BigField: string
            };
            type field_1 = obj['I_BigField'];
        |},
      ),
    ).
      toMatchSnapshot()
  });

  test("can resolve inline interfaces", ({expect, _}) => {
    expect.string(
      print(
        {|
            type obj = {
                field_1: number,
                field_2: {
                    field_3: boolean
                }
            };
            type field_1 = obj['field_2']['field_3'];
        |},
      ),
    ).
      toMatchSnapshot()
  });

  test("can access recursive references", ({expect, _}) => {
    expect.string(
      print(
        {|
            interface IMain {
              simple: string,
              inline: {
                num: number
              }
            };
            type str = IMain['simple']
            type obj = IMain['inline'];
            type num = obj['num'];
        |},
      ),
    ).
      toMatchSnapshot()
  });

  test("can access a string literal of fields", ({expect, _}) => {
    expect.string(
      print(
        {|
            type c = {a: string, b: number};
            type d = c['a' | 'b']
        |},
      ),
    ).
      toMatchSnapshot()
  });
  test("can create an array after the access", ({expect, _}) => {
    expect.string(
      print(
        {|
            type c = {a: string, b: number};
            type d = c['a'][]
        |},
      ),
    ).
      toMatchSnapshot()
  });
});

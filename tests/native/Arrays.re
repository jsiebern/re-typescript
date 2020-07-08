open TestFramework;

describe("arrays", ({test, _}) => {
  test("arrays in record field defs", ({expect, _}) => {
    expect.string(print({|type obj = { field: Array<string>, }|})).
      toMatchSnapshot();
  });
  test("arrays in record field defs 2", ({expect, _}) => {
    expect.string(print({|type obj = { field: string[] }|})).toMatchSnapshot();
  });

  test("arrays of inline records", ({expect, _}) => {
    expect.string(print({|type x = {field: string}[];|})).toMatchSnapshot()
  });
});

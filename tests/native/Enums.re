open TestFramework;

describe("clean enums", ({test, _}) => {
  test("generates variants from clean enums", ({expect, _}) => {
    expect.string(print({|enum Enum { Red, Green, Blue }|})).toMatchSnapshot()
  });

  test("generates uppercase keys", ({expect, _}) => {
    expect.string(print({|enum Enum { red, Green, blue }|})).toMatchSnapshot()
  });
});

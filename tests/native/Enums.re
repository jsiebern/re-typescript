open TestFramework;

describe("clean enums", ({test, _}) => {
  test("throws for non declared / exported enum", ({expect, _}) => {
    expect.fn(() => print({|enum Enum { Red, Green, Blue }|})).toThrow()
  });

  test("generates variants from clean enums", ({expect, _}) => {
    expect.string(print({|export enum Enum { Red, Green, Blue }|})).
      toMatchSnapshot()
  });

  test("generates uppercase keys", ({expect, _}) => {
    expect.string(print({|declare enum Enum { red, Green, blue }|})).
      toMatchSnapshot()
  });
});

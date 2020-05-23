open TestFramework;

describe("function definitions", ({test, _}) => {
  test("simple function", ({expect, _}) => {
    expect.string(
      print({|
            function someFunction(): void;
        |}),
    ).
      toMatchSnapshot()
  });
  test("function with return type", ({expect, _}) => {
    expect.string(
      print(
        {|
            function someFunction(): string;
            function someOtherFunction(): { inline: string };
        |},
      ),
    ).
      toMatchSnapshot()
  });
  test("function with arguments", ({expect, _}) => {
    expect.string(
      print(
        {|
            function someFunction(a: string, b: number): string;
        |},
      ),
    ).
      toMatchSnapshot()
  });
  test("function with optional arguments", ({expect, _}) => {
    expect.string(
      print(
        {|
            function someFunction(a: string, b?: number): string;
        |},
      ),
    ).
      toMatchSnapshot()
  });
  test("function with inline arguments", ({expect, _}) => {
    expect.string(
      print(
        {|
              function someFunction(a: string | number, b?: { inline: number }): string;
          |},
      ),
    ).
      toMatchSnapshot()
  });
  test("inline obj union as function return type", ({expect, _}) => {
    expect.string(
      print(
        {|
            function someOtherFunction(): null | { inline: string };
        |},
      ),
    ).
      toMatchSnapshot()
  });
  test("untyped type arguments become any", ({expect, _}) => {
    expect.string(
      print({|
            function someFunction(a): string;
        |}),
    ).
      toMatchSnapshot()
  });
  test("untyped return becomes any", ({expect, _}) => {
    expect.string(print({|
            function someFunction();
        |})).
      toMatchSnapshot()
  });
  test("functions accept type parameters", ({expect, _}) => {
    expect.string(
      print(
        {|
            function someFunction<A,B,C>(a: A, b: B): C;
        |},
      ),
    ).
      toMatchSnapshot()
  });
});

describe("inline function definitions", ({test, _}) => {
  test("inline function in an interface", ({expect, _}) => {
    expect.string(
      print(
        {|
            interface iTest {
                field: string;
                action: (a: string, b?: number) => void;
            }
            type access = iTest['action'];
        |},
      ),
    ).
      toMatchSnapshot()
  })
});

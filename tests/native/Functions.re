open TestFramework;

describe("function definitions", ({test, _}) => {
  test("throws for non declared / exported function", ({expect, _}) => {
    expect.fn(() =>
      print({|
            function someFunction(): void;
        |})
    ).
      toThrow()
  });

  test("simple function", ({expect, _}) => {
    expect.string(
      print({|
            export function someFunction(): void;
        |}),
    ).
      toMatchSnapshot()
  });
  test("function with return type", ({expect, _}) => {
    expect.string(
      print(
        {|
            declare function someFunction(): string;
            declare function someOtherFunction(): { inline: string };
        |},
      ),
    ).
      toMatchSnapshot()
  });
  test("function with arguments", ({expect, _}) => {
    expect.string(
      print(
        {|
            export function someFunction(a: string, b: number): string;
        |},
      ),
    ).
      toMatchSnapshot()
  });
  test("function with optional arguments", ({expect, _}) => {
    expect.string(
      print(
        {|
            export function someFunction(a: string, b?: number): string;
        |},
      ),
    ).
      toMatchSnapshot()
  });
  test("function with inline arguments", ({expect, _}) => {
    expect.string(
      print(
        {|
          declare function someFunction(a: string | number, b?: { inline: number }): string;
          |},
      ),
    ).
      toMatchSnapshot()
  });
  test("inline obj union as function return type", ({expect, _}) => {
    expect.string(
      print(
        {|
            export function someOtherFunction(): null | { inline: string };
        |},
      ),
    ).
      toMatchSnapshot()
  });
  test("untyped type arguments become any", ({expect, _}) => {
    expect.string(
      print(
        {|
            declare function someFunction(a): string;
        |},
      ),
    ).
      toMatchSnapshot()
  });
  test("untyped return becomes any", ({expect, _}) => {
    expect.string(
      print({|
           declare function someFunction();
        |}),
    ).
      toMatchSnapshot()
  });
  test("functions accept type parameters", ({expect, _}) => {
    expect.string(
      print(
        {|
           export function someFunction<A,B,C>(a: A, b: B): C;
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
  });
  test("method signature in an interface", ({expect, _}) => {
    expect.string(
      print(
        {|
            interface method_signature {
              func1: (x: number) => number;       // Function type literal
              func2(x: number): number;           // Method signature
              func3(x: string, y: boolean): void; // Method signature for extraction
            }
            type extracted_method_signature = method_signature['func3'];
        |},
      ),
    ).
      toMatchSnapshot()
  });
  test("inline function in a function definition", ({expect, _}) => {
    expect.string(
      print(
        {|
            export function func(a: string, b: (a: string, b?: number) => string): void;
        |},
      ),
    ).
      toMatchSnapshot()
  });
});

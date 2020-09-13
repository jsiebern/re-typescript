open TestFramework;

describe("conditional types", ({test, _}) => {
  test("inline conditional types are printed as unions", ({expect, _}) => {
    expect.string(
      print(
        {|
      declare function f<T extends boolean>(x: T): T extends true ? string : T;
    |},
      ),
    ).
      toMatchSnapshot()
  });
  test("references to conditional types get resolved", ({expect, _}) => {
    expect.string(
      print(
        {|
      type x<a> = a extends string ? string : number;
      type y = x<string>
      type z = x<boolean>
      type m = x<{ inline: boolean }>
    |},
      ),
    ).
      toMatchSnapshot()
  });
  test(
    "conditional types can be chained and the root union reflects that",
    ({expect, _}) => {
    expect.string(
      print(
        {|
      type TypeName<T> = T extends string
        ? "string"
        : T extends number
        ? "number"
        : T extends boolean
        ? "boolean"
        : T extends undefined
        ? "undefined"
        : T extends Function
        ? "function"
        : "object";

      type T0 = TypeName<string>; // "string"
      type T1 = TypeName<"a">; // "string"
      type T2 = TypeName<true>; // "boolean"
      type T3 = TypeName<() => void>; // "function"
      type T4 = TypeName<string[]>; // "object"
    |},
      ),
    ).
      toMatchSnapshot()
  });
  test("unions passed to conditionals get resolved correctly", ({expect, _}) => {
    expect.string(
      print(
        {|
      type TypeName<T> = T extends string
        ? "string"
        : T extends number
        ? "number"
        : T extends boolean
        ? "boolean"
        : T extends undefined
        ? "undefined"
        : T extends Function
        ? "function"
        : "object";

      type T5 = TypeName<string | (() => void)>;
      type T6 = TypeName<string | string[] | undefined>;
      type T7 = TypeName<string[] | number[]>;
    |},
      ),
    ).
      toMatchSnapshot()
  });
  test("distributive conditional types", ({expect, _}) => {
    expect.string(
      print(
        {|
      type BoxedValue<T> = { value: T };
      type BoxedArray<T> = { array: T[] };
      type Boxed<T> = T extends any[] ? BoxedArray<T[number]> : BoxedValue<T>;

      type T1 = Boxed<string>;
      type T2 = Boxed<number[]>;
      type T3 = Boxed<string | number[]>;
    |},
      ),
    ).
      toMatchSnapshot()
  });
});

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
  test("strip away some keys", ({expect, _}) => {
    expect.string(
      print(
        {|
      interface A {
        x: string;
        y: number;
        z: boolean;
      }

      type stripped = Exclude<keyof A, "y">;

      type ReuseStrippedKeys = {
        [key in stripped]: A[key]
      }
    |},
      ),
    ).
      toMatchSnapshot()
  });
  test("More complex example from the TS website", ({expect, _}) => {
    expect.string(
      print(
        {|
      type Diff<T, U> = T extends U ? never : T;
      type Filter<T, U> = T extends U ? T : never;

      type T1 = Diff<"a" | "b" | "c" | "d", "a" | "c" | "f">;
      type T2 = Filter<"a" | "b" | "c" | "d", "a" | "c" | "f">; // "a" | "c"
      type T3 = Diff<string | number | (() => void), Function>; // string | number
      type T4 = Filter<string | number | (() => void), Function>; // () => void

      type NotNullable<T> = Diff<T, null | undefined>;

      type T5 = NotNullable<string | number | undefined>;
      type T6 = NotNullable<string | string[] | null | undefined>;
    |},
      ),
    ).
      toMatchSnapshot()
  });
});

open TestFramework;

describe("type parameters generation & grammar", ({test, _}) => {
  test("prints correct generics for type parameters", ({expect, _}) => {
    expect.string(print({|type with_param<ARG> = ARG|})).toMatchSnapshot()
  });

  test("accepts ts grammar for type parameters", ({expect, _}) => {
    expect.string(
      print(
        {|
            type with_param<a1, a2 extends a1> = { a1: a1, a2: a2 };
            type with_param_2<a1, a2 extends string, a3 = string> = {a1: a1, a2: a2, a3: a3};
            type with_param_3<a1, a2 extends string, a3 extends a2 = string> = {a1: a1, a2: a2, a3: a3};
        |},
      ),
    ).
      toMatchSnapshot()
  });
});

describe("type parameter logic", ({test, _}) => {
  test(
    "does not throw when construcing a faulty type, but will throw on call site",
    ({expect, _}) => {
    expect.string(
      print({|
        type with_param<a1, a2 = string, a3> = a3;
    |}),
    ).
      toMatchSnapshot();
    expect.fn(() =>
      print(
        {|
        type with_param<a1, a2 = string, a3> = a3;
        type call_params = with_param<string, number, boolean>;
    |},
      )
    ).
      toThrow();
  });
  test("can have inline records as defaults", ({expect, _}) => {
    expect.string(
      print(
        {|
        type with_param<a1, a2 = {inline: boolean}> = {a1:a1, a2:a2};
        type call_params = with_param<string>;
    |},
      ),
    ).
      toMatchSnapshot()
  });
  test("can have inline records at call site", ({expect, _}) => {
    expect.string(
      print(
        {|
        type with_param<a1, a2 = {inline: boolean}> = {a1:a1, a2:a2};
        type call_params = with_param<{another_inline: number}>;
    |},
      ),
    ).
      toMatchSnapshot()
  });
  test(
    "still generates default inline values, even if overriden", ({expect, _}) => {
    expect.string(
      print(
        {|
        type with_param<a1, a2 = {inline: boolean}> = {a1:a1, a2:a2};
        type call_params = with_param<string, number>;
    |},
      ),
    ).
      toMatchSnapshot()
  });
  test("can use various default structures", ({expect, _}) => {
    expect.string(
      print(
        {|
        type with_param<a = [string, number]> = a;
        type call_params = with_param;
    |},
      ),
    ).
      toMatchSnapshot()
  });
  test("can have nested default values", ({expect, _}) => {
    expect.string(
      print(
        {|
        type orig_param<a = string> = a;
        type with_param<a = { b: orig_param<number> }> = a;
        type call_params = with_param;
    |},
      ),
    ).
      toMatchSnapshot()
  });

  test("default parameter can be a union of any kind", ({expect, _}) => {
    expect.string(
      print(
        {|
      type with_param2<a = 'one' | 'two'> = a;
      type call_params2 = with_param2;
    |},
      ),
    ).
      toMatchSnapshot();
    expect.string(
      print(
        {|
      type with_param2<a = string | number> = a;
      type call_params2 = with_param2;
    |},
      ),
    ).
      toMatchSnapshot();
  });
  test(
    "references parameters that are used when inlining a type", ({expect, _}) => {
    expect.string(
      print(
        {|
      export function someFunction<A,B,C>(a: A, b: B): { b: B, c: C };
    |},
      ),
    ).
      toMatchSnapshot()
  });
});

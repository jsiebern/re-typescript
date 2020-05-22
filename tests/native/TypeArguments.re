open TestFramework;

describe("type arguments generation & grammar", ({test, _}) => {
  test("prints correct generics for type arguments", ({expect, _}) => {
    expect.string(print({|type with_arg<ARG> = ARG|})).toMatchSnapshot()
  });

  test("accepts ts grammar for type arguments", ({expect, _}) => {
    expect.string(
      print(
        {|
            type with_arg<a1, a2 extends a1> = { a1: a1, a2: a2 };
            type with_arg_2<a1, a2 extends string, a3 = string> = {a1: a1, a2: a2, a3: a3};
            type with_arg_3<a1, a2 extends string, a3 extends a2 = string> = {a1: a1, a2: a2, a3: a3};
        |},
      ),
    ).
      toMatchSnapshot()
  });
});

describe("type argument logic", ({test, _}) => {
  test(
    "does not throw when construcing a faulty type, but will throw on call site",
    ({expect, _}) => {
    expect.string(
      print({|
        type with_arg<a1, a2 = string, a3> = a3;
    |}),
    ).
      toMatchSnapshot();
    expect.fn(() =>
      print(
        {|
        type with_arg<a1, a2 = string, a3> = a3;
        type call_args = with_arg<string, number, boolean>;
    |},
      )
    ).
      toThrow();
  });
  test("can have inline records as defaults", ({expect, _}) => {
    expect.string(
      print(
        {|
        type with_arg<a1, a2 = {inline: boolean}> = {a1:a1, a2:a2};
        type call_args = with_arg<string>;
    |},
      ),
    ).
      toMatchSnapshot()
  });
  test("can have inline records at call site", ({expect, _}) => {
    expect.string(
      print(
        {|
        type with_arg<a1, a2 = {inline: boolean}> = {a1:a1, a2:a2};
        type call_args = with_arg<{another_inline: number}>;
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
        type with_arg<a1, a2 = {inline: boolean}> = {a1:a1, a2:a2};
        type call_args = with_arg<string, number>;
    |},
      ),
    ).
      toMatchSnapshot()
  });
  test("can use various default structures", ({expect, _}) => {
    expect.string(
      print(
        {|
        type with_arg<a = [string, number]> = a;
        type call_args = with_arg;
    |},
      ),
    ).
      toMatchSnapshot()
  });
  test("can have nested default values", ({expect, _}) => {
    expect.string(
      print(
        {|
        type orig_arg<a = string> = a;
        type with_arg<a = { b: orig_arg<number> }> = a;
        type call_args = with_arg;
    |},
      ),
    ).
      toMatchSnapshot()
  });

  test("default argument can be a union of any kind", ({expect, _}) => {
    expect.string(
      print(
        {|
      type with_arg2<a = 'one' | 'two'> = a;
      type call_args2 = with_arg2;
    |},
      ),
    ).
      toMatchSnapshot();
    expect.string(
      print(
        {|
      type with_arg2<a = string | number> = a;
      type call_args2 = with_arg2;
    |},
      ),
    ).
      toMatchSnapshot();
  });
});

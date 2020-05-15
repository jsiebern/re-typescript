open TestFramework;
open Re_typescript_base;

let config = Re_typescript_printer.Config.defaultConfig;
let print = (~ctx=config, value) =>
  Re_typescript_printer.print_from_ts(
    ~ctx,
    Parser.main(Lexer.read, Lexing.from_string(value)),
  );

/********************************************
  Basic type syntax
 ********************************************/
describe("basic syntax for types", ({test, _}) => {
  test("basic type definition", ({expect, _}) => {
    expect.string(print("type t_string = string;")).toMatchSnapshot();
    expect.string(print("type t_number = number;")).toMatchSnapshot();
    expect.string(print("type t_bool = boolean;")).toMatchSnapshot();
  });

  test("basic type definition without semicolon", ({expect, _}) => {
    expect.string(print("type t_string = string")).toMatchSnapshot()
  });
});

/********************************************
  Config Flags
 ********************************************/
describe("config flags effects", ({test, _}) => {
  test("number_mode default value", ({expect, _}) => {
    let ts = "type t_number = number";
    expect.string(print(ts)).toMatch(
      print(~ctx={...config, number_mode: Float}, ts),
    );
  });

  test("number_mode", ({expect, _}) => {
    let ts = "type t_number = number";
    expect.string(print(~ctx={...config, number_mode: Float}, ts)).
      toMatchSnapshot();
    expect.string(print(~ctx={...config, number_mode: Int}, ts)).
      toMatchSnapshot();
    // Unboxed not implemented yet
    expect.fn(() => print(~ctx={...config, number_mode: Unboxed}, ts)).
      toThrow();
  });
});

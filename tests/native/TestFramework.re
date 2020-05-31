include Rely.Make({
  let config =
    Rely.TestFrameworkConfig.initialize({
      snapshotDir: "tests/native/__snapshots__",
      projectDir: "src",
    });
});

open Re_typescript_base;

let config = Re_typescript_printer.Config.default_config;
let print = (~ctx=config, value) =>
  Re_typescript_printer.print_from_ts(
    ~ctx,
    Parser_incr.parse(Lexing.from_string(value)),
  );

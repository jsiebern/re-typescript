include Rely.Make({
  let config =
    Rely.TestFrameworkConfig.initialize({
      snapshotDir: "tests/native/__snapshots__",
      projectDir: "src",
    });
});

open Re_typescript_base;
module Exceptions = Re_typescript_printer.Tree_utils.Exceptions;

let config = Re_typescript_printer.Config.default_config;
let print = (~ctx=config, value) => {
  let str =
    Re_typescript_printer.structure_from_ts(
      ~ctx,
      Parser_incr.parse(Lexing.from_string(value)),
    );
  Reason_toolchain.RE.print_implementation_with_comments(
    Format.str_formatter,
    (str, []),
  );
  Format.flush_str_formatter();
};

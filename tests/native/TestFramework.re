include Rely.Make({
  let config =
    Rely.TestFrameworkConfig.initialize({
      snapshotDir: "tests/native/__snapshots__",
      projectDir: "src",
    });
});

open Re_typescript_base;
open Re_typescript_fs;
module Exceptions = Re_typescript_printer.Tree_utils.Exceptions;

type x = {x: Hashtbl.t(Fp.t(Fp.absolute), string)};
let x = {x: Hashtbl.create(0)};
let test_loader: module Loader.T =
  (module
   Loader_virtual.Make({
     let tbl = x.x;
   }));
let test_resolver: module Resolver.T =
  (module
   Resolver.Make({
     let config = {Resolver.loader: test_loader, tsconfig: None};
   }));
let test_path = Fp.absoluteExn("/test_framework.d.ts");

let config = Re_typescript_config.default_config;
let print = (~ctx=config, value) => {
  Hashtbl.replace(x.x, test_path, value);

  let str =
    Re_typescript_printer.structure_from_ts(
      ~ctx,
      ~parser=
        content =>
          Ok(
            Parser_incr.parse(Lexing.from_string(content |> CCString.trim)),
          ),
      ~resolver=test_resolver,
      test_path,
    );
  Reason_toolchain.RE.print_implementation_with_comments(
    Format.str_formatter,
    (str, []),
  );
  Format.flush_str_formatter();
};

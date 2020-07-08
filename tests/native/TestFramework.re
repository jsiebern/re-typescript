open Lwt.Infix;
open Websocket;

include Rely.Make({
  let config =
    Rely.TestFrameworkConfig.initialize({
      snapshotDir: "tests/native/__snapshots__",
      projectDir: "src",
    });
});

module Impl =
  Re_typescript_ws_client.WsClient(
    {
      let uri = Uri.of_string("http://127.0.0.1:83");
    },
    {
      let onStateChange = _ => ();
    },
  );

type connection = {
  mutable close: unit => Lwt.t(unit),
  mutable parse_file: string => Result.t(list((string, string)), string),
  mutable is_connected: bool,
};
let connection = {
  close: () => Lwt.return_unit,
  parse_file: _ => Ok([]),
  is_connected: false,
};

let config = Re_typescript_config.default_config;
let print = (~ctx=config, value) => {
  if (!connection.is_connected) {
    let (send, recv, close) = Lwt_main.run(Impl.connect());
    connection.close = close;
    connection.parse_file = (
      value =>
        Lwt_main.run(
          send(`QuickParse(("/test_framework.d.ts", value)))
          >>= recv
          >>= (
            message =>
              switch (message) {
              | Some(`ParseOk(parsed)) => Lwt.return(Ok(parsed))
              | Some(`ParseError(e)) => Lwt.return(Error(e))
              | Some(_) => Lwt.fail_invalid_arg("Unexpected message type")
              | None => Lwt.fail(Not_found)
              }
          ),
        )
    );
    connection.is_connected = true;
  };
  let files = connection.parse_file(value);

  switch (files) {
  | Error(e) => raise(Failure(e))
  | Ok(files) =>
    let str = Re_typescript_ts_parser.structure_from_ts(~ctx, files);
    Reason_toolchain.RE.print_implementation_with_comments(
      Format.str_formatter,
      (str, []),
    );
    Format.flush_str_formatter();
  };
};

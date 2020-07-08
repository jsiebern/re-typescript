open Lwt.Infix;
open Re_typescript_base;
open Re_typescript_fs;

let file = (
  "/root/src/bin.d.ts",
  {|
export interface Map<A,B> {
  a: A;
  b: B;
}
export interface RecoilRootProps {
  initializeState?: (options: {
    set: <T>(recoilVal: T, newVal: T) => void; // Ignores type params on inline functions
    setUnvalidatedAtomValues: (atomMap: Map<string, any>) => void;
  }) => void;
}
  |},
);

module Impl =
  Re_typescript_ws_client.WsClient(
    {
      let uri = Uri.of_string("http://127.0.0.1:82");
    },
    {
      let onStateChange = _ => ();
    },
  );

let (send, recv, close) = Lwt_main.run(Impl.connect());
let files =
  Lwt_main.run(
    send(`QuickParse(file))
    >>= recv
    >>= (
      message =>
        close()
        >>= (
          () =>
            switch (message) {
            | Some(`ParseOk(parsed)) =>
              Lwt_io.lines_to_file(
                (
                  Sys.getenv("PROJECT_ROOT")
                  |> CCString.split_on_char(':')
                  |> CCList.get_at_idx_exn(0)
                )
                ++ "/_current.json",
                parsed
                |> CCList.get_at_idx_exn(0)
                |> snd
                |> Yojson.Basic.from_string
                |> Yojson.Basic.pretty_to_string
                |> CCString.lines
                |> Lwt_stream.of_list,
              )
              >>= (_ => Lwt.return(parsed))
            | Some(`ParseError(e)) =>
              Console.log("\n\n" ++ e ++ "\n\n");
              Lwt.fail_with("ParseError");
            | Some(_) => Lwt.fail_invalid_arg("Unexpected message type")
            | None => Lwt.fail(Not_found)
            }
        )
    ),
  );

let () =
  try(
    {
      print_newline();
      Random.self_init();
      Console.log((
        Random.int(1000000),
        Random.int(1000000),
        Random.int(1000000),
        Random.int(1000000),
      ));
      Console.log("--------------------------------------");
      let str =
        Re_typescript_ts_parser.structure_from_ts(
          ~ctx={
            ...Re_typescript_config.default_config,
            number_mode: Int,
            omit_extended_unreferenced_records: true,
            bucklescript_config: {
              ...Re_typescript_config.default_bucklescript_config,
              string_variant_mode: `Variant,
            },
            output_type: Bucklescript,
          },
          files,
        );
      Reason_toolchain.RE.print_implementation_with_comments(
        Format.str_formatter,
        (str, []),
      );
      Console.log(Format.flush_str_formatter());

      Console.log("--------------------------------------");
      print_newline();
    }
  ) {
  | Syntaxerr.Error(msg) => Console.error(msg)
  | Lexer.SyntaxError(msg) => Console.error(msg)
  | Re_typescript_printer.Tree_utils.Exceptions.Parser_unsupported(msg, pos) =>
    Console.error(Error.parser_error_with_info(~msg, ~content="", pos))
  | Parser_incr.Parsing_error(pos) =>
    Console.error(Error.parser_error_with_info(~content="", pos))
  | Parser.Error =>
    let content = "";
    let lexbuf = Lexing.from_string("");
    Console.error(
      Error.parser_error(
        ~content,
        ~start=lexbuf.lex_start_p,
        ~end_=lexbuf.lex_curr_p,
      ),
    );
  | e =>
    Console.error(e);
    raise(e);
  };

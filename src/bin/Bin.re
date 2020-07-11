open Lwt.Infix;
open Re_typescript_base;
open Re_typescript_fs;

let file = (
  "/root/src/bin.d.ts",
  {|
// type c = {a: string, b: number};
// type d = c['a' | 'b']

// interface IMain {
//   simple: string,
//   inline: {
//     num: number
//   }
// }
// type str = IMain['simple']
// type obj = IMain['inline'];
// type num = obj['num'];

// --- can be generated inline
// interface obj {
//   field: 'red' | 'blue',
// }
// type in_arr = Array<obj['field']>;


// --- can use recursion in nested union types
type ValueOrObj<T> = T | { obj_value: T };
// type rec_y = string | { inline_obj: rec_y };
// type rec_y<T> = T | { inline_obj: rec_y<T> };


// module ValueOrObj: {
//   type t('T)
//   and valueOrObj_1('T) = {obj_value: 'T};
//   let t: 'T => t('T);
//   let valueOrObj_1: valueOrObj_1('T) => t('T);
// } = {
//   [@unboxed]
//   type t('T) =
//     | Any('a): t('T)
//   and valueOrObj_1('T) = {obj_value: 'T};
//   let t = (v: 'T) => Any(v);
//   let valueOrObj_1 = (v: valueOrObj_1('T)) => Any(v);
// };
// type valueOrObj('T) = ValueOrObj.t('T);



|},
);

/*let process =
  Lwt_main.run(
    {
      let rec readStart = process => {
        let lines = Lwt_io.read_lines(process#stdout);
        Lwt.pick([
          Lwt_unix.sleep(1.0) >>= (_ => Lwt.return_none),
          lines |> Lwt_stream.peek,
        ])
        >>= (
          ln =>
            switch (ln) {
            | Some(ln) when CCString.find(~sub="Server: Running", ln) > (-1) =>
              process |> Lwt.return
            | ln =>
              switch (ln) {
              | None => ()
              | Some(ln) => Console.log("> " ++ ln)
              };
              readStart(process);
            }
        );
      };
      let process =
        Lwt_process.open_process_full(
          Lwt_process.shell("$(fnm exec -- which node) src_ws/service.bs.js 84"),
        );
      let rec readStdErr = process =>
          {
            Lwt.map(ln => (process, ln), Lwt_io.read_line(process#stderr));
          }
          >>= (
            ((process, ln)) => {
              Console.log(ln);
              readStdErr(process);
            }
          );
        Lwt.async(() =>
        Lwt.catch(() =>
          readStdErr(process) >>= (_ => Lwt.return_unit),
          _ => Lwt.return_unit
          )
        );
      Lwt_process.exec(("clear", [||])) >>= (_ => readStart(process));
    },
  );*/

module Impl =
  Re_typescript_ws_client.WsClient(
    {
      let uri = Uri.of_string("http://127.0.0.1:84");
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

//process#terminate;
//Lwt_main.run(Lwt_unix.waitpid([], process#pid));

open Lwt.Infix;
open Re_typescript_base;
open Re_typescript_fs;

let file = (
  "/root/src/bin.d.ts",
  {|
// interface IPromise<X> {
//   field: X;
// }

// type ResouceResult<T> = T & {
//   promise: IPromise<T>;
//   resolved: boolean;
// };


// interface i_1<C, A = string> { field1: A, fieldx: C };
//         interface i_2<B, A, B> extends i_1<A> { field2: B }
//         type x = i_2<boolean, string>;
//         type y = i_1<string>;






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
            | Some(`ParseOk(parsed)) => Lwt.return(parsed)
            | Some(`ParseError(e)) =>
              Console.log("\n\n" ++ e ++ "\n\n");
              Lwt.fail_with("ParseError");
            | Some(_) => Lwt.fail_invalid_arg("Unexpected message type")
            | None => Lwt.fail(Not_found)
            }
        )
    ),
  );

// module SS = CCSet.Make(String);
// type res = {
//   client_id: string,
//   files: list((string, string)),
//   files_done: SS.t,
// };
// exception Done(res);
// let parse_files = () => {
//   let (send, recv, close) = Lwt_main.run(Impl.connect());
//   send(`Initialize) |> Lwt.ignore_result;

//   let rec recv_forever = (res: res) =>
//     recv()
//     >>= (
//       maybeValue =>
//         {
//           switch (maybeValue) {
//           | Some(`Initialized(client_id)) =>
//             send(
//               `CreateFile((
//                 client_id,
//                 res.files |> CCList.get_at_idx_exn(0) |> fst,
//               )),
//             )
//             |> Lwt.map(_ => {...res, client_id})
//           | Some(`FileCreated(path)) =>
//             send(
//               `SetFileContents((
//                 res.client_id,
//                 path,
//                 res.files |> CCList.find(((p, _)) => p == path) |> snd,
//               )),
//             )
//             |> Lwt.map(_ => res)
//           | Some(`FileContentsOk(path)) =>
//             let res = {...res, files_done: res.files_done |> SS.add(path)};
//             switch (
//               res.files
//               |> CCList.find_opt(((p, _)) => !SS.mem(p, res.files_done))
//             ) {
//             | None => send(`Parse(res.client_id)) |> Lwt.map(_ => res)
//             | Some((file, _)) =>
//               send(`CreateFile((res.client_id, file))) |> Lwt.map(_ => res)
//             };
//           | Some(`ParseOk(files)) => Lwt.fail(Done({...res, files}))
//           | _ => Lwt.return(res)
//           };
//         }
//         >>= recv_forever
//     );
//   try(
//     Lwt_main.run(recv_forever({client_id: "", files, files_done: SS.empty}))
//   ) {
//   | Done(res) =>
//     send(`Destroy(res.client_id)) >>= (() => close()) |> Lwt.ignore_result;

//     res;
//   };
// };

// let res = parse_files();

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

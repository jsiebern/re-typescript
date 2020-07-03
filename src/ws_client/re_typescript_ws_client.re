open Lwt.Infix;
open Websocket;
open Websocket_lwt_unix;

Lwt_log.default :=
  Lwt_log.channel(
    ~template="$(date).$(milliseconds) [$(level)] $(message)",
    ~close_mode=`Keep,
    ~channel=Lwt_io.stdout,
    (),
  );

let wsSendMessage = ref(_ => ());
let wsRecMessage = ref(_ => ());
let wsIsReady = ref(() => ());

module Types = Re_typescript_ws_types;
module Client: Types.Client_interface.T = {
  let client_id = ref("");
  let files = ref([]);

  let sendMessage = (msg: Types.Ws_messages.client_msg) =>
    wsSendMessage^(msg |> Types.Ws_messages.string_of_client_msg);
  let receiveMessage = (message: Types.Ws_messages.server_msg) => {
    switch (message) {
    | `Initialized(id) =>
      client_id := id;
      Console.log(
        Printf.sprintf("Got client_id: %s, creating a file...", id),
      );
      sendMessage(`CreateFile((client_id^, "/path/to/file.d.ts")));
    | `FileCreated(id) =>
      files := files^ @ [(id, "/path/to/file.d.ts")];
      Console.log(
        Printf.sprintf("Got file: %s, putting some contents...", id),
      );
      sendMessage(`SetFileContents((client_id^, id, "type x = string;")));
    | `FileContentsOk(id) =>
      Console.log(Printf.sprintf("File contents created ok for: %s", id))
    };
  };
  wsRecMessage :=
    (msg => receiveMessage(msg |> Types.Ws_messages.server_msg_of_string));

  let isReady = () => {
    sendMessage(`Initialize);
  };
  wsIsReady := (() => isReady());
};

let section = Lwt_log.Section.make("ws_client");

let client = uri =>
  Frame.(
    Resolver_lwt.resolve_uri(~uri, Resolver_lwt_unix.system)
    >>= (
      endp =>
        Conduit_lwt_unix.(
          endp_to_client(~ctx=default_ctx, endp)
          >>= (client => with_connection(~ctx=default_ctx, client, uri))
        )
        >>= (
          ((recv, send)) => {
            wsSendMessage :=
              (content => send(Frame.create(~content, ())) |> ignore);

            let react = fr =>
              switch (fr.opcode) {
              | Opcode.Ping => send @@ Frame.create(~opcode=Opcode.Pong, ())

              | Opcode.Close =>
                /* Immediately echo and pass this last message to the user */
                (
                  if (String.length(fr.content) >= 2) {
                    send @@
                    Frame.create(
                      ~opcode=Opcode.Close,
                      ~content=String.sub(fr.content, 0, 2),
                      (),
                    );
                  } else {
                    send @@ Frame.close(1000);
                  }
                )
                >>= (() => Lwt.fail(Exit))

              | Opcode.Pong => Lwt.return_unit

              | Opcode.Text
              | Opcode.Binary =>
                wsRecMessage^(fr.content);
                Lwt.return_unit;
              // Lwt_io.printf("> %s\n> %!", fr.content);
              | _ => send @@ Frame.close(1002) >>= (() => Lwt.fail(Exit))
              };

            let rec react_forever = () => recv() >>= react >>= react_forever;
            let rec pushf = () =>
              Lwt_io.(read_line_opt(stdin))
              >>= (
                fun
                | None =>
                  Lwt_log.debug(~section, "Got EOF. Sending a close frame.")
                  >>= (() => send @@ Frame.close(1000) >>= pushf)
                | Some(content) => {
                    send @@ Frame.create(~content, ()) >>= pushf;
                  }
              );
            wsIsReady^();
            pushf() <?> react_forever();
          }
        )
    )
  );

let apply_loglevel =
  fun
  | 2 => Lwt_log.(add_rule("*", Info))
  | 3 => Lwt_log.(add_rule("*", Debug))
  | _ => ();

let () = {
  apply_loglevel(3);
  Lwt_main.run(client(Uri.of_string("http://127.0.0.1:81")));
};

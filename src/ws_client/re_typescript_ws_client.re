open Lwt.Infix;
open Websocket;
open Websocket_lwt_unix;
module Types = Re_typescript_ws_types;

Lwt_log.default :=
  Lwt_log.channel(
    ~template="$(date).$(milliseconds) [$(level)] $(message)",
    ~close_mode=`Keep,
    ~channel=Lwt_io.stdout,
    (),
  );

module WsClient = (Uri: {let uri: Uri.t;}) : Types.Client_interface.T =>
  (Config: Types.Client_interface.Config) => {
    Config.onStateChange(Init);

    let section = Lwt_log.Section.make("ws_client");

    let uri = Uri.uri;

    let uri_resolved =
      Resolver_lwt.resolve_uri(~uri, Resolver_lwt_unix.system);

    let connect = () => {
      Config.onStateChange(Connecting);

      let client =
        uri_resolved
        >>= (
          endpoint =>
            Conduit_lwt_unix.(endp_to_client(~ctx=default_ctx, endpoint))
        );
      let connection =
        client
        >>= Conduit_lwt_unix.(
              client => with_connection(~ctx=default_ctx, client, uri)
            );
      connection
      >>= (
        ((recv, send)) => {
          Config.onStateChange(Connected);

          let sendMessage: Types.Ws_messages.client_msg => Lwt.t(unit) =
            message => {
              let jsonOfMessage =
                Types.Ws_messages.string_of_client_msg(message);
              send(Frame.create(~content=jsonOfMessage, ()));
            };

          let reactToMessage = (send, frame) => {
            Frame.(
              switch (frame.opcode) {
              | Opcode.Ping =>
                send(Frame.create(~opcode=Opcode.Pong, ()))
                |> Lwt.map(_ => None)
              | Opcode.Close =>
                /* Immediately echo and pass this last message to the user */
                (
                  if (String.length(frame.content) >= 2) {
                    send(
                      Frame.create(
                        ~opcode=Opcode.Close,
                        ~content=String.sub(frame.content, 0, 2),
                        (),
                      ),
                    );
                  } else {
                    send(Frame.close(1000));
                  }
                )
                >>= (() => Lwt.fail(Exit))

              | Opcode.Pong => Lwt.return_none

              | Opcode.Text
              | Opcode.Binary =>
                Lwt.return_some(
                  frame.content |> Types.Ws_messages.server_msg_of_string,
                )
              | _ => send @@ Frame.close(1002) >>= (() => Lwt.fail(Exit))
              }
            );
          };

          let receive = () => recv() >>= reactToMessage(send);
          Lwt.return((sendMessage, receive));
        }
      );
    };
  };

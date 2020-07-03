type state =
  | Init
  | Connecting
  | Connected
  | Closed;

module type Config = {let onStateChange: state => unit;};

module type T =
  (Config) =>
   {
    let connect:
      unit =>
      Lwt.t(
        (
          Ws_messages_t.client_msg => Lwt.t(unit),
          unit => Lwt.t(option(Ws_messages_t.server_msg)),
        ),
      );
    // let onStateChange: (state => unit) => unit;
    // let isReady: unit => Lwt.t(unit);
  };

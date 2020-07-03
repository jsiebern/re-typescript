module type T = {
  let sendMessage: Ws_messages_t.client_msg => unit;
  let receiveMessage: Ws_messages_t.server_msg => unit;
  let isReady: unit => unit;
};

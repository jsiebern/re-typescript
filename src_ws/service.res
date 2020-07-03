open Ws

let wss = Server.make({port: 81})

wss->Server.onConnection(ws => {
  let reply = (msg: Ws_messages_bs.server_msg) => {
    let asJson =
      msg |> Ws_messages_bs.write_server_msg |> Js.Json.stringifyAny |> Belt.Option.getExn
    ws->Client.send(asJson)
  }

  ws->Client.onMessage(message => {
    switch message |> Js.Json.parseExn |> Ws_messages_bs.read_client_msg {
    | #Initialize => reply(#Initialized(Clients.create()))
    | #CreateFile((client_id, filePath)) =>
      reply(#FileCreated(Clients.createFile(~client_id, filePath)))
    | #SetFileContents((client_id, file_path, contents)) =>
      reply(#FileContentsOk(Clients.setFileContents(~client_id, ~file_path, contents)))
    | #Parse(client_id) => reply(#ParseOk(Clients.parse(~client_id)))
    | #Destroy(client_id) => Clients.destroy(~client_id)
    }
  })
})

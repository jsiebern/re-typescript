open Ws

let port = Process.argv->Belt.Array.get(2)->Belt.Option.map(int_of_string)->Belt.Option.getWithDefault(82)

let wss = Server.make({port: port})
let address = wss->Server.address
let log = msg => Js.log(`> Server: ${msg}`)
let logClient = (id, msg) => log(`Client${id != "" ? `(${id})` : ""} -> ${msg}`)
let logClientMsg = (id, msg: Ws_messages_bs.client_msg) =>
  logClient(
    id,
    switch msg {
    | #Initialize => "Initialize"
    | #CreateFile((client_id, file_path)) => `CreateFile("${client_id}", "${file_path}")`
    | #SetFileContents((client_id, file_path, _)) =>
      `SetFileContents("${client_id}", "${file_path}")`
    | #Parse(client_id) => `Parse("${client_id}")`
    | #Destroy(client_id) => `Destroy("${client_id}")`
    | #CreateFileWithConent((client_id, file_path, _)) =>
      `CreateFileWithConent("${client_id}", "${file_path}")`
    | #UpdateFile((client_id, file_path, _)) => `UpdateFile("${client_id}", "${file_path}")`
    | #DeleteFile((client_id, file_path)) => `DeleteFile("${client_id}", "${file_path}")`
    | #QuickParse((file_path, _)) => `QuickParse("${file_path}")`
    },
  )
log(`Running on: ${address.address}:${address.port->string_of_int} (${address.family})`)

module ClientSet = {
  module T = Belt.Id.MakeComparable({
    type t = Client.t
    let cmp = (a, b) => {
      compare(a->Client.getUniqueId, b->Client.getUniqueId)
    }
  })

  let empty = Belt.Set.make(~id=module(T))
}

wss->Server.onError(exn => {
  let name = exn->Js.Exn.name->Belt.Option.getWithDefault("n/a")
  let message = exn->Js.Exn.message->Belt.Option.getWithDefault("n/a")
  log(`Server error: ${message} (${name})`)
})

let clients = ref(ClientSet.empty)
wss->Server.onConnection(ws => {
  logClient("", "Connected")
  let uniqeId = Shortid.generate()
  ws->Client.setUniqueId(uniqeId)
  clients := clients.contents->Belt.Set.add(ws)
  logClient("", `Assigned id "${uniqeId}"`)
  let logClient = logClient(uniqeId)
  let logClientMsg = logClientMsg(uniqeId)

  let reply = (msg: Ws_messages_bs.server_msg) => {
    let asJson =
      msg |> Ws_messages_bs.write_server_msg |> Js.Json.stringifyAny |> Belt.Option.getExn
    ws->Client.send(asJson)
  }

  ws->Client.onClose((code, reason) => {
    Clients.destroy(~client_id=ws->Client.getUniqueId)
    clients := clients.contents->Belt.Set.remove(ws)
    logClient(`Connection closed: ${reason} (${code->string_of_int})`)
  })

  ws->Client.onError(exn => {
    Clients.destroy(~client_id=ws->Client.getUniqueId)
    clients := clients.contents->Belt.Set.remove(ws)

    let name = exn->Js.Exn.name->Belt.Option.getWithDefault("n/a")
    let message = exn->Js.Exn.message->Belt.Option.getWithDefault("n/a")
    logClient(`Error: ${message} (${name})`)
  })

  ws->Client.onMessage(message => {
    let msg = message |> Js.Json.parseExn |> Ws_messages_bs.read_client_msg
    logClientMsg(msg)
    switch msg {
    | #Initialize => reply(#Initialized(Clients.create(~client_id=ws->Client.getUniqueId)))
    | #CreateFile((client_id, file_path)) =>
      reply(#FileCreated(Clients.createFile(~client_id, file_path)))
    | #SetFileContents((client_id, file_path, contents)) =>
      reply(#FileContentsOk(Clients.setFileContents(~client_id, ~file_path, contents)))
    | #Parse(client_id) =>
      switch Clients.parse(~client_id) {
      | Ok(file_list) => reply(#ParseOk(file_list))
      | Error(e) => reply(#ParseError(e))
      }
    | #Destroy(client_id) => Clients.destroy(~client_id)
    | #CreateFileWithConent((client_id, file_path, contents)) =>
      let file_path = Clients.createFile(~client_id, file_path)
      reply(#FileContentsOk(Clients.setFileContents(~client_id, ~file_path, contents)))
    | #UpdateFile((client_id, file_path, contents)) =>
      reply(#FileUpdated(Clients.setFileContents(~client_id, ~file_path, contents)))
    | #DeleteFile((client_id, file_path)) =>
      reply(#FileUpdated(Clients.deleteFile(~client_id, file_path)))
    | #QuickParse((file_path, contents)) =>
      switch Clients.quickParse(~file_path, contents) {
      | Ok(file_list) => reply(#ParseOk(file_list))
      | Error(e) => reply(#ParseError(e))
      }
    }
  })
})

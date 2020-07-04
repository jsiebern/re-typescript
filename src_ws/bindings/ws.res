module Client = {
  type t

  @bs.send
  external onMessage: (t, @bs.as("message") _, string => unit) => unit = "on"
  @bs.send
  external onClose: (t, @bs.as("close") _, (int, string) => unit) => unit = "on"
  @bs.send
  external onError: (t, @bs.as("error") _, Js.Exn.t => unit) => unit = "on"
  @bs.send
  external onOpen: (t, @bs.as("open") _, unit => unit) => unit = "on"

  @bs.send external send: (t, string) => unit = "send"

  // Non-Standard
  @bs.set external setUniqueId: (t, string) => unit = "uniqueId"
  @bs.get external getUniqueId: t => string = "uniqueId"
}

module Server = {
  type config = {port: int}
  type address = {
    address: string,
    family: string,
    port: int,
  }

  type t

  @bs.new @bs.module("ws") external make: config => t = "Server"

  @bs.send external address: t => address = "address"
  @bs.send
  external onConnection: (t, @bs.as("connection") _, Client.t => unit) => unit = "on"
  @bs.send
  external onError: (t, @bs.as("error") _, Js.Exn.t => unit) => unit = "on"
}

module Client = {
  type t

  @bs.send
  external onMessage: (t, @bs.as("message") _, string => unit) => unit = "on"

  @bs.send external send: (t, string) => unit = "send"
}

module Server = {
  type config = {port: int}

  type t

  @bs.new @bs.module("ws") external make: config => t = "Server"

  @bs.send
  external onConnection: (t, @bs.as("connection") _, Client.t => unit) => unit = "on"
}

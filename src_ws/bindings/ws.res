module Server = {
    type config = {
        port: int,
    };

    type t;

    @bs.new @bs.module("ws") external make: config => t = "Server";

    @bs.send
    external onMessage: (t, @bs.as("message") _, t => unit) => unit = "on";
    @bs.send
    external onConnection: (t, @bs.as("connection") _, t => unit) => unit = "on";
}
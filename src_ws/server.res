open Ws.Server;

let wss = make({ port: 81 });

wss->onConnection(ws => {

    Js.log("connected");
    Js.log(ws);
});
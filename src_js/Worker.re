[@bs.val]
[@bs.module "./../../../_build/default/src/js/re_typescript_js.bc.js"]
external run: string => string = "run";

WebWorkers.setWorkerOnMessage(
  WebWorkers.self,
  (e: WebWorkers.MessageEvent.t) => {
    let data = WebWorkers.MessageEvent.data(e);

    run(data |> Bridge_bs.write_parse_request |> Js.Json.stringify)
    |> Js.Json.parseExn
    |> Bridge_bs.read_parse_result
    |> WebWorkers.postMessageFromWorker;
  },
);
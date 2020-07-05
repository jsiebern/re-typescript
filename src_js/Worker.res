@bs.val
@bs.module("./../_build/default/src/js/re_typescript_js.bc.js")
external run: string => string = "run";
@bs.val
@bs.module("./../_build/default/src/js/re_typescript_js.bc.js")
external example_list: unit => string = "example_list";

WebWorkers.setWorkerOnMessage(
  WebWorkers.self,
  (e: WebWorkers.MessageEvent.t) => {
    let data = WebWorkers.MessageEvent.data(e);
    switch (data) {
    | Bridge_t.Parse(parse_request) =>
      let json = Clients.quickParse(~file_path=parse_request.file_path, parse_request.content)->Belt.Result.getExn;
      let parse_request = {
        ...parse_request,
        content: json->Belt.List.get(0)->Belt.Option.getExn->snd,
      };
      Bridge_t.Res_Parse(
        run(
          parse_request |> Bridge_bs.write_parse_request |> Js.Json.stringify,
        )
        |> Js.Json.parseExn
        |> Bridge_bs.read_parse_result,
      )
      |> WebWorkers.postMessageFromWorker
    | Bridge_t.ExampleList =>
      Bridge_t.Res_ExampleList(
        example_list() |> Js.Json.parseExn |> Bridge_bs.read_example_list,
      )
      |> WebWorkers.postMessageFromWorker
    };
  },
);
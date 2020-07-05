@bs.val @bs.module("./../_build/default/src/js/re_typescript_js.bc.js")
external run: string => string = "run"
@bs.val @bs.module("./../_build/default/src/js/re_typescript_js.bc.js")
external example_list: unit => string = "example_list"

WebWorkers.setWorkerOnMessage(WebWorkers.self, (e: WebWorkers.MessageEvent.t) => {
  let data = WebWorkers.MessageEvent.data(e)
  switch data {
  | (requestId, Bridge_t.Parse(parse_request)) =>
    (
      requestId,
      Bridge_t.Res_Parse(
        run(parse_request |> Bridge_bs.write_parse_request |> Js.Json.stringify)
        |> Js.Json.parseExn
        |> Bridge_bs.read_parse_result,
      )
    ) |> WebWorkers.postMessageFromWorker
  | (requestId, Bridge_t.ExampleList) =>
    (
      requestId,
      Bridge_t.Res_ExampleList(example_list() |> Js.Json.parseExn |> Bridge_bs.read_example_list),
    ) |> WebWorkers.postMessageFromWorker
  }
})

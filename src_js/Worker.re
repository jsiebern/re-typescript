module Reason = {
  type a;
  [@bs.module "reason"] external printML: a => string = "printML";
  [@bs.module "reason"] external printRE: a => string = "printRE";
  [@bs.module "reason"] external parseML: string => a = "parseML";
};

[@bs.val]
[@bs.module "./../../../_build/default/src/js/re_typescript_js.bc.js"]
external run: string => string = "run";
let run = (value, re) =>
  try(
    Ok((re ? Reason.printRE : Reason.printML)(Reason.parseML(run(value))))
  ) {
  | e =>
    Js.log(e);
    Error(
      Js.Exn.asJsExn(e)
      ->Belt.Option.flatMap(Js.Exn.message)
      ->Belt.Option.getWithDefault("ERROR"),
    );
  };

WebWorkers.setWorkerOnMessage(
  WebWorkers.self,
  (e: WebWorkers.MessageEvent.t) => {
    let data = WebWorkers.MessageEvent.data(e);
    (
      switch (run(data##value, data##re)) {
      | Ok(data) => {"status": "success", "data": data}
      | Error(data) => {"status": "error", "data": data}
      }
    )
    |> WebWorkers.postMessageFromWorker;
  },
);

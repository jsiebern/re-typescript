module Reason = {
  type a;
  [@bs.module "reason"] external printML: a => string = "printML";
  [@bs.module "reason"] external printRE: a => string = "printRE";
  [@bs.module "reason"] external parseML: string => a = "parseML";
};

module Ansi = {
  type t = {ansi_to_html: (. string) => string};
  [@bs.new] [@bs.module "ansi_up"] external ansi_up: unit => t = "default";
  let instance = ansi_up();
};

[@bs.val]
[@bs.module "./../../../_build/default/src/js/re_typescript_js.bc.js"]
external run: string => string = "run";
let run = (value, re) =>
  try(
    Ok((re ? Reason.printRE : Reason.printML)(Reason.parseML(run(value))))
  ) {
  | e =>
    Error(
      Ansi.instance.ansi_to_html(.
        Js.Exn.asJsExn(e)
        ->Belt.Option.flatMap(Js.Exn.message)
        ->Belt.Option.getWithDefault("ERROR"),
      ),
    )
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

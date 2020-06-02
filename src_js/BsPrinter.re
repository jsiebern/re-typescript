let worker = WebWorkers.create_webworker("worker/worker.js");

let set_printed: ref(option(Bridge_bs.parse_result => unit)) = ref(None);
WebWorkers.onMessage(
  worker,
  (e: WebWorkers.MessageEvent.t) => {
    let data = WebWorkers.MessageEvent.data(e);

    switch (set_printed^) {
    | None => ()
    | Some(set_printed) => set_printed(data |> Bridge_bs.read_parse_result)
    };
  },
);

let usePrintedValue = (~re=true, value: string) => {
  let (printed, setPrinted) =
    React.useReducer((_, v) => v, Bridge_bs.Ok(""));
  set_printed := Some(setPrinted);

  React.useEffect2(
    () => {
      WebWorkers.postMessage(worker, {"value": value, "re": re});

      None;
    },
    (re, value),
  );

  printed;
};

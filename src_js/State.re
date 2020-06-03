let language = Recoil.atom({key: "language", default: Bridge_bs.Reason});

let ts_source = Recoil.atom({key: "ts_source", default: TsDefault.content});

let config = Recoil.atom({key: "config", default: Sync.config});

let worker = WebWorkers.create_webworker("worker/worker.js");
let parsing_complete: ref((. Bridge_bs.parse_result) => unit) =
  ref((. _) => ());
let working = ref(false);
WebWorkers.onMessage(
  worker,
  (e: WebWorkers.MessageEvent.t) => {
    working := false;
    parsing_complete^(. WebWorkers.MessageEvent.data(e));
  },
);
let parse_result: Recoil.readOnly(Bridge_bs.parse_result) =
  Recoil.asyncSelector({
    key: "parse_result",
    get: ({get}) => {
      let content = get(ts_source);
      let language = get(language);
      let config = get(config);
      Js.Promise.make((~resolve, ~reject) =>
        if (! working^) {
          parsing_complete := resolve;

          working := true;
          WebWorkers.postMessage(
            worker,
            Bridge_bs.{content, language, config},
          );
        } else {
          reject(. Not_found);
        }
      );
    },
  });
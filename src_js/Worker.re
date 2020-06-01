open Config_t;
open! Bridge_bs;

let default_bucklescript_config = {
  string_variant_mode: `BsInline,
  mixed_variant_mode: `BsUnboxed,
  number_variant_mode: `BsInline(Some({prefix: "_", suffix: ""})),
};
let default_bucklescript_bindings_config = {
  string_variant_mode: `Variant,
  mixed_variant_mode: `BsUnboxed,
  number_variant_mode: `Variant(Some({prefix: "_", suffix: ""})),
};

let default_config: config = {
  suppress_warning_for_extended_records: false,
  omit_extended_unreferenced_records: false,
  output_type: Bucklescript(default_bucklescript_config),
  array_mode: Array,
  number_mode: Float,
  intersection_mode: {
    objects: Tuple,
    unions: Tuple,
    classes: Tuple,
    functions: Tuple,
    other: Tuple,
    tuple_members_optional: false,
  },
  generate_parser: false,
  generate_serializer: false,
};

let config = {
  ...default_config,
  number_mode: Int,
  suppress_warning_for_extended_records: true,
  output_type:
    Bucklescript({
      ...default_bucklescript_config,
      string_variant_mode: `PolyVariant,
    }),
};

[@bs.val]
[@bs.module "./../../../_build/default/src/js/re_typescript_js.bc.js"]
external run: string => string = "run";

let run = (pr: parse_request) =>
  run(write_parse_request(pr) |> Js.Json.stringify)
  |> Js.Json.parseExn
  |> read_parse_result;

WebWorkers.setWorkerOnMessage(
  WebWorkers.self,
  (e: WebWorkers.MessageEvent.t) => {
    let data = WebWorkers.MessageEvent.data(e);
    (
      switch (
        run({
          language: data##re ? Reason : Ocaml,
          content: data##value,
          config,
        })
      ) {
      | Ok(data) => {"status": "success", "data": data}
      | Error((_, data)) => {"status": "error", "data": data}
      }
    )
    |> WebWorkers.postMessageFromWorker;
  },
);
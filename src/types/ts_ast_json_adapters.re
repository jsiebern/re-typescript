module Symbol: Atdgen_runtime.Json_adapter.S = {
  open Yojson.Safe;

  let normalize = (x: t): t =>
    switch (x) {
    | `Assoc(fields) =>
      List.mem_assoc("flags", fields)
        ? `List([`String("Symbol"), x]) : `List([`String("SymbolRef"), x])
    | `String(type_) as x => x
    | malformed => malformed
    };

  let restore = (x: t): t =>
    switch (x) {
    | `List([`String(type_), `Assoc(fields)]) =>
      let fields = [
        ("id", `String(type_)),
        ...List.filter(((k, v)) => k != "id", fields),
      ];

      `Assoc(fields);
    | `String(type_) as x => x
    | malformed => malformed
    };
};

module KindName =
  Atdgen_runtime.Json_adapter.Type_field.Make({
    let type_field_name = "kindName";
  });
module IntrinsicName =
  Atdgen_runtime.Json_adapter.Type_field.Make({
    let type_field_name = "intrinsicName";
  });

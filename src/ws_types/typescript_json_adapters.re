module RemoveUndefined: Atdgen_runtime.Json_adapter.S = {
  let normalize = json => json;
  let restore = json => json;
};

module KindName = {
  open Yojson.Safe;

  let type_field_name = "kindName";
  let normalize = (x: t): t =>
    switch (x) {
    | `Assoc(fields) =>
      switch (List.assoc(type_field_name, fields)) {
      | `String(type_) => `List([`String(type_), x])
      | exception Not_found => x
      | _ => x /* malformed */
      }
    | `String(type_) as x => x
    | malformed => malformed
    };

  let restore = (x: t): t =>
    switch (x) {
    | `List([`String(type_), `Assoc(fields)]) =>
      let fields = [
        (type_field_name, `String(type_)),
        ...List.filter(((k, v)) => k != type_field_name, fields),
      ];

      `Assoc(fields);
    | `String(type_) as x => x
    | malformed => malformed
    };
};

module RemoveUndefined: Atdgen_codec_runtime.Json_adapter.S = {
  let normalize = json =>
    switch json |> Js.Json.classify {
    | JSONObject(obj) =>
      Js.Dict.keys(obj)->Belt.Array.forEach(key =>
        if Js.typeof(Js.Dict.unsafeGet(obj, key)) === "undefined" {
          Js.Dict.unsafeDeleteKey(. obj->Obj.magic, key)
        }
      )
      Json.Encode.jsonDict(obj)
    | _ => json
    }
  let restore = j => j
}

module KindName = {
  module Json = Js.Json
  let type_field_name = "kindName"
  let normalize = (json: Json.t) => {
    switch json |> Js.Json.classify {
    | JSONObject(obj) =>
      switch Js.Dict.get(obj, type_field_name) {
      | Some(type_) =>
        let normalized: Json.t = (Obj.magic((type_, RemoveUndefined.normalize(json))): Json.t)
        normalized
      | None =>
      switch (Js.Dict.get(obj, "kind")->Belt.Option.flatMap(Js.Json.decodeNumber)) {
        | None =>
          Js.Dict.set(obj, "kindName", "Count"->Js.Json.string);
          (Obj.magic(("Count", RemoveUndefined.normalize(json))): Json.t)
        | Some(kindId) =>
          let kindName = Typescript_syntaxkind.kindCache->Belt.Map.Int.getExn(kindId->int_of_float);
          Js.Dict.set(obj, "kindName", kindName->Js.Json.string);
          (Obj.magic((kindName, RemoveUndefined.normalize(json))): Json.t)
      }
      }
    | _ => RemoveUndefined.normalize(json)
    }
  }

  let restore = json =>
    switch json |> Js.Json.classify {
    | JSONArray([v, o]) when Js.typeof(v) == "string" =>
      switch o |> Js.Json.classify {
      | JSONObject(obj) =>
        Js.Dict.set(obj, type_field_name, v)
        Json_encode.jsonDict(obj)
      | _ => json
      }
    | _ => json
    }
}

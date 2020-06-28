[@bs.new]
external _unsafeCreateUninitializedArray: int => array('a) = "Array";
let _stringify = j => {
  DebugHelper.debugClean(j, 1);
};

open Printf;

module Json = {
  type t = Js.Json.t;
  external read_t: t => t = "%identity";
  external write_t: t => t = "%identity";
};

module Json_adapter = {
  module type S = {
    let normalize: Json.t => Json.t;
    let restore: Json.t => Json.t;
  };

  module Type_field = {
    module type Param = {let type_field_name: string;};

    module Make = (Param: Param) : S => {
      open Param;

      let normalize = (json: Json.t) =>
        switch (json |> Js.Json.classify) {
        | JSONObject(obj) =>
          switch (Js.Dict.get(obj, type_field_name)) {
          | Some(type_) =>
            let normalized: Json.t = (Obj.magic((type_, json)): Json.t);
            normalized;
          | None => json
          }
        | _ => json
        };

      let restore = json =>
        switch (json |> Js.Json.classify) {
        | JSONArray([|v, o|]) when Js.typeof(v) == "string" =>
          switch (o |> Js.Json.classify) {
          | JSONObject(obj) =>
            Js.Dict.set(obj, type_field_name, v);
            Json_encode.jsonDict(obj);
          | _ => json
          }
        | _ => json
        };
    };

    module Default_param: Param = {
      let type_field_name = "type";
    };

    include Make(Default_param);
  };

  module Kind_field =
    Type_field.Make({
      let type_field_name = "kind";
    });
};

module Encode = {
  include Json_encode;

  type t('a) = encoder('a);

  let make = f => f;

  let encode = (f, x) => f(x);

  let unit = () => null;

  let int32 = s => string(Int32.to_string(s));
  let int64 = s => string(Int64.to_string(s));

  type spec('a, 'b) = {
    name: string,
    data: 'a,
    encode: t('b),
  };

  type field_spec('a) =
    | Optional(spec(option('a), 'a), option('a))
    | Required(spec('a, 'a), option('a));

  type field =
    | F(field_spec('a)): field;

  let list = (encode, l) => l |> Array.of_list |> array(encode);

  let field = (~default=?, encode, ~name, data) =>
    F([@implicit_arity] Required({name, data, encode}, default));

  let field_o = (~default=?, encode, ~name, data) =>
    F([@implicit_arity] Optional({name, data, encode}, default));

  let obj = fields =>
    List.fold_left(
      (acc, F(f)) =>
        switch (f) {
        | [@implicit_arity] Required({name, data, encode}, None) => [
            (name, encode(data)),
            ...acc,
          ]
        | [@implicit_arity] Required({name, data, encode}, Some(default)) =>
          if (default == data) {
            acc;
          } else {
            [(name, encode(data)), ...acc];
          }
        | [@implicit_arity] Optional({name, data, encode}, default) =>
          switch (data, default) {
          | (None, _) => acc
          | (Some(s), Some(default)) =>
            if (s == default) {
              acc;
            } else {
              [(name, encode(s)), ...acc];
            }
          | (Some(s), None) => [(name, encode(s)), ...acc]
          }
        },
      [],
      fields,
    )
    |> object_;

  let tuple1 = (f, x) => jsonArray([|f(x)|]);

  let contramap = (f, g, b) => g(f(b));

  let constr0 = string;
  let constr1 = (s, f, x) => pair(string, f, (s, x));

  let option_as_constr = f =>
    fun
    | None => string("None")
    | Some(s) => pair(string, f, ("Some", s));

  let adapter = (restore: Json.t => Json.t, writer: t('a), x) => {
    let encoded = writer(x);
    restore(encoded);
  };
};

module Decode = {
  include Json_decode;

  exception DecodeErrorPath(list(string), string);

  type t('a) = decoder('a);

  let make = f => f;

  let decode' = (f, json) => f(json);

  let decode = (f, json) =>
    try(f(json)) {
    | [@implicit_arity] DecodeErrorPath(path, msg) =>
      let path = String.concat(".", path);
      raise(DecodeError({j|$path: $msg|j}));
    };

  let with_segment = (segment, f, json) =>
    try(f(json)) {
    | DecodeError(msg) =>
      Js.Console.trace();
      Js.log(msg);
      raise([@implicit_arity] DecodeErrorPath([segment], msg));
    | [@implicit_arity] DecodeErrorPath(path, msg) =>
      raise([@implicit_arity] DecodeErrorPath([segment, ...path], msg))
    };

  let unit = j =>
    if ((Obj.magic(j): Js.null('a)) === Js.null) {
      ();
    } else {
      raise(
        DecodeError(sprintf("Expected null, got %s", Js.Json.stringify(j))),
      );
    };

  let int32 = j => Int32.of_string(string(j));
  let int64 = j => Int64.of_string(string(j));

  let array = (decode, json) =>
    if (Js.Array.isArray(json)) {
      let source: array(Js.Json.t) = Obj.magic(json: Js.Json.t);
      let length = Js.Array.length(source);
      let target = _unsafeCreateUninitializedArray(length);
      for (i in 0 to length - 1) {
        let value =
          try(
            with_segment(
              string_of_int(i),
              decode,
              Array.unsafe_get(source, i),
            )
          ) {
          | DecodeError(msg) =>
            raise @@
            DecodeError(msg ++ "\n\tin array at index " ++ string_of_int(i))
          };

        Array.unsafe_set(target, i, value);
      };
      target;
    } else {
      raise @@ DecodeError("Expected array, got " ++ _stringify(json));
    };

  let list = (decode, json) => json |> array(decode) |> Array.to_list;

  let pair = (decodeA, decodeB, json) =>
    if (Js.Array.isArray(json)) {
      let source: array(Js.Json.t) = Obj.magic(json: Js.Json.t);
      let length = Js.Array.length(source);
      if (length == 2) {
        try((
          with_segment("0", decodeA, Array.unsafe_get(source, 0)),
          with_segment("1", decodeB, Array.unsafe_get(source, 1)),
        )) {
        | DecodeError(msg) =>
          raise @@ DecodeError(msg ++ "\n\tin pair/tuple2")
        };
      } else {
        raise @@
        DecodeError(
          {j|Expected array of length 2, got array of length $length|j},
        );
      };
    } else {
      raise @@ DecodeError("Expected array, got " ++ _stringify(json));
    };

  let tuple2 = pair;

  let tuple3 = (decodeA, decodeB, decodeC, json) =>
    if (Js.Array.isArray(json)) {
      let source: array(Js.Json.t) = Obj.magic(json: Js.Json.t);
      let length = Js.Array.length(source);
      if (length == 3) {
        try((
          with_segment("0", decodeA, Array.unsafe_get(source, 0)),
          with_segment("1", decodeB, Array.unsafe_get(source, 1)),
          with_segment("2", decodeC, Array.unsafe_get(source, 2)),
        )) {
        | DecodeError(msg) => raise @@ DecodeError(msg ++ "\n\tin tuple3")
        };
      } else {
        raise @@
        DecodeError(
          {j|Expected array of length 3, got array of length $length|j},
        );
      };
    } else {
      raise @@ DecodeError("Expected array, got " ++ _stringify(json));
    };

  let tuple4 = (decodeA, decodeB, decodeC, decodeD, json) =>
    if (Js.Array.isArray(json)) {
      let source: array(Js.Json.t) = Obj.magic(json: Js.Json.t);
      let length = Js.Array.length(source);
      if (length == 4) {
        try((
          with_segment("1", decodeA, Array.unsafe_get(source, 0)),
          with_segment("2", decodeB, Array.unsafe_get(source, 1)),
          with_segment("3", decodeC, Array.unsafe_get(source, 2)),
          with_segment("4", decodeD, Array.unsafe_get(source, 3)),
        )) {
        | DecodeError(msg) => raise @@ DecodeError(msg ++ "\n\tin tuple4")
        };
      } else {
        raise @@
        DecodeError(
          {j|Expected array of length 4, got array of length $length|j},
        );
      };
    } else {
      raise @@ DecodeError("Expected array, got " ++ _stringify(json));
    };

  let dict = (decode, json) =>
    if (Js.typeof(json) == "object"
        && !Js.Array.isArray(json)
        && !((Obj.magic(json): Js.null('a)) === Js.null)) {
      let source: Js.Dict.t(Js.Json.t) = Obj.magic(json: Js.Json.t);
      let keys = Js.Dict.keys(source);
      let l = Js.Array.length(keys);
      let target = Js.Dict.empty();
      for (i in 0 to l - 1) {
        let key = Array.unsafe_get(keys, i);
        let value =
          try(with_segment(key, decode, Js.Dict.unsafeGet(source, key))) {
          | DecodeError(msg) => raise @@ DecodeError(msg ++ "\n\tin dict")
          };

        Js.Dict.set(target, key, value);
      };
      target;
    } else {
      raise @@ DecodeError("Expected object, got " ++ _stringify(json));
    };

  let field = (key, decode, json) =>
    if (Js.typeof(json) == "object"
        && !Js.Array.isArray(json)
        && !((Obj.magic(json): Js.null('a)) === Js.null)) {
      let dict: Js.Dict.t(Js.Json.t) = Obj.magic(json: Js.Json.t);
      switch (Js.Dict.get(dict, key)) {
      | Some(value) =>
        try(with_segment(key, decode, value)) {
        | DecodeError(msg) =>
          raise @@ DecodeError(msg ++ "\n\tat field '" ++ key ++ "'")
        }
      | None => raise @@ DecodeError({j|Expected field '$(key)'|j})
      };
    } else {
      raise @@ DecodeError("Expected object, got " ++ _stringify(json));
    };

  let obj_array = (f, json) => dict(f, json) |> Js.Dict.entries;

  let obj_list = (f, json) => obj_array(f, json) |> Array.to_list;

  let nullable = (decode, json) =>
    if ((Obj.magic(json): Js.null('a)) === Js.null) {
      None;
    } else {
      Some(decode(json));
    };

  /* Unlike Json_decode.field, this returns None if key is not found */
  let fieldOptional = (key, decode, json) =>
    if (Js.typeof(json) == "object"
        && !Js.Array.isArray(json)
        && !((Obj.magic(json): Js.null('a)) === Js.null)) {
      let dict: Js.Dict.t(Js.Json.t) = Obj.magic(json: Js.Json.t);
      switch (Js.Dict.get(dict, key)) {
      | None => None
      | Some(value) =>
        try(Some(with_segment(key, decode, value))) {
        | DecodeError(msg) =>
          raise @@ DecodeError(msg ++ "\n\tat field '" ++ key ++ "'")
        }
      };
    } else {
      raise @@
      DecodeError("Expected object, got " ++ Js.Json.stringify(json));
    };

  let fieldDefault = (s, default, f) =>
    fieldOptional(s, f)
    |> map(
         fun
         | None => default
         | Some(s) => s,
       );

  let tuple1 = (f, x) =>
    if (Js.Array.isArray(x)) {
      let source: array(Js.Json.t) = Obj.magic(x: Js.Json.t);
      let length = Js.Array.length(source);
      if (length == 1) {
        try(with_segment("0", f, Array.unsafe_get(source, 0))) {
        | DecodeError(msg) => raise @@ DecodeError(msg ++ "\n\tin tuple1")
        };
      } else {
        raise @@
        DecodeError(
          {j|Expected array of length 1, got array of length $length|j},
        );
      };
    } else {
      raise @@ DecodeError("Expected array, got " ++ Js.Json.stringify(x));
    };

  let enum = (l, json) => {
    let constr0 = j => {
      let s = string(j);
      `Constr0(s);
    };
    let constr = j => {
      let p = pair(string, x => x, j);
      `Constr(p);
    };
    switch (either(constr0, constr, json)) {
    | `Constr0(s) =>
      with_segment(
        s,
        () =>
          switch (List.assoc(s, l)) {
          | exception Not_found =>
            raise @@ DecodeError(sprintf("unknown constructor %S", s))
          | `Single(a) => a
          | `Decode(_) =>
            raise @@
            DecodeError(sprintf("constructor %S expects arguments", s))
          },
        (),
      )
    | `Constr(s, args) =>
      with_segment(
        s,
        () =>
          switch (List.assoc(s, l)) {
          | exception Not_found =>
            raise @@ DecodeError(sprintf("unknown constructor %S", s))
          | `Single(_) =>
            raise @@
            DecodeError(
              sprintf("constructor %S doesn't expect arguments", s),
            )
          | `Decode(d) => decode'(d, args)
          },
        (),
      )
    };
  };

  let option_as_constr = f =>
    either(
      x =>
        if (string(x) == "None") {
          None;
        } else {
          raise(
            DecodeError(
              sprintf("Expected None, got %s", Js.Json.stringify(x)),
            ),
          );
        },
      x =>
        switch (pair(string, f, x)) {
        | ("Some", v) => Some(v)
        | _ =>
          raise(
            DecodeError(
              sprintf("Expected Some _, got %s", Js.Json.stringify(x)),
            ),
          )
        },
    );

  let adapter = (normalize: Json.t => Json.t, reader: t('a), json) =>
    reader(normalize(json));
};

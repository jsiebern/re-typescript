module type T = {type value;};
module type TMap = {
  type value;
  type key = pri string;
  type t;

  let make: (~size: int=?, unit) => t;
  let get: (key, t) => option(value);
  let has: (key, t) => bool;
  let for_each: ((key, value) => unit, t) => unit;
  let size: t => int;
  let keys: t => Iter.t(key);
  let values: t => Iter.t(value);
  let entries: t => Iter.t((key, value));
  let add: (key, value, t) => unit;
  let set: (key, value, t) => unit;
  let delete: (key, t) => bool;
  let clear: t => unit;
};

module Make: (T: T) => TMap =
  (T: T) => {
    type key = string;
    type value = T.value;
    type t = Hashtbl.t(key, value);
    let eq_key = CCString.equal;

    let make = (~size=3, ()) => Hashtbl.create(size);
    let get = (key, map) =>
      Iter.of_hashtbl(map)
      |> Iter.find(((k, value)) => eq_key(key, k) ? Some(value) : None);
    let has = (key, map) =>
      Iter.hashtbl_keys(map) |> Iter.exists(eq_key(key));
    let for_each = (fn, map) =>
      Iter.of_hashtbl(map, ((key, value)) => fn(key, value));
    let size = map => Hashtbl.length(map);
    let keys = map => Iter.hashtbl_keys(map);
    let values = map => Iter.hashtbl_values(map);
    let entries = map => Iter.of_hashtbl(map);
    let add = (key, value, map) => Hashtbl.add(map, key, value);
    let set = (key, value, map) => Hashtbl.replace(map, key, value);
    let delete = (key, map) =>
      has(key, map)
        ? {
          Hashtbl.remove(map, key);
          true;
        }
        : false;
    let clear = map => Hashtbl.clear(map);
  };

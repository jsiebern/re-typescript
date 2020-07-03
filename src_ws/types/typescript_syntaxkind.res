@bs.val @bs.module("ts-morph") @bs.scope("ts")
external syntaxKind: Js.Dict.t<'string> = "SyntaxKind";
let kindCacheLst: (array<int>, array<string>) =
  syntaxKind
  ->Js.Dict.keys
  ->Belt.Array.keepU((. key) => Belt.Int.fromString(key)->Belt.Option.isSome)
  ->Belt.Array.keepMapU((. key) =>
      syntaxKind
      ->Js.Dict.get(key)
      ->Belt.Option.map(value => (key->int_of_string, value))
    )
  ->Belt.Array.unzip;

let kindCache =
  Belt.Array.zip(fst(kindCacheLst), snd(kindCacheLst))
  ->Belt.Map.Int.fromArray;

let kindCacheRev =
  Belt.Array.zip(snd(kindCacheLst), fst(kindCacheLst))
  ->Belt.Map.String.fromArray;

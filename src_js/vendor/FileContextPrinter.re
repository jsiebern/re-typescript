type initialConfig = {
  linesBefore: int,
  linesAfter: int,
};

module Config = {
  type t = {
    linesBefore: int,
    linesAfter: int,
  };

  let initialize: initialConfig => t =
    config => {
      linesBefore: config.linesBefore,
      linesAfter: config.linesAfter,
    };
};

module type FileContextPrinterConfig = {let config: Config.t;};
module Make = (UserConfig: FileContextPrinterConfig) => {
  let print = (~highlight, x) => {
    let l = (x |> CCListLabels.length |> string_of_int |> CCString.length) + 1;
    x
    ->Belt.List.mapWithIndex((i, ln) =>
        " "
        ++ i->string_of_int
        ++ CCString.repeat(
             ~count=l - (i |> string_of_int |> CCString.length),
             " ",
           )
        ++ "|  "
        ++ (
          if (i + 1 === fst(fst(highlight))) {
            ln
            ++ "\n"
            ++ CCString.repeat(~count=snd(fst(highlight)) - 1 + l + 4, " ")
            ++ CCString.repeat(
                 ~count=snd(snd(highlight)) - snd(fst(highlight)),
                 {j|↑|j},
               );
          } else {
            ln;
          }
        )
      )
    ->Belt.List.keepWithIndex((_, i) =>
        i
        + 1 >= fst(fst(highlight))
        - 2
        && i
        + 1 <= fst(snd(highlight))
        + 2
      )
    ->Belt.List.toArray
    ->Js.Array.joinWith("\n", _);
  };
};

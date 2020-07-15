module Config: {
  type t;
  [@js.builder]
  let make:
    (
      ~sourceFile: Ts_raw.Node.t=?,
      ~typeChecker: Ts_raw.TypeChecker.t=?,
      unit
    ) =>
    t;
};

let createWrappedNode: (Ts_raw.Node.t, Config.t) => Ts_morph.Node.t;
let getSyntaxKindName: int => string;

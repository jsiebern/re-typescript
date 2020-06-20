include Fp;

// let append_extension = (path: Fp.t('kind), ext) => switch (path |> Fp.firstClass) {
//     | Relative(path) => (Fp.toDebugString(path) ++ ext) |> Fp.relativeExn
//     | Absolute(path) => (Fp.toString(path) ++ ext) |> Fp.absoluteExn
// }

let append_extension = (path: Fp.t(Fp.absolute), ext) =>
  Fp.toString(path) ++ ext |> Fp.absoluteExn;

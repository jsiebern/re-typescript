type path = Fp.t(Fp.absolute);

type config = {
  tsconfig: option(path),
  loader: (module Loader.T),
};
module type Config = {let config: config;};

module type T = {
  let config: config;
  let resolve:
    (~current_file: path, string) => CCResult.t((string, path), string);
};

module Make = (C: Config) : T => {
  let config = C.config;
  let resolve =
      (~current_file: path, module_specifier: string)
      : CCResult.t((string, path), string) => {
    module Loader = (val config.loader: Loader.T);

    // Do this rudimentary for now, assume that `module_specifier` is a relative path
    switch (Fp.relative(module_specifier ++ ".d.ts")) {
    | None =>
      Error(
        Printf.sprintf("Invalid relative import path: %s", module_specifier),
      )
    | Some(module_path) =>
      let file_path = Fp.join(current_file |> Fp.dirName, module_path);
      Loader.file_read(file_path) |> CCResult.map(res => (res, file_path));
    };
  };
};

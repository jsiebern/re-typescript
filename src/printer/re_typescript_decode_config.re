open Re_typescript_base;

type output_type =
  | BucklescriptBindings
  | Bucklescript
  | Native
and array_mode =
  | List
  | Array
and number_mode =
  | Int
  | Float
  | Unboxed
and config = {
  output_type,
  array_mode,
  number_mode,
  files: list((string, Ts.toplevel)),
  file_loader: (module Re_typescript_file_loader.T),
};
let defaultConfig: config = {
  output_type: Bucklescript,
  array_mode: Array,
  number_mode: Float,
  files: [],
  file_loader: (module Re_typescript_file_loader.Loader_fs),
};

// module X = (val defaultConfig.file_loader: Re_typescript_file_loader.T);

module Ts = Typescript_t;

type connector = {
  id: int,
  parent: int,
}
and type_declaration =
  | TypeAlias(type_alias, type_definition)
and type_alias = {
  id: int,
  name: identifier,
  type_node: option(Ts.node),
}
and identifier = {
  id: int,
  text: string,
}
and type_definition =
  | String
  | Number
and source_file = {
  fileName: string,
  statements: list(type_declaration),
};

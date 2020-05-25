type with_pi('t) = {
  pi: Parse_info.t,
  item: 't,
};

type declaration =
  | Namespace(with_pi(declaration_namespace))
and declaration_namespace = {
  n_ident: list(identifier_name),
  n_declarations: list(declaration_namespace_item),
}
and declaration_namespace_item = {
  ni_item: declaration,
  ni_exported: bool,
}
and identifier_name = with_pi(string);

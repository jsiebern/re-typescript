type with_pi('t) = {
  pi: Parse_info.t,
  item: 't,
};

type declaration =
  | Namespace(with_pi(declaration_namespace))
  | Type(with_pi(declaration_type))
and declaration_type = {
  t_ident: list(identifier_name),
  t_parameters: type_parameters,
  t_type: type_,
}
and type_parameters = list(type_parameter)
and type_parameter = {
  tp_ident: identifier_name,
  tp_extends: option(type_),
  tp_default: option(type_),
}
and type_ =
  | Object(option(type_member_list))
  | Tuple(list(type_))
  | Array(type_)
  | Function(definition_function)
  | Constructor(definition_function)
  | Intersection(type_, type_)
  | Union(type_, option(type_))
  | Query(list(identifier_name))
  | StringLiteral(identifier_name)
  | NumberLiteral(with_pi(int))
  | TypeReference(list(identifier_name), option(type_arguments))
  | String(Parse_info.t)
  | Number(Parse_info.t)
  | Boolean(Parse_info.t)
  | Null(Parse_info.t)
  | Undefined(Parse_info.t)
  | Void(Parse_info.t)
  | Any(Parse_info.t)
  | Symbol(Parse_info.t)
  | This(Parse_info.t)
and type_arguments = list(type_)
and definition_function = {
  f_parameters: option(type_parameters),
  f_body: list(parameter),
  f_ret: type_,
}
and type_member_list = list(type_member)
and type_member =
  | PropertySignature(property_signature)
  | ConstructSignature(call_signature)
  | IndexSignature(identifier_name, type_)
  | MethodSignature(method_signature)
and method_signature = {
  ms_property_name: property_name,
  ms_call_signature: call_signature,
  ms_optional: bool,
}
and property_signature = {
  ps_property_name: property_name,
  ps_type_annotation: type_,
  ps_optional: bool,
}
and call_signature = {
  cs_type_parameters: option(type_parameters),
  cs_parameter_list: parameter_list,
  cs_type_annotation: option(type_),
}
and property_name =
  | PIdentifier(identifier_name)
  | PString(with_pi(string))
  | PNumber(with_pi(int))
and parameter_list = list(parameter_type)
and parameter_type =
  | RestParameter(identifier_name, option(type_))
  | Parameter(parameter)
and parameter = {
  p_optional: bool,
  p_ibinding: identifier_or_binding,
  p_type_annotation: option(type_),
  p_accessibility: accessibility_modifier,
}
and declaration_namespace = {
  n_ident: list(identifier_name),
  n_declarations: list(declaration_namespace_item),
}
and declaration_namespace_item = {
  ni_item: declaration,
  ni_exported: bool,
}
and identifier_or_binding =
  | IdentifierBinding(identifier_name)
  | ObjectBinding(list(identifier_name))
  | ArrayBinding(list(identifier_name), option(identifier_name))
and accessibility_modifier =
  | Public
  | Protected
  | Private
and identifier_name = with_pi(string);

type with_pi('t) = {
  pi: Parse_info.t,
  item: 't,
};

type declaration =
  | IdentifierReference(identifier_name)
  | ExportList(with_pi(export_list))
  | Export(declaration)
  | ExportAssignment(identifier_name)
  | ExportDefault(declaration)
  | Ambient(declaration)
  | Namespace(with_pi(declaration_namespace))
  | Module(with_pi((string, list(declaration))))
  | Type(with_pi(declaration_type))
  | Interface(with_pi(declaration_interface))
  | Enum(with_pi(declaration_enum))
  | FunctionDec(with_pi(declaration_function))
  | Class(with_pi(declaration_class))
  | Variable(with_pi(list((identifier_name, option(type_)))))
  | ImportAlias(with_pi((identifier_name, list(identifier_name))))
  | Import(with_pi(declaration_import))
and declaration_import = {
  i_clause: declaration_import_clause,
  i_from: string,
}
and declaration_import_clause =
  | TripleSlashReference
  | ImportModuleSpecifier
  | ImportBinding(identifier_name)
  | ImportNamespace(identifier_name)
  | ImportNamed(list(declaration_import_named))
  | ImportSplitNamespace(identifier_name, identifier_name)
  | ImportSplitNamed(identifier_name, list(declaration_import_named))
and declaration_import_named =
  | ImportNamedIdent(identifier_name)
  | ImportNamedAs(identifier_name, identifier_name)
and export_list = {
  el_items: list(export_list_item),
  el_from: option(string),
}
and export_list_item =
  | ExportStar
  | ExportIdent(identifier_name)
  | ExportAs(identifier_name, identifier_name)
and declaration_class = {
  c_ident: option(identifier_name),
  c_parameters: option(type_parameters),
  c_extends: option(type_reference),
  c_implements: option(class_implements_clause),
  c_elements: list(class_body_element),
}
and class_body_element =
  | ClassConstructor(parameter_list)
  | ClassIndexSignature(index_signature)
  | ClassPropertyMember(class_property_member)
and class_property_member = {
  cp_accessibility: option(accessibility_modifier),
  cp_static: bool,
  cp_property_name: property_name,
  cp_type_annotation: option(type_),
  cp_call_signature: option(call_signature),
}
and class_implements_clause = list(type_reference)
and declaration_function = {
  f_ident: identifier_name,
  f_call_signature: call_signature,
}
and declaration_enum = {
  e_ident: identifier_name,
  e_const: bool,
  e_members: list(enum_member),
}
and enum_member = {
  em_property_name: property_name,
  em_value: option(enum_member_value),
}
and enum_member_value =
  | EMVString(with_pi(string))
  | EMVNumber(with_pi(int))
and declaration_interface = {
  i_ident: identifier_name,
  i_parameters: option(type_parameters),
  i_extends: option(interface_extends_clause),
  i_members: option(type_member_list),
}
and interface_extends_clause = list(type_reference)
and declaration_type = {
  t_ident: identifier_name,
  t_parameters: option(type_parameters),
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
  | Query(with_pi(type_reference))
  | KeyOf(with_pi(type_))
  | StringLiteral(with_pi(string))
  | NumberLiteral(with_pi(int))
  | BoolLiteral(with_pi(bool))
  | TypeReference(type_reference)
  | TypeExtract(type_reference, list(list(string)))
  | String(Parse_info.t)
  | Number(Parse_info.t)
  | Boolean(Parse_info.t)
  | Null(Parse_info.t)
  | Undefined(Parse_info.t)
  | Void(Parse_info.t)
  | Any(Parse_info.t)
  | Symbol(Parse_info.t)
  | This(Parse_info.t)
  | UnionTemp(list(temp_union_member))
  | MappedObject(with_pi(mapped_object))
and mapped_object = {
  mo_readonly: bool,
  mo_ident: identifier_name,
  mo_type: type_,
  mo_optional: bool,
  mo_type_annotation: option(type_),
}
and temp_union_member = [
  | `U_String(string)
  | `U_Number(int)
  | `U_Type(type_)
  | `U_Bool(bool)
]
and type_reference = (list(identifier_name), option(type_arguments))
and type_arguments = list(type_)
and definition_function = {
  f_parameters: option(type_parameters),
  f_body: parameter_list,
  f_ret: type_,
}
and type_member_list = list(type_member)
and type_member =
  | PropertySignature(property_signature)
  | CallSignature(call_signature)
  | ConstructSignature(call_signature)
  | IndexSignature(index_signature)
  | MethodSignature(method_signature)
and index_signature = {
  is_ident: identifier_name,
  is_kind: index_signature_kind,
  is_type_annotation: type_,
}
and index_signature_kind =
  | ISString
  | ISNumber
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
  p_accessibility: option(accessibility_modifier),
}
and declaration_namespace = {
  n_ident: list(identifier_name),
  n_declarations: list(declaration),
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

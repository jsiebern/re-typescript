%%

%public let type_reference :=
  | tn = identifier_path; ta = type_arguments?; { `TypeReference(tn, ta) }

%public let identifier_name :=
  | i = IDENT; { let (i, pi) = i in { Ts.pi; item = i } }

%public let identifier_path :=
  | l = separated_nonempty_list(DOT, identifier_name); { l }

%public let identifier_or_binding :=
  | bp = binding_pattern; { bp }

%public let binding_identifier :=
  | i = identifier_name; { `IdentifierBinding(i) }

%public let binding_pattern :=
  | bp = object_binding_pattern; { `ObjectBinding(bp) }
  | bp = array_binding_pattern; { `ArrayBinding(bp) }

%public let object_binding_pattern :=
  | l = delimited(LCURLY, separated_or_terminated_list(COMMA, binding_property), RCURLY); { l }

%public let array_binding_pattern :=
  | l = delimited(LBRACKET, rest_element, RBRACKET); { [l] }
  | LBRACKET; l = separated_or_terminated_list(COMMA, binding_property); rp = rest_element?; RBRACKET; { l @ [rp] }

%public let rest_element :=
  | ELLIPSIS; i = identifier_name; { `Rest(i) }

%public let binding_property :=
  | i = identifier_name; { `Identifier(i) }
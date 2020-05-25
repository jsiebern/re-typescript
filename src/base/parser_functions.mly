%%

%public let declaration_function :=
  | FUNCTION; i = identifier_name; cs = call_signature;  { `Function(i, cs) }

%public let call_signature :=
  | tp = type_parameters?; pa = delimited(LPAREN, parameter_list, RPAREN); ta = type_annotation?; { `CallSignature(tp, pa, ta) }

%public let parameter_list :=
  | l = separated_nonempty_list(COMMA, parameter); rp = rest_parameter?; { l @ [rp] }

%public let parameter :=
  | rp = parameter_required; { Some(`X(rp)) }
  | op = parameter_optional; { Some(`Y(op)) }

%public let rest_parameter :=
  | re = rest_element; ta = type_annotation?; { `RestParameter(re, ta) }

%public let parameter_required :=
  | acm = accessibility_modifier?; ib = identifier_or_binding; ta = type_annotation?; { `Parameter(ib, ta, false, acm) }
  | bi = binding_identifier; sl = literal_string; { `Parameter(bi, Some(`Unionx([`StringLiteral(sl)], [])), false, None) }

%public let accessibility_modifier ==
  | PUBLIC; { `Public }
  | PROTECTED; { `Protected }
  | PRIVATE; { `Private }

%public let parameter_optional :=
  | acm = accessibility_modifier?; ib = identifier_or_binding; QMARK; ta = type_annotation?; { `Parameter(ib, ta, true, acm) }
  | bi = binding_identifier; QMARK; sl = literal_string; { `Parameter(bi, Some(`Unionx([`StringLiteral(sl)], [])), false, None) }
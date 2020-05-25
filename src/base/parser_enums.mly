%%

%public let declaration_enum :=
  | c = boption(CONST); ENUM; i = identifier_name; body = delimited(LPAREN, enum_body, RPAREN); { `Enum(i, body) }

%public let enum_body :=
  | b = separated_or_terminated_list(COMMA, enum_member)?; { b }

%public let enum_member :=
  | n = property_name; { `EnumMember(n, None) }
  | n = property_name; EQUALS; v = enum_value; { `EnumMember(n, Some(v)) }

%public let enum_value :=
  | v = literal_string; { v }
  | v = literal_number; { v }
%%

%public let declaration_type :=
  | TYPE; i = identifier_name; ta = type_arguments?; EQUALS; te = type_; { `Type(i, ta, te) }

%public let type_ :=
  | t = type_union_or_intersection_or_primary; { t }
  | t = type_function; { t }
  | t = type_constructor; { t }

%public let type_union_or_intersection_or_primary :=
  | t = type_union; { t }
  | t = type_intersection_or_primary; { t }

%public let type_intersection_or_primary :=
  | t = type_intersection; { t }
  | t = type_primary; { t }

%public let type_primary :=
  // | t = type_parenthesized; { t }
  | t = type_predefined; { t }
  | t = type_reference; { t }
  | t = type_object; { t }
  | t = type_array; { t }
  | t = type_tuple; { t }
  | t = type_query; { t }
  | t = type_this; { t }

// %public let type_parenthesized :=
//   | t = delimited(LPAREN, type_, RPAREN); { t }

(*
  Function
*)
%public let type_function :=
  | tp = type_parameters?; pl = delimited(LPAREN, loption(parameter_list), RPAREN); ARROW; ret = type_; { `Function(tp, pl, ret) }
%public let type_constructor :=
  | NEW; f = type_function; { `Constructor(f) }

(*
  Intersection
*)
%public let type_intersection :=
  | x = type_intersection_or_primary; AMPERSAND; y = type_primary; { `Intersection(x,y) }

(*
  Union
*)
%public let type_union :=
  | x = type_union_or_intersection_or_primary; PIPE; y = type_intersection_or_primary; { `Union(x,y) }

(*
    Predefined types
*)
%public let type_predefined :=
  | PRIM_STRING;       { `String }
  | PRIM_NUMBER;       { `Number }
  | PRIM_BOOLEAN;      { `Boolean }
  | PRIM_NULL;         { `Null }
  | PRIM_UNDEFINED;    { `Undefined }
  | PRIM_VOID;         { `Void }
  | PRIM_ANY;          { `Any }
  | SYMBOL;            { `Symbol }

(*
    Type query
*)
%public let type_query :=
  | TYPEOF; ip = identifier_path; { `Query(ip) }

(*
    This
*)
%public let type_this :=
  | THIS; { `This }

(*
    Array
*)
%public let type_array :=
  | t = type_primary; LBRACKET; RBRACKET; { `Array(t) }
  | ARRAY; t = delimited(LT, type_, GT); { `Array(t) }

(*
    Tuple
*)
%public let type_tuple :=
  | l = delimited(LBRACKET, separated_or_terminated_list(COMMA, type_), RBRACKET); { `Tuple(l) }

(*
    Object
*)
%public let type_object :=
  | t = delimited(LCURLY, type_member_list?, RCURLY); { `Object(t) }

%public let type_member_list :=
  | l = separated_or_terminated_list(SEMICOLON | COMMA, type_member); { l }

%public let type_member :=
  | s = property_signature; { s }
  | s = call_signature; { s }
  | s = construct_signature; { s }
  | s = index_signature; { s }
  | s = method_signature; { s }

%public let property_signature :=
  | n = property_name; opt = boption(QMARK); ta = type_annotation; { `PropertySignature(n, ta, opt) }

%public let construct_signature :=
  | NEW; cs = call_signature; { `ConstructSignature(cs) }

%public let index_signature :=
  | s = delimited(LBRACKET, b = binding_identifier; COLON; PRIM_STRING; { b }, RBRACKET); ta = type_annotation; { `IndexSignature(s, ta) }
  | s = delimited(LBRACKET, b = binding_identifier; COLON; PRIM_NUMBER; { b }, RBRACKET); ta = type_annotation; { `IndexSignature(s, ta) }

%public let method_signature :=
  | n = property_name; opt = boption(QMARK); ca = call_signature; { `MethodSignature(n, ca, opt) }
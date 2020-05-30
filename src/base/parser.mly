
%start <Ts.declaration list> main

%%

let main :=
  | decl = declaration*; EOF; { decl }

(***************************************
 *  Global
 ***************************************)

let declaration :=
  | d = declaration_namespace; { d }
  | d = declaration_function; { d }
  | d = declaration_enum; { d }
  | d = declaration_interface; { d }
  | d = declaration_type; { d }

let type_ :=
  | t = type_union_or_intersection_or_primary; { t }
  | t = type_function; { t }
  | t = type_constructor; { t }

let type_annotation :=
  | COLON; t = type_; { t }

(***************************************
 *  Properties
 ***************************************)
let property_name :=
  | i = identifier_name; { Ts.PIdentifier(i) }
  | s = STRING; { let (s, pi, _) = s in Ts.PString({ Ts.pi; item = s }) }
  | n = NUMBER; { let (n, pi) = n in Ts.PNumber({ Ts.pi; item = int_of_float(n) }) }
  
let type_parameters :=
  | tp = delimited(LT, separated_nonempty_list(COMMA, type_parameter), GT); { tp }

let type_parameter :=
  | p = identifier_name; { Ts.{ tp_ident = p; tp_extends = None; tp_default = None } }
  | p = identifier_name; EXTENDS; ex = type_; { Ts.{ tp_ident = p; tp_extends = Some(ex); tp_default = None } }
  | p = identifier_name; EQUALS; eq = type_; { Ts.{ tp_ident = p; tp_extends = None; tp_default = Some(eq) } }
  | p = identifier_name; EXTENDS; ex = type_; EQUALS; eq = type_; { Ts.{ tp_ident = p; tp_extends = Some(ex); tp_default = Some(eq) } }

(***************************************
 *  Namespaces
 ***************************************)

let declaration_namespace :=
  | pi = NAMESPACE; i = identifier_path; body = delimited(LPAREN, namespace_body, RPAREN); { Ts.Namespace({ pi; item = { n_ident = i; n_declarations = body } }) }

let namespace_body :=
  | b = namespace_element+; { b }

let namespace_element :=
  | d = declaration; { { Ts.ni_item = d; ni_exported = false } }
  | d = namespace_element_export; { { Ts.ni_item = d; ni_exported = true } }

let namespace_element_export :=
  | EXPORT; d = declaration; { d }

(***************************************
 *  Types
 ***************************************)

let declaration_type :=
  | pi = TYPE; i = identifier_name; tp = type_parameters?; EQUALS; te = type_; { Ts.Type({ pi; item = {t_ident = i; t_parameters = tp; t_type = te}}) }

let type_union_or_intersection_or_primary :=
  | t = type_union; { t }
  | t = type_intersection_or_primary; { t }

let type_intersection_or_primary :=
  | t = type_intersection; { t }
  | t = type_primary; { t }

let type_primary :=
  // | t = type_parenthesized; { t }
  | t = type_predefined; { t }
  | t = type_reference; { Ts.TypeReference(t) }
  | t = type_object; { Ts.Object(t) }
  | t = type_array; { t }
  | t = type_tuple; { t }
  | t = type_query; { t }
  | t = type_this; { t }

// let type_parenthesized :=
//   | t = delimited(LPAREN, type_, RPAREN); { t }

(*
  Function
*)
let type_function :=
  | tp = type_parameters?; pl = delimited(LPAREN, loption(parameter_list), RPAREN); ARROW; ret = type_; { Ts.Function({ f_parameters = tp; f_body = pl; f_ret = ret }) }
let type_constructor :=
  | NEW; tp = type_parameters?; pl = delimited(LPAREN, loption(parameter_list), RPAREN); ARROW; ret = type_; { Ts.Constructor({ f_parameters = tp; f_body = pl; f_ret = ret }) }

(*
  Intersection
*)
let type_intersection :=
  | x = type_intersection_or_primary; AMPERSAND; y = type_primary; { Ts.Intersection(x,y) }

(*
  Union
*)
let type_union :=
  | x = type_union_or_intersection_or_primary; PIPE; y = type_intersection_or_primary; { Ts.Union(x,Some(y)) }

(*
    Predefined types
*)
let type_predefined :=
  | pi = PRIM_STRING;       { Ts.String(pi) }
  | pi = PRIM_NUMBER;       { Ts.Number(pi) }
  | pi = PRIM_BOOLEAN;      { Ts.Boolean(pi) }
  | pi = PRIM_NULL;         { Ts.Null(pi) }
  | pi = PRIM_UNDEFINED;    { Ts.Undefined(pi) }
  | pi = PRIM_VOID;         { Ts.Void(pi) }
  | pi = PRIM_ANY;          { Ts.Any(pi) }
  | pi = SYMBOL;            { Ts.Symbol(pi) }
  | s =  literal_string;    { s }
  | n =  literal_number;    { n }

(*
    Type query
*)
let type_query :=
  | TYPEOF; ip = identifier_path; { Ts.Query(ip) }

(*
    This
*)
let type_this :=
  | pi = THIS; { Ts.This(pi) }

(*
    Array
*)
let type_array :=
  | t = type_primary; LBRACKET; RBRACKET; { Ts.Array(t) }
  | ARRAY; t = delimited(LT, type_, GT); { Ts.Array(t) }

(*
    Tuple
*)
let type_tuple :=
  | l = delimited(LBRACKET, separated_or_terminated_list(COMMA, type_), RBRACKET); { Ts.Tuple(l) }

(*
    Object
*)
let type_object :=
  | t = delimited(LCURLY, type_member_list?, RCURLY); { t }

let type_member_list :=
  | l = separated_or_terminated_list(SEMICOLON | COMMA, type_member); { l }

let type_member :=
  | s = property_signature; { s }
  | s = call_signature; { Ts.CallSignature(s) }
  | s = construct_signature; { s }
  | s = index_signature; { s }
  | s = method_signature; { s }

let property_signature :=
  | n = property_name; opt = boption(QMARK); ta = type_annotation; { Ts.PropertySignature({ ps_property_name = n; ps_type_annotation = ta; ps_optional = opt }) }

let construct_signature :=
  | NEW; cs = call_signature; { Ts.ConstructSignature(cs) }

let index_signature :=
  | s = delimited(LBRACKET, b = identifier_name; COLON; PRIM_STRING; { b }, RBRACKET); ta = type_annotation; { Ts.IndexSignature(s, ta) }
  | s = delimited(LBRACKET, b = identifier_name; COLON; PRIM_NUMBER; { b }, RBRACKET); ta = type_annotation; { Ts.IndexSignature(s, ta) }

let method_signature :=
  | n = property_name; opt = boption(QMARK); ca = call_signature; { Ts.MethodSignature({ ms_property_name = n; ms_call_signature = ca; ms_optional = opt }) }

(***************************************
 *  Enums
 ***************************************)

let declaration_enum :=
  | c = boption(CONST); pi = ENUM; i = identifier_name; m = loption(delimited(LPAREN, enum_body, RPAREN)); { Ts.Enum({ pi; item = { e_ident = i; e_const = c; e_members = m } }) }

let enum_body :=
  | b = separated_or_terminated_list(COMMA, enum_member); { b }

let enum_member :=
  | n = property_name; { { Ts.em_property_name = n; em_value = None } }
  | n = property_name; EQUALS; v = enum_value; { { Ts.em_property_name = n; em_value = Some(v) } }

let enum_value :=
  | v = STRING; { let (s, pi, _) = v in Ts.EMVString({ pi; item = s }) }
  | v = NUMBER; { let (n, pi) = v in Ts.EMVNumber({ pi; item = int_of_float(n) }) }

(***************************************
 *  Functions
 ***************************************)

let declaration_function :=
  | pi = FUNCTION; i = identifier_name; cs = call_signature;  { Ts.FunctionDec({ pi; item = { f_ident = i; f_call_signature = cs } }) }

let call_signature :=
  | tp = type_parameters?; pa = delimited(LPAREN, parameter_list, RPAREN); ta = type_annotation?; { { Ts.cs_type_parameters = tp; cs_parameter_list = pa; cs_type_annotation = ta } }

let parameter_list :=
  | l = separated_nonempty_list(COMMA, parameter); rp = rest_parameter?; { l @ match rp with | None -> [] | Some(rp) -> [rp] }

let parameter :=
  | rp = parameter_required; { rp }
  | op = parameter_optional; { op }

let rest_parameter :=
  | re = rest_element; ta = type_annotation?; { Ts.RestParameter(re, ta) }

let parameter_required :=
  | acm = accessibility_modifier?; ib = identifier_or_binding; ta = type_annotation?; { Ts.Parameter({ p_optional = false; p_ibinding = ib; p_type_annotation = ta; p_accessibility = acm }) }

let accessibility_modifier ==
  | PUBLIC; { Ts.Public }
  | PROTECTED; { Ts.Protected }
  | PRIVATE; { Ts.Private }

let parameter_optional :=
  | acm = accessibility_modifier?; ib = identifier_or_binding; QMARK; ta = type_annotation?; { Ts.Parameter({ p_optional = true; p_ibinding = ib; p_type_annotation = ta; p_accessibility = acm }) }

(***************************************
 *  Interfaces
 ***************************************)

let declaration_interface :=
  | i = identifier_name; tp = type_parameters?; ex = interface_extends_clause?; ob = type_object; { Ts.Interface({ pi = i.pi; item = { i_ident = i; i_parameters = tp; i_extends = ex; i_members = ob }}) }

let interface_extends_clause :=
  | EXTENDS; ex = class_or_interface_type_list; { ex }

let class_or_interface_type_list :=
  | l = separated_or_terminated_list(COMMA, class_or_interface_type); { l }

let class_or_interface_type :=
  | tr = type_reference; { tr }

(***************************************
 *  Identifiers
 ***************************************)

let type_reference :=
  | tn = identifier_path; ta = type_arguments?; { (tn, ta) }

let type_arguments :=
  | ta = delimited(LT, separated_nonempty_list(COMMA, type_), GT); { ta }

let identifier_name :=
  | i = IDENT; { let (i, pi) = i in { Ts.pi; item = i } }

let identifier_path :=
  | l = separated_nonempty_list(DOT, identifier_name); { l }

let identifier_or_binding :=
  | bi = binding_identifier; { bi }
  | bp = binding_pattern; { bp }

let binding_identifier :=
  | i = identifier_name; { Ts.IdentifierBinding(i) }

let binding_pattern :=
  | bp = object_binding_pattern; { bp }
  | bp = array_binding_pattern; { bp }

let object_binding_pattern :=
  | l = delimited(LCURLY, separated_or_terminated_list(COMMA, identifier_name), RCURLY); { Ts.ObjectBinding(l) }

let array_binding_pattern :=
  | l = delimited(LBRACKET, rest_element, RBRACKET); { Ts.ArrayBinding([], Some(l)) }
  | LBRACKET; l = separated_or_terminated_list(COMMA, identifier_name); rp = rest_element?; RBRACKET; { Ts.ArrayBinding(l, rp) }

let rest_element :=
  | ELLIPSIS; i = identifier_name; { i }

(***************************************
 *  Literals
 ***************************************)

let literal_string :=
  | s = STRING; { let (s, pi ,_) = s in Ts.StringLiteral({ pi; item = s }) }

let literal_number :=
  | n = NUMBER; { let (n, pi) = n in Ts.NumberLiteral({pi; item = int_of_float(n) }) }

(***************************************
 *  Utils
 ***************************************)

let separated_or_terminated_list(separator, X) :=
  | x=X; { [x] }
  | x=X; separator; { [x] }
  | x=X; separator; xs=separated_or_terminated_list(separator, X); { x :: xs }

let maybe_as_bool(X) :=
  v = X?; { match v with | None -> false | Some _ -> true }

let maybe_as_list(X) :=
    v = X?; { match v with | None -> [] | Some v -> v }
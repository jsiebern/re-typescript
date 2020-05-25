
%start <Ts.declaration list> main

%%

let main :=
  | decl = declaration*; EOF; { decl }

(*
  Global
*)

%public let declaration :=
  | d = declaration_namespace; { d }
  | d = declaration_function; { d }
  | d = declaration_enum; { d }
  | d = declaration_interface; { d }
  | d = declaration_type; { d }

%public let type_annotation :=
  | COLON; t = type_; { t }

(*
  Properties
*)
%public let property_name :=
  | i = identifier_name; { `Identifier(i) }
  | s = literal_string; { s }
  | n = literal_number; { n }
  
%public let type_parameters :=
  | tp = delimited(LT, separated_nonempty_list(COMMA, type_parameter), GT); { tp }

%public let type_parameter :=
  | p = identifier_name; { `TypeParameter(p, None) }
  | p = identifier_name; EXTENDS; ex = type_; { `TypeParameter(p, Some(ex)) }

%public let type_arguments :=
  | ta = delimited(LT, separated_nonempty_list(COMMA, type_), GT); { ta }


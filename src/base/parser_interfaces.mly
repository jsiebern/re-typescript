%%

%public let declaration_interface :=
  | i = identifier_name; tp = type_parameters?; ex = interface_extends_clause?; ob = type_object; { `Interface(i, tp, ex, ob ) }

let interface_extends_clause :=
  | EXTENDS; ex = class_or_interface_type_list; { ex }

let class_or_interface_type_list :=
  | l = separated_or_terminated_list(COMMA, class_or_interface_type); { l }

let class_or_interface_type :=
  | tr = type_reference; { tr }
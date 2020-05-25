%%

%public let declaration_namespace :=
  | pi = NAMESPACE; i = identifier_path; body = delimited(LPAREN, namespace_body, RPAREN); { Ts.Namespace({ pi; item = { n_ident = i; n_declarations = body } }) }

let namespace_body :=
  | b = namespace_element+; { b }

let namespace_element :=
  | d = declaration; { { Ts.ni_item = d; ni_exported = false } }
  | d = namespace_element_export; { { Ts.ni_item = d; ni_exported = true } }

let namespace_element_export :=
  | EXPORT; d = declaration; { d }
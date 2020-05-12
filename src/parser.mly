%token TYPE INTERFACE
%token <string> IDENT
%token <string> STRING
%token EQUALS
%token EXTENDS
%token GT LT
%token PRIM_STRING PRIM_NUMBER PRIM_BOOLEAN
%token SEMICOLON COLON COMMA QMARK DOT
%token LCURLY RCURLY LPAREN RPAREN
%token EOF

%start <Ts.type_def list> main
%%

let main :=
  ~ = list(type_def); EOF; <>

type_def:
  | TYPE; name = IDENT; EQUALS; t = type_; SEMICOLON? { `TypeDef(name, t) }
  | TYPE; name = IDENT; EQUALS; LCURLY; obj = separated_or_terminated_list(COMMA, obj_field); RCURLY; SEMICOLON? { `TypeDef(name, `Obj(obj)) }
  | INTERFACE; name = IDENT; extends = extends; LCURLY; obj = separated_or_terminated_list(SEMICOLON, obj_field); RCURLY; SEMICOLON? { `InterfaceDef(name, extends, obj) }

let extends :=
  | { [] }
  | EXTENDS; v = ref_; { v }

let ref_ :=
  ~ = separated_or_terminated_list(DOT, ref_part); <>

let ref_part :=
  | name = IDENT; { (name, []) }
  | name = IDENT; LT; t = separated_or_terminated_list(COMMA, type_); GT; { (name, t) }


separated_or_terminated_list(separator, X):
  | x=X { [x] }
  | x=X separator { [x] }
  | x=X separator xs=separated_or_terminated_list(separator, X) { x :: xs }

obj_field:
  | k = STRING; r = QMARK?; COLON; t = type_    { {name = k; type_ = t; required = match r with | None -> false | Some _ -> true } }
  | k = IDENT; r = QMARK?; COLON; t = type_     { {name = k; type_ = t; required = match r with | None -> false | Some _ -> true } } ;

type_:
  | PRIM_STRING       { `String }
  | PRIM_NUMBER       { `Number }
  | PRIM_BOOLEAN      { `Boolean }
  | r = ref_          { `Ref(r) };
(* Keywords *)
%token <Parse_info.t>
  TYPE
  INTERFACE
  ARRAY
  ENUM
  EXTENDS
  READONLY
  IMPORT EXPORT DEFAULT
  FROM
  AS IN
  DECLARE
  FALSE TRUE
  INSTANCEOF
  TYPEOF
  VAR LET CONST
  FUNCTION
  NAMESPACE

(* With value *)
%token <string * Parse_info.t>       IDENT
%token <string * Parse_info.t * int> STRING
%token <float * Parse_info.t>        NUMBER

(* Primitive *)
%token <Parse_info.t>
  PRIM_STRING
  PRIM_NUMBER
  PRIM_BOOLEAN
  PRIM_NULL
  PRIM_UNDEFINED
  PRIM_VOID
  PRIM_ANY

(* Operators *)
%token <Parse_info.t>
  EQUALS
  QMARK
  DOT
  AMPERSAND
  STAR
  PIPE

(* Syntax *)
%token <Parse_info.t>
  LCURLY RCURLY
  LPAREN RPAREN
  LBRACKET RBRACKET
  LT GT
  SEMICOLON COLON COMMA

(* Noise *)
%token <string * Parse_info.t> UNKNOWN
%token <string * Parse_info.t> COMMENT
%token <string * Parse_info.t> COMMENT_LINE

%token <Parse_info.t> EOF

%start <Ts.toplevel> main
%%

let main :=
  | expr = expr*;  EOF; { Ts.make_top_level(expr) }

let expr :=
  | import = import; { `I(import) }
  | type_def = type_def; { `T(type_def) }
  | COMMENT; { `Empty }
  | COMMENT_LINE; { `Empty }

type_def:
  | export = opt_as_bool(EXPORT); TYPE; name = IDENT; args = opt_as_list(type_args); EQUALS; t = type_or_union; SEMICOLON?; { (`TypeDef(fst(name), t, args), export) }
  | export = opt_as_bool(EXPORT); INTERFACE; name = IDENT; args = opt_as_list(type_args); extends = extends; LCURLY; obj = maybe_separated_or_terminated_list(obj_separator, obj_field); RCURLY; SEMICOLON?; { (`InterfaceDef(fst(name), extends, obj, args), export) }
  | export = opt_as_bool(EXPORT); is_const = opt_as_bool(CONST); ENUM; name = IDENT; LCURLY; members = separated_nonempty_list(COMMA, enum_member); RCURLY; SEMICOLON?; { (`EnumDef(fst(name), members, is_const), export) }

let type_args :=
  | LT; args = separated_or_terminated_list(COMMA, type_arg); GT; { args }

let type_arg :=
  | name = IDENT; EXTENDS; ext = type_; EQUALS; eq = type_; { { Ts.constraint_=Some(ext); name=fst(name); default=Some(eq) } }
  | name = IDENT; EXTENDS; ext = type_; { { Ts.constraint_=Some(ext); name=fst(name); default=None } }
  | name = IDENT; EQUALS; eq = type_; { { Ts.constraint_=None; name=fst(name); default=Some(eq) } }
  | name = IDENT; { { Ts.constraint_=None; name=fst(name); default=None } }

let enum_member :=
  | name = IDENT; EQUALS; v = prim_value; { { Ts.key=fst(name); default=Some(v) }  }
  | name = IDENT; { { Ts.key=fst(name); default=None } }

let prim_value :=
  | s = STRING; { let (s,_,_) = s in `V_String(s) }
  | n = NUMBER; { `V_Number(fst(n) |> int_of_float) }

let extends :=
  | { [] }
  | EXTENDS; v = ref_; { v }

let ref_ :=
  ~ = separated_or_terminated_list(DOT, ref_part); <>

let ref_part :=
  | name = IDENT; { (fst(name), []) }
  | name = IDENT; LT; t = separated_or_terminated_list(COMMA, type_or_union); GT; { (fst(name), t) }

obj_field:
  | ro = opt_as_bool(READONLY); key = STRING; optional = opt_as_bool(QMARK); COLON; t = type_or_union    { let (key,_,_) = key in {Ts.key; type_ = t; optional; readonly = ro } }
  | ro = opt_as_bool(READONLY); key = IDENT; optional = opt_as_bool(QMARK); COLON; t = type_or_union     { let (key,_) = key in {Ts.key; type_ = t; optional; readonly = ro } } ;

let type_or_union :=
  | u = union; { u }
  | t = type_; { t }

type_:
  (* Base types *)
  | PRIM_STRING;       { `String }
  | PRIM_NUMBER;       { `Number }
  | PRIM_BOOLEAN;      { `Boolean }
  | PRIM_NULL;         { `Null }
  | PRIM_UNDEFINED;    { `Undefined }
  | PRIM_VOID;         { `Void }
  | PRIM_ANY;          { `Any }
  (* Tuple *)
  | LBRACKET; types = separated_nonempty_list(COMMA, type_or_union); RBRACKET; { `Tuple(types) }
  (* Inline obj *)
  | LCURLY; obj = maybe_separated_or_terminated_list(obj_separator, obj_field); RCURLY; { `Obj(obj) }
  (* Arrays *)
  | ARRAY; LT; t = type_or_union; GT;               { `Array(t) }
  | t = type_; LBRACKET; RBRACKET;                  { `Array(t) }
  | LPAREN; t = union; RPAREN; LBRACKET; RBRACKET;  { `Array(t) }
  (* Type extraction *)
  | r = ref_; fa = nonempty_list(field_access); { `TypeExtract(r, fa) }
  (* Reference *)
  | r = ref_; { `Ref(r) }
  
let union :=
  | u_values = separated_nonempty_list(PIPE, union_value); { `Union(u_values) }

let union_value :=
  | b = boolean_literal; { `U_Bool(b) }
  | s = STRING; { let (s,_,_) = s in `U_String(s) }
  | n = NUMBER; { `U_Number(fst(n) |> int_of_float) }
  | t = type_; { `U_Type(t) }

(*
  Imports
*)

let import :=
  | IMPORT; TYPE?; name = import_alias; FROM; path = STRING; SEMICOLON?; { let (path, _, _) = path in { Ts.path; name } }

let import_alias :=
  | name = IDENT; { `Named(fst(name)) }
  | name = IDENT; AS; alias = IDENT; { `Alias(`Named(fst(name)), fst(alias)) }
  | STAR; AS; alias = IDENT; { `Alias(`Star, fst(alias)) }
  | LCURLY; lst = separated_or_terminated_list(COMMA, import_alias); AS; alias = IDENT; RCURLY; { `Alias(`List(lst), fst(alias)) }
  | LCURLY; lst = separated_or_terminated_list(COMMA, import_alias); RCURLY; { `List(lst) }

(*
  Utility functions
*)

let maybe_separated_or_terminated_list(separator, X) :=
  v = separated_or_terminated_list(separator, X)?; { match v with | None -> [] | Some(v) -> v }

separated_or_terminated_list(separator, X):
  | x=X { [x] }
  | x=X separator { [x] }
  | x=X separator xs=separated_or_terminated_list(separator, X) { x :: xs }

let opt_as_bool(X) :=
  v = X?; { match v with | None -> false | Some _ -> true }

let opt_as_list(X) :=
  v = X?; { match v with | None -> [] | Some v -> v }

let obj_separator :=
  | COMMA; { Some(()) }
  | SEMICOLON; { Some(()) }
  | { None }

let field_access :=
  | LBRACKET; s = STRING; RBRACKET; { let (s,_,_) = s in s }

let boolean_literal :=
  | TRUE; { true }  
  | FALSE; { false }
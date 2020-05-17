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
  | imports = import*; types = type_def*; EOF; { { types; imports } }

type_def:
  | export = opt_as_bool(EXPORT); TYPE; name = IDENT; EQUALS; t = type_; SEMICOLON?; { (`TypeDef(fst(name), t), export) }
  | export = opt_as_bool(EXPORT); INTERFACE; name = IDENT; extends = extends; LCURLY; obj = maybe_separated_or_terminated_list(obj_separator, obj_field); RCURLY; SEMICOLON?; { (`InterfaceDef(fst(name), extends, obj), export) }
  | export = opt_as_bool(EXPORT); is_const = opt_as_bool(CONST); ENUM; name = IDENT; LCURLY; members = separated_nonempty_list(COMMA, enum_member); RCURLY; SEMICOLON?; { (`EnumDef(fst(name), members, is_const), export) }

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
  | name = IDENT; LT; t = separated_or_terminated_list(COMMA, type_); GT; { (fst(name), t) }

obj_field:
  | ro = opt_as_bool(READONLY); key = STRING; optional = opt_as_bool(QMARK); COLON; t = type_    { let (key,_,_) = key in {Ts.key; type_ = t; optional; readonly = ro } }
  | ro = opt_as_bool(READONLY); key = IDENT; optional = opt_as_bool(QMARK); COLON; t = type_     { let (key,_) = key in {Ts.key; type_ = t; optional; readonly = ro } } ;

type_:
  (* Base types *)
  | PRIM_STRING;       { `String }
  | PRIM_NUMBER;       { `Number }
  | FALSE
  | TRUE
  | PRIM_BOOLEAN;      { `Boolean }
  | PRIM_NULL;         { `Null }
  | PRIM_UNDEFINED;    { `Undefined }
  | PRIM_VOID;         { `Void }
  | PRIM_ANY;          { `Any }
  (* Tuple *)
  | LBRACKET; types = separated_nonempty_list(COMMA, type_); RBRACKET; { `Tuple(types) }
  (* Inline obj *)
  | LCURLY; obj = maybe_separated_or_terminated_list(obj_separator, obj_field); RCURLY; { `Obj(obj) }
  (* Arrays *)
  | ARRAY; LT; t = type_; GT;       { `Array(t) }
  | t = type_; LBRACKET; RBRACKET;  { `Array(t) }
  (* Type extraction *)
  | r = ref_; fa = nonempty_list(field_access); { `TypeExtract(r, fa) }
  (* Reference *)
  | r = ref_; { `Ref(r) }


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

let obj_separator :=
  | COMMA; { Some(()) }
  | SEMICOLON; { Some(()) }
  | { None }

let field_access :=
  | LBRACKET; s = STRING; RBRACKET; { let (s,_,_) = s in s }
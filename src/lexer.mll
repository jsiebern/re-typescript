{
open Lexing
open Parser

exception SyntaxError of string

let next_line lexbuf =
  let pos = lexbuf.lex_curr_p in
  lexbuf.lex_curr_p <-
    { pos with pos_bol = lexbuf.lex_curr_pos;
               pos_lnum = pos.pos_lnum + 1
    }
}

let white = [' ' '\t']+
let newline = '\r' | '\n' | "\r\n"
let ident = ['a'-'z' 'A'-'Z' '0'-'9' '_' '.']+

rule read =
  parse
  | white                   { read lexbuf }
  | newline                 { next_line lexbuf; read lexbuf }
  | "type"                  { TYPE }
  | "interface"             { INTERFACE }
  | "extends"               { EXTENDS }
  | "import"                { IMPORT }
  | "from"                  { FROM }
  | "as"                    { AS }
  | '*'                     { STAR }
  | '='                     { EQUALS }
  | '?'                     { QMARK }
  | ';'                     { SEMICOLON }
  | ':'                     { COLON }
  | '.'                     { DOT }
  | '<'                     { LT }
  | '>'                     { GT }
  | ','                     { COMMA }
  | '{'                     { LCURLY }
  | '}'                     { RCURLY }
  | '('                     { LPAREN }
  | ')'                     { RPAREN }
  | ['"' '\'']              { read_string (Buffer.create 17) lexbuf }
  | "string"                { PRIM_STRING }
  | "number"                { PRIM_NUMBER }
  | "boolean"               { PRIM_BOOLEAN }
  | ident as v              { IDENT ( v ) }
  | _                       { read lexbuf }
  | eof                     { EOF }

and read_string buf =
  parse
  | ['"' '\'']{ STRING (Buffer.contents buf) }
  | '\\' '/'  { Buffer.add_char buf '/'; read_string buf lexbuf }
  | '\\' '\\' { Buffer.add_char buf '\\'; read_string buf lexbuf }
  | '\\' 'b'  { Buffer.add_char buf '\b'; read_string buf lexbuf }
  | '\\' 'f'  { Buffer.add_char buf '\012'; read_string buf lexbuf }
  | '\\' 'n'  { Buffer.add_char buf '\n'; read_string buf lexbuf }
  | '\\' 'r'  { Buffer.add_char buf '\r'; read_string buf lexbuf }
  | '\\' 't'  { Buffer.add_char buf '\t'; read_string buf lexbuf }
  | [^ '"' '\'' '\\']+
    { Buffer.add_string buf (Lexing.lexeme lexbuf);
      read_string buf lexbuf
    }
  | _ { raise (SyntaxError ("Illegal string character: " ^ Lexing.lexeme lexbuf)) }
  | eof { raise (SyntaxError ("String is not terminated")) }
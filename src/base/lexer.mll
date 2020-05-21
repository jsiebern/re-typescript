{
  open Lexing
  open Parser

  exception SyntaxError of string

  let tok lexbuf = Lexing.lexeme lexbuf

  let keyword_table =
    let h = Hashtbl.create 31 in
    List.iter (fun (s,f) -> Hashtbl.add h s f ) [
      "false",      (fun ii -> FALSE ii);
      "function",   (fun ii -> FUNCTION ii);
      "in",         (fun ii -> IN ii);
      "instanceof", (fun ii -> INSTANCEOF ii);
      "true",       (fun ii -> TRUE ii);
      "typeof",     (fun ii -> TYPEOF ii);
      "var",        (fun ii -> VAR ii);
      "let",        (fun ii -> LET ii);
      "const",      (fun ii -> CONST ii);
      "default",    (fun ii -> DEFAULT ii);
      "extends",    (fun ii -> EXTENDS ii);
      "readonly",   (fun ii -> READONLY ii);
      "import",     (fun ii -> IMPORT ii);
      "export",     (fun ii -> EXPORT ii);
      "from",       (fun ii -> FROM ii);
      "as",         (fun ii -> AS ii);
      "type",       (fun ii -> TYPE ii);
      "interface",  (fun ii -> INTERFACE ii);
      "Array",      (fun ii -> ARRAY ii);
      "enum",       (fun ii -> ENUM ii);
      "string",     (fun ii -> PRIM_STRING ii);
      "number",     (fun ii -> PRIM_NUMBER ii);
      "boolean",    (fun ii -> PRIM_BOOLEAN ii);
      "null",       (fun ii -> PRIM_NULL ii);
      "undefined",  (fun ii -> PRIM_UNDEFINED ii);
      "void",       (fun ii -> PRIM_VOID ii);
      "any",        (fun ii -> PRIM_ANY ii);
      "declare",    (fun ii -> DECLARE ii);
      "namespace",  (fun ii -> NAMESPACE ii);
      "module",     (fun ii -> MODULE ii);
    ];
    h

  let update_loc lexbuf ?file ~line ~absolute chars =
    let pos = lexbuf.Lexing.lex_curr_p in
    let new_file = match file with
                  | None -> pos.pos_fname
                  | Some s -> s
    in
    lexbuf.Lexing.lex_curr_p <- { pos with
      pos_fname = new_file;
      pos_lnum = if absolute then line else pos.pos_lnum + line;
      pos_bol = pos.pos_cnum - chars;
                                }

  let tokinfo lexbuf = Parse_info.t_of_lexbuf lexbuf

  let with_pos lexbuf f =
    let p = lexbuf.Lexing.lex_start_p in
    let pos = lexbuf.Lexing.lex_start_pos in
    let r = f () in
    lexbuf.Lexing.lex_start_p <- p;
    lexbuf.Lexing.lex_start_pos <- pos;
    r
}

let NEWLINE = '\r' | '\n' | "\r\n"
let hexa = ['0'-'9''a'-'f''A'-'F']
let inputCharacter = [^ '\r' '\n' ]

rule read =
  parse
  (* Noise *)
  | "/*" {
      with_pos lexbuf (fun () ->
      let info = tokinfo lexbuf in
      let buf = Buffer.create 127 in
      Buffer.add_string buf (tok lexbuf);
      st_comment buf lexbuf;
      let content = Buffer.contents buf in
      COMMENT(content, info))
    }
  | ("//#" [' ' '\t' ]*
     (['0'-'9']+ as line) [' ' '\t' ]*
     '"' ([^ '"' '\n']* as file) '"' [' ' '\t' ]*
    ) as raw NEWLINE {
      let info = tokinfo lexbuf in
      let line = int_of_string line in
      update_loc lexbuf ~file ~line ~absolute:true 0;
      COMMENT_LINE (raw, info)
    }
  | ("//" inputCharacter*) as cmt { COMMENT(cmt, tokinfo lexbuf) }
  | [' ' '\t' ]+ {
      read lexbuf
    }
  | NEWLINE {
      update_loc lexbuf ~line:1 ~absolute:false 0;
      read lexbuf
    }

  (* Operators *)
  | '*'                     { STAR (tokinfo lexbuf); }
  | '='                     { EQUALS (tokinfo lexbuf); }
  | '?'                     { QMARK (tokinfo lexbuf); }
  | '&'                     { AMPERSAND (tokinfo lexbuf); }
  | '|'                     { PIPE (tokinfo lexbuf); }

  (* Syntax *)
  | '{'                     { LCURLY (tokinfo lexbuf); }
  | '}'                     { RCURLY (tokinfo lexbuf); }
  | '('                     { LPAREN (tokinfo lexbuf); }
  | ')'                     { RPAREN (tokinfo lexbuf); }
  | '['                     { LBRACKET (tokinfo lexbuf); }
  | ']'                     { RBRACKET (tokinfo lexbuf); }
  | ';'                     { SEMICOLON (tokinfo lexbuf); }
  | ':'                     { COLON (tokinfo lexbuf); }
  | '.'                     { DOT (tokinfo lexbuf); }
  | '<'                     { LT (tokinfo lexbuf); }
  | '>'                     { GT (tokinfo lexbuf); }
  | ','                     { COMMA (tokinfo lexbuf); }

  (* Keywords and ident *)
  | ['a'-'z''A'-'Z''$''_']['a'-'z''A'-'Z''$''_''0'-'9']* {
      let s = tok lexbuf in
      let info = tokinfo lexbuf in
      try
        let f = Hashtbl.find keyword_table s in
        f info
      with
        | Not_found -> IDENT (s, info)
    }

  (* Strings *)
  | ("'"|'"') as quote {
      with_pos lexbuf (fun () ->
      let from = lexbuf.Lexing.lex_start_p.pos_cnum in
      let info = tokinfo lexbuf in
      let buf = Buffer.create 127 in
      string_quote quote buf lexbuf;
      let s = Buffer.contents buf in
      (* s does not contain the enclosing "'" but the info does *)
      let to_ = lexbuf.Lexing.lex_curr_p.pos_cnum in
      STRING (s, info, to_ - 1 - from))
    }

  (* Numbers *)
  | '0'['0'-'7']+ {
      let s = tok lexbuf in
      let info = tokinfo lexbuf in
      NUMBER (s |> float_of_string, info)
    }

  | ['0'-'9']*'.'?['0'-'9']+['e''E']['-''+']?['0'-'9']+ (* {1,3} *) {
      let s = tok lexbuf in
      let info = tokinfo lexbuf in
      NUMBER (s |> float_of_string, info)
    }
  | ['0'-'9']+'.'? |
    ['0'-'9']*'.'['0'-'9']+ {
      let s = tok lexbuf in
      let info = tokinfo lexbuf in
      NUMBER (s |> float_of_string, info)
    }

  | _ {
      UNKNOWN (tok lexbuf, tokinfo lexbuf)
    }
  | eof { EOF (tokinfo lexbuf); }
and string_escape quote buf = parse
  | '\\'{ Buffer.add_string buf "\\\\" }
  | 'x' hexa hexa
  | 'u' hexa hexa hexa hexa {
      Buffer.add_char buf '\\';
      Buffer.add_string buf (tok lexbuf) }
  | (_ as c)
    { if Char.(c <> '\'') && Char.(c <> '\"') then Buffer.add_char buf '\\';
      Buffer.add_char buf c }
  | eof { Format.eprintf  "LEXER: WEIRD end of file in string_escape@."; ()}

and string_quote q buf = parse
  | ("'"|'"') as q' {
    if Char.(q = q')
    then ()
    else (Buffer.add_char buf q'; string_quote q buf lexbuf) }
  | "\\" NEWLINE {
    update_loc lexbuf ~line:1 ~absolute:false 0;
    string_quote q buf lexbuf }
  | NEWLINE {
    Format.eprintf  "LEXER: WEIRD newline in quoted string@.";
    update_loc lexbuf ~line:1 ~absolute:false 0;
    Buffer.add_string buf (tok lexbuf);
    string_quote q buf lexbuf }
  | '\\' {
      string_escape q buf lexbuf;
      string_quote q buf lexbuf
    }
  | (_ as x)       { Buffer.add_char buf x; string_quote q buf lexbuf }
  | eof { Format.eprintf  "LEXER: WEIRD end of file in quoted string@."; ()}
and st_comment buf = parse
  | "*/" { Buffer.add_string buf (tok lexbuf) }
  | NEWLINE {
      update_loc lexbuf ~line:1 ~absolute:false 0;
      Buffer.add_string buf (tok lexbuf);
      st_comment buf lexbuf }
  | [^'*' '\n' '\r' ]+ { Buffer.add_string buf (tok lexbuf);st_comment buf lexbuf }
  | '*'     { Buffer.add_char buf '*';st_comment buf lexbuf }

  | eof { Format.eprintf "LEXER: end of file in comment@."; Buffer.add_string buf "*/"}
  | _  {
      let s = tok lexbuf in
      Format.eprintf "LEXER: unrecognised symbol in comment: %s@." s;
      Buffer.add_string buf s;
      st_comment buf lexbuf
    }
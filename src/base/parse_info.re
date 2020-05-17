type t = {
  src: option(string),
  name: option(string),
  col: int,
  line: int,
  idx: int,
};

let zero = {src: None, name: None, col: 0, line: 0, idx: 0};

let t_of_lexbuf = (lexbuf): t => {
  let idx = lexbuf.Lexing.lex_start_p.Lexing.pos_cnum;
  let (line, col) = (
    lexbuf.Lexing.lex_start_p.pos_lnum,
    lexbuf.Lexing.lex_start_p.pos_cnum - lexbuf.Lexing.lex_start_p.pos_bol,
  );

  let name = Some(lexbuf.Lexing.lex_start_p.pos_fname);
  let src = Some(lexbuf.Lexing.lex_start_p.pos_fname);
  {idx, line, col, name, src};
};

let start_position = (t: t) => {
  Lexing.pos_fname: CCOpt.get_or(~default="", t.name),
  pos_lnum: t.line,
  pos_bol: t.idx - t.col,
  pos_cnum: t.idx,
};

let t_of_position = (~src, pos) => {
  name: Some(pos.Lexing.pos_fname),
  src,
  line: pos.Lexing.pos_lnum,
  col: pos.Lexing.pos_cnum - pos.Lexing.pos_bol,
  idx: 0,
};

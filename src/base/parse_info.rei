type t = {
  src: option(string),
  name: option(string),
  col: int,
  line: int,
  idx: int,
};

let zero: t;

let t_of_lexbuf: Lexing.lexbuf => t;

let start_position: t => Lexing.position;

let t_of_position: (~src: option(string), Lexing.position) => t;

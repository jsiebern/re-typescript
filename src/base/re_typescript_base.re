module Ts = Ts;
module Parser = Parser;
module Parser_incr = Parser_incr;
module Lexer = Lexer;
module Error = Error;
module Parse_info = Parse_info;

let default_parser = content => {
  let lexbuf = Lexing.from_string(content |> CCString.trim);

  try(Ok(Parser_incr.parse(lexbuf))) {
  | Lexer.SyntaxError(msg) => Error(msg)
  | Parser_incr.Parsing_error(pos) =>
    Error(Error.parser_error_with_info(~content, pos))
  | Parser.Error =>
    Error(
      Error.parser_error(
        ~msg=?None,
        ~content,
        ~start=lexbuf.lex_start_p,
        ~end_=lexbuf.lex_curr_p,
      ),
    )
  | e => Error(Printexc.to_string(e))
  };
};

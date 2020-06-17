open Lexing;
open Parser;

exception Parsing_error(Parse_info.t);

module I = Parser.MenhirInterpreter;

let parse_aux = (the_parser, lexbuf) => {
  let init = the_parser(lexbuf.Lexing.lex_start_p);
  let reset = lexbuf => {
    lexbuf.Lexing.lex_curr_p = lexbuf.Lexing.lex_start_p;
    lexbuf.Lexing.lex_curr_pos = lexbuf.Lexing.lex_start_pos;
  };

  let fol = (prev, tok: Parser.token) =>
    switch (prev) {
    | [] => true
    | [p, ..._] =>
      Parser_token.info(p).Re_typescript_config.Parse_info.line
      != Parser_token.info(tok).Re_typescript_config.Parse_info.line
    };

  let rec loop_error = (prev, checkpoint) => {
    module I = Parser.MenhirInterpreter;
    switch (checkpoint) {
    | I.InputNeeded(_env) =>
      let checkpoint =
        I.offer(
          checkpoint,
          (
            Parser.EOF(Parse_info.zero),
            lexbuf.Lexing.lex_curr_p,
            lexbuf.Lexing.lex_curr_p,
          ),
        );

      loop_error(prev, checkpoint);
    | I.Shifting(_)
    | I.AboutToReduce(_) => loop_error(prev, I.resume(checkpoint))
    | I.Accepted(_) => assert(false)
    | I.Rejected => `Error(prev)
    | I.HandlingError(_) => loop_error(prev, I.resume(checkpoint))
    };
  };

  let rec loop = (prev, comments, (last_checkpoint, checkpoint)) => {
    switch (checkpoint) {
    | I.InputNeeded(_env) =>
      let inputneeded = checkpoint;
      let (token, comments) =
        switch (prev) {
        | [Parser.EOF(_) as prev, ..._] => (prev, comments)
        | _ =>
          let rec read_one = (comments, lexbuf) =>
            switch (Lexer.read(lexbuf)) {
            | (COMMENT_LINE(_) | COMMENT(_)) as t =>
              read_one([t, ...comments], lexbuf)
            | t => (t, comments)
            };

          let (t, comments) = read_one(comments, lexbuf);

          (t, comments);
        };

      let last_checkpoint = (prev, comments, inputneeded);
      let checkpoint =
        I.offer(
          checkpoint,
          (token, lexbuf.Lexing.lex_start_p, lexbuf.Lexing.lex_curr_p),
        );

      loop([token, ...prev], comments, (last_checkpoint, checkpoint));
    | I.Shifting(_)
    | I.AboutToReduce(_) =>
      loop(prev, comments, (last_checkpoint, I.resume(checkpoint)))
    | I.Accepted(v) => `Ok((v, prev, comments))
    | I.Rejected => `Error(prev)
    | I.HandlingError(e) =>
      let insert_virtual_semmit =
        switch (prev) {
        | []
        | [VIRTUAL_SEMICOLON(_), ..._] => false
        | [RCURLY(_), ..._] => true
        | [EOF(_), ..._] => true
        | [offending, before, ..._] when fol([before], offending) => true
        | _ => false
        };

      switch (insert_virtual_semmit) {
      | false => loop_error(prev, I.resume(checkpoint))
      | true =>
        let error_checkpoint = checkpoint;
        let error_prev = prev;
        let (prev, comments, checkpoint) = last_checkpoint;
        if (I.acceptable(
              checkpoint,
              VIRTUAL_SEMICOLON(Parse_info.zero),
              lexbuf.Lexing.lex_curr_p,
            )) {
          reset(lexbuf);
          let t = VIRTUAL_SEMICOLON(Parse_info.zero);
          let checkpoint =
            I.offer(
              checkpoint,
              (t, lexbuf.Lexing.lex_curr_p, lexbuf.Lexing.lex_curr_p),
            );

          loop([t, ...prev], comments, (last_checkpoint, checkpoint));
        } else {
          loop_error(error_prev, I.resume(error_checkpoint));
        };
      };
    };
  };

  switch (loop([], [], (([], [], init), init))) {
  | `Ok(x) => x
  | `Error(tok) =>
    let tok =
      switch (tok) {
      | [] => EOF(Parse_info.zero)
      | [x, ..._] => x
      };

    let pi = Parser_token.info(tok);
    raise(Parsing_error(pi));
  };
};

let parse' = lex => {
  let (p, t_rev, comment_rev) = parse_aux(Parser.Incremental.module_, lex);
  (p, List.rev(t_rev), List.rev(comment_rev));
};

let parse = lex => {
  let (p, _, _) = parse_aux(Parser.Incremental.module_, lex);
  p;
};

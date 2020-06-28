module SymbolId: {type t = pri int;} = {
  type t = int;
};
module NodeId: {type t = pri int;} = {
  type t = int;
};
module TypeId: {type t = pri int;} = {
  type t = int;
};

let from_int: int => 'a = a => Obj.magic(a);
let to_int: 'a => int = a => Obj.magic(a);

type symbol_id = SymbolId.t;
let read_symbol_id: (Yojson.Safe.lexer_state, Lexing.lexbuf) => symbol_id =
  (lexestate, lexbuf) => {
    Yojson.Raw.read_space(lexestate, lexbuf);
    Yojson.Basic.read_int(lexestate, lexbuf) |> from_int;
  };
let write_symbol_id = (buf, flags: symbol_id) =>
  Yojson.Basic.write_int(buf, to_int(flags));

type node_id = NodeId.t;
let read_node_id: (Yojson.Safe.lexer_state, Lexing.lexbuf) => node_id =
  (lexestate, lexbuf) => {
    Yojson.Raw.read_space(lexestate, lexbuf);
    Yojson.Basic.read_int(lexestate, lexbuf) |> from_int;
  };
let write_node_id = (buf, flags: node_id) =>
  Yojson.Basic.write_int(buf, to_int(flags));

type type_id = TypeId.t;
let read_type_id: (Yojson.Safe.lexer_state, Lexing.lexbuf) => type_id =
  (lexestate, lexbuf) => {
    Yojson.Raw.read_space(lexestate, lexbuf);
    Yojson.Basic.read_int(lexestate, lexbuf) |> from_int;
  };
let write_type_id = (buf, flags: type_id) =>
  Yojson.Basic.write_int(buf, to_int(flags));

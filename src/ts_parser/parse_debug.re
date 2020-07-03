module Ts = Typescript_t;
module Types = Re_typescript_printer.Tree_types;
open Types;

PastelConsole.init();

let pp_node = node =>
  Typescript_j.string_of_node(node)
  |> Yojson.Basic.from_string
  |> Yojson.Basic.pretty_to_string;
let pp_type = type_ =>
  Typescript_j.string_of_type_(type_)
  |> Yojson.Basic.from_string
  |> Yojson.Basic.pretty_to_string;
let pp_node_kind = node => (node |> Typescript_unwrap.unwrap_Node).kindName;
let pp_type_kind = type_ => (type_ |> Typescript_unwrap.unwrap_Type).kindName;

let write_node = node =>
  pp_node_kind(node)
  ++ (
    Typescript_unwrap.unwrap_Name(node)
    |> CCOpt.map(name => "(" ++ name ++ ")")
    |> CCOpt.value(~default="")
  );

type path_member = [
  | `String(string)
  | `Error(string)
  | `Pos(string)
  | `Node(Ts.node)
  | `Type(Ts.type_)
  | `Pair(path_member, path_member)
  | `DecreaseIndent
];
type parse = {
  mutable json_enabled: bool,
  mutable enabled: bool,
  mutable currentEntry: string,
  mutable parsePath: Hashtbl.t(string, list(path_member)),
};
let parse = {
  json_enabled: false,
  enabled: true,
  currentEntry: "",
  parsePath: Hashtbl.create(0),
};

let put = member =>
  CCHashtbl.add_list(parse.parsePath, parse.currentEntry, member);
let add = str => put(`String(str));
let add_pos = str => put(`Pos(str));

let add_node = (node: Ts.node) => put(`Node(node));
let add_type = (type_: Ts.type_) => put(`Type(type_));
let close_pos = () => put(`DecreaseIndent);
let add_pair = (node: Ts.node, type_: Ts.type_) =>
  put(`Pair((`Node(node), `Type(type_))));
let add_node_pair = (node: Ts.node, node2: Ts.node) =>
  put(`Pair((`Node(node), `Node(node2))));
let set_current = (node: Ts.node) => {
  parse.currentEntry = write_node(node);
};
exception Parse_error_with_path(string);
let print_path = () => {
  Console.debug(
    Pastel.(
      {
        let indent_length = "   ";
        let rec pr = (path_left: list(path_member), depth) => {
          let indent =
            depth >= 0
              ? {
                CCString.repeat(indent_length, depth);
              }
              : "";
          let br = depth < 0 ? "" : "\n";
          <Pastel>
            {switch (path_left) {
             | [] => <Pastel />
             | [`DecreaseIndent, ...left] => pr(left, depth - 2)
             | [`Node(node), ...left] =>
               <Pastel>
                 indent
                 <Pastel color=Yellow> {write_node(node)} </Pastel>
                 br
                 {pr(left, depth)}
                 br
               </Pastel>
             | [`Type(type_), ...left] =>
               <Pastel>
                 indent
                 <Pastel color=Blue> {pp_type_kind(type_)} </Pastel>
                 br
                 {pr(left, depth)}
                 br
               </Pastel>
             | [`Pair(m1, m2), ...left] =>
               <Pastel>
                 indent
                 <Pastel color=White>
                   {"[ "
                    ++ {
                      pr([m1], -1);
                    }
                    ++ " > "
                    ++ {
                      pr([m2], -1);
                    }
                    ++ " ]"}
                 </Pastel>
                 br
                 {pr(left, depth)}
                 br
               </Pastel>
             | [`Pos(pos), ...left] =>
               <Pastel>
                 indent
                 indent_length
                 <Pastel underline=true color=Green> pos </Pastel>
                 br
                 {pr(left, depth + 2)}
                 br
               </Pastel>
             | [`String(str), ...left] =>
               <Pastel>
                 indent
                 <Pastel color=Cyan> str </Pastel>
                 br
                 {pr(left, depth)}
                 br
               </Pastel>
             | [`Error(str), ...left] =>
               <Pastel>
                 indent
                 <Pastel color=RedBright bold=true> str </Pastel>
                 br
                 {pr(left, depth)}
                 br
               </Pastel>
             }}
          </Pastel>;
        };
        <Pastel>
          "\n"
          <Pastel bold=true color=CyanBright underline=true>
            {parse.currentEntry ++ ":"}
            "\n"
          </Pastel>
          {switch (Hashtbl.find_opt(parse.parsePath, parse.currentEntry)) {
           | None => <Pastel color=White> "n/a" </Pastel>
           | Some(path) => pr(path |> CCList.rev, 0)
           }}
          "\n"
        </Pastel>;
      }
    ),
  );
};
let raiseWith = (~type_=?, ~node=?, str) =>
  if (parse.enabled) {
    {
      if (str !== "") {
        put(`Error(">>> " ++ str));
      };
      print_path();
    };
    if (parse.json_enabled) {
      Console.debug(
        Pastel.(
          <Pastel>
            {switch (type_, node) {
             | (None, None) => ""
             | (Some(t), None) =>
               <Pastel>
                 <Pastel bold=true underline=true color=CyanBright>
                   "Type:"
                 </Pastel>
                 <Pastel backgroundColor=WhiteBright color=Black>
                   "\n"
                   "\n"
                   "    "
                   {pp_type(t)}
                   "    "
                   "\n"
                 </Pastel>
                 "\n"
                 "\n"
               </Pastel>

             | (None, Some(n)) =>
               <Pastel>
                 <Pastel bold=true underline=true color=CyanBright>
                   "Node:"
                 </Pastel>
                 <Pastel backgroundColor=WhiteBright color=Black>
                   "\n"
                   "\n"
                   "    "
                   {pp_node(n)}
                   "    "
                   "\n"
                 </Pastel>
                 "\n"
                 "\n"
               </Pastel>
             | (Some(t), Some(n)) =>
               <Pastel>
                 <Pastel bold=true underline=true color=CyanBright>
                   "Type:"
                 </Pastel>
                 <Pastel backgroundColor=WhiteBright color=Black>
                   "\n"
                   "\n"
                   "    "
                   {pp_type(t)}
                   "    "
                   "\n"
                 </Pastel>
                 "\n"
                 "\n"
                 <Pastel bold=true underline=true color=CyanBright>
                   "Node:"
                 </Pastel>
                 <Pastel backgroundColor=WhiteBright color=Black>
                   "\n"
                   "\n"
                   "    "
                   {pp_node(n)}
                   "    "
                   "\n"
                 </Pastel>
                 "\n"
                 "\n"
               </Pastel>
             }}
          </Pastel>
        ),
      );
    };
    if (parse.enabled) {
      raise(Parse_error_with_path(str));
    };
  };
let raise = () => {
  raiseWith("");
};

open Re_typescript_base;
open Re_typescript_fs;

let content = {|
import Global2 from './global';
// export default function (a: string);

type x = Global2;
// type y = Global.other;
|};
let global = {|
export type funky = (arg: other) => boolean;
export type other = number;
// export default function(a: string): number;
export default other;
|};

let default_path = Fp.absoluteExn("/bin.d.ts");
let default_loader: module Loader.T =
  (module
   Loader_virtual.Make({
     let tbl = Hashtbl.create(0);
     Hashtbl.replace(tbl, default_path, content);
     Hashtbl.replace(tbl, Fp.absoluteExn("/global.d.ts"), global);
   }));
let default_resolver: module Resolver.T =
  (module
   Resolver.Make({
     let config = {Resolver.loader: default_loader, tsconfig: None};
   }));

let () = {
  let lexbuf = Lexing.from_string(content |> CCString.trim);

  try(
    {
      print_newline();
      Random.self_init();
      Console.log((
        Random.int(1000000),
        Random.int(1000000),
        Random.int(1000000),
        Random.int(1000000),
      ));
      Console.log("--------------------------------------");
      let str =
        Re_typescript_printer.structure_from_ts(
          ~ctx={
            ...Re_typescript_config.default_config,
            number_mode: Int,
            omit_extended_unreferenced_records: true,
            bucklescript_config: {
              ...Re_typescript_config.default_bucklescript_config,
              string_variant_mode: `BsInline,
            },
            output_type: Bucklescript,
          },
          ~parser=default_parser,
          ~resolver=default_resolver,
          default_path,
        );
      Reason_toolchain.RE.print_implementation_with_comments(
        Format.str_formatter,
        (str, []),
      );
      Console.log(Format.flush_str_formatter());

      Console.log("--------------------------------------");
      print_newline();
    }
  ) {
  | Syntaxerr.Error(msg) => Console.error(msg)
  | Lexer.SyntaxError(msg) => Console.error(msg)
  | Re_typescript_printer.Tree_utils.Exceptions.Parser_unsupported(msg, pos) =>
    Console.error(Error.parser_error_with_info(~msg, ~content, pos))
  | Parser_incr.Parsing_error(pos) =>
    Console.error(Error.parser_error_with_info(~content, pos))
  | Parser.Error =>
    Console.error(
      Error.parser_error(
        ~content,
        ~start=lexbuf.lex_start_p,
        ~end_=lexbuf.lex_curr_p,
      ),
    )
  | e =>
    Console.error(e);
    raise(e);
  };
};

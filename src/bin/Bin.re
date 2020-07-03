open Re_typescript_base;
open Re_typescript_fs;

let content = {|
interface A {
                x: string;
                y: number;
            }
            type B = A | undefined;
            type keys = keyof B;
|};
let global = {|
|};

let default_path = Fp.absoluteExn("/root/src/bin.d.ts");
let default_loader: module Loader.T =
  (module
   Loader_virtual.Make({
     let tbl = Hashtbl.create(0);
     Hashtbl.replace(tbl, default_path, content);
     Hashtbl.replace(
       tbl,
       Fp.absoluteExn("/root/src/import_source.d.ts"),
       global,
     );
   }));
let default_resolver: module Resolver.T =
  (module
   Resolver.Make({
     let config = {Resolver.loader: default_loader, tsconfig: None};
   }));

let () = {
  let r_content = ref(None);
  let r_lexbuf = ref(None);

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
        Re_typescript_ts_parser.structure_from_ts(
          ~ctx={
            ...Re_typescript_config.default_config,
            number_mode: Int,
            omit_extended_unreferenced_records: true,
            bucklescript_config: {
              ...Re_typescript_config.default_bucklescript_config,
              string_variant_mode: `Variant,
            },
            output_type: Bucklescript,
          },
          ~parser=
            content => {
              let lexbuf = Lexing.from_string(content |> CCString.trim);
              r_content := Some(content);
              r_lexbuf := Some(lexbuf);
              Ok(Parser_incr.parse(lexbuf));
            },
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
    let content = r_content^ |> CCOpt.get_exn;
    let lexbuf = r_lexbuf^ |> CCOpt.get_exn;
    Console.error(
      Error.parser_error(
        ~content,
        ~start=lexbuf.lex_start_p,
        ~end_=lexbuf.lex_curr_p,
      ),
    );
  | e =>
    Console.error(e);
    raise(e);
  };
};

open Re_typescript_base;

let content = {|
interface KeyValueProcessor {
          both: { (key: number, value: string): boolean }
      };

// declare namespace React {
//   export type ReactType<TProps> = TProps;
//   export type ReactNode = string;
//   export type ReactNodeArray = string[];
//   export type ReactElement<T> = number;
// }

// export type SemanticShorthandItemFunc<TProps> = (
//   component: React.ReactType<TProps>,
//   props: TProps,
//   children?: React.ReactNode | React.ReactNodeArray,
// ) => React.ReactElement<any> | null

// export type SemanticShorthandCollection<TProps> = SemanticShorthandItem<TProps>[]
// export type SemanticShorthandContent = React.ReactNode
// export type SemanticShorthandItem<TProps> =
//   | React.ReactNode
//   | TProps
//   | SemanticShorthandItemFunc<TProps>

// import fs = require("fs");

// declare let $: JQuery;
// export default $;

// declare namespace myLib {
//  class Cat {
//         constructor(n: number);

//         //~ We can read 'c.age' from a 'Cat' instance
//         readonly age: number;

//         //~ We can invoke 'c.purr()' from a 'Cat' instance
//         purr(): void;
//     }
//   }
|};

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
          Parser_incr.parse(lexbuf),
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

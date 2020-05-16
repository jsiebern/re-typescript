module Reason = {
  type a;
  [@bs.module "reason"] external printRE: a => string = "printRE";
  [@bs.module "reason"] external parseML: string => a = "parseML";
};

module Re_typescript_printer = {
  open Migrate_parsetree;
  open Decode_config;
  open Decode_utils;
  open Decoder;

  let print_from_ts = (~ctx: config, input: Re_typescript_base.Ts.toplevel) => {
    module Generator = (val get_decoder(ctx.output_type): Ast_generator.T);
    let ast = Generator.generate(~ctx, decode(~ctx, input));

    Pprintast.string_of_structure(ast->Obj.magic);
  };
};

let print = v => {
  let v = v |> Tablecloth.String.trim;
  let lexbuf = Lexing.from_string(v);
  try(
    Reason.printRE(
      Reason.parseML(
        Re_typescript_printer.print_from_ts(
          ~ctx=Decode_config.defaultConfig,
          Parser.main(Lexer.read, lexbuf),
        ),
      ),
    )
  ) {
  | Lexer.SyntaxError(msg) => Printf.sprintf("%s%!", msg)
  | Parser.Error =>
    ReactDOMServerRe.renderToString(Error.parser_error(~content=v, ~lexbuf))
  | e =>
    Js.log(e);
    raise(e);
  };
};
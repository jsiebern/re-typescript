module Reason = {
  type a;
  [@bs.module "reason"] external printRE: a => string = "printRE";
  [@bs.module "reason"] external parseML: string => a = "parseML";
};

module Re_typescript_printer = {
  open Migrate_parsetree;
  open Re_typescript_decode_config;
  open Re_typescript_decode_utils;
  open Re_typescript_decode;

  let print_from_ts = (~ctx: config, input: Re_typescript_base.Ts.toplevel) => {
    module Generator = (
      val get_decoder(ctx.output_type): Re_typescript_ast_generator.T
    );
    let ast = Generator.generate(~ctx, decode(~ctx, input));

    Pprintast.string_of_structure(ast->Obj.magic);
  };
};

let print = v => {
  let lexbuf = Lexing.from_string(v);
  try(
    Reason.printRE(
      Reason.parseML(
        Re_typescript_printer.print_from_ts(
          ~ctx=Re_typescript_decode_config.defaultConfig,
          Parser.main(Lexer.read, lexbuf),
        ),
      ),
    )
  ) {
  | Lexer.SyntaxError(msg) => Printf.sprintf("%s%!", msg)
  | Parser.Error =>
    Printf.sprintf(
      "At offset %d: syntax error.\n%!",
      Lexing.lexeme_start(lexbuf),
    )
  | e =>
    Js.log(e);
    raise(e);
  };
};
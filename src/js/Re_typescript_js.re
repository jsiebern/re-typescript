open Re_typescript_base;
open Re_typescript_config;
open Re_typescript_fs;
open Js_of_ocaml;

type jsstr = Js.t(Js.js_string);
Pastel.setMode(HumanReadable);

// TODO: Replace copied content from test framework
type x = {x: Hashtbl.t(Fp.t(Fp.absolute), string)};
let x = {x: Hashtbl.create(0)};
let test_loader: module Loader.T =
  (module
   Loader_virtual.Make({
     let tbl = x.x;
   }));
let test_resolver: module Resolver.T =
  (module
   Resolver.Make({
     let config = {Resolver.loader: test_loader, tsconfig: None};
   }));
let test_path = Fp.absoluteExn("/test_framework.d.ts");

let print__structure = (content, config) => {
  open Re_typescript_printer.Tree_utils.Exceptions;
  Hashtbl.replace(x.x, test_path, content);

  let lexbuf = Lexing.from_string(content |> CCString.trim);

  try(
    CCResult.Ok(
      Re_typescript_printer.structure_from_ts(
        ~ctx=config,
        ~parser=
          content =>
            Ok(
              Parser_incr.parse(
                Lexing.from_string(content |> CCString.trim),
              ),
            ),
        ~resolver=test_resolver,
        test_path,
      ),
    )
  ) {
  | Lexer.SyntaxError(msg) =>
    CCResult.Error((
      Bridge.Base_lexer_error,
      Error.parser_error(
        ~msg,
        ~content,
        ~start=lexbuf.lex_start_p,
        ~end_=lexbuf.lex_curr_p,
      ),
    ))
  | Parser_incr.Parsing_error(_)
  | Parser.Error =>
    CCResult.Error((
      Bridge.Base_parser_error,
      Error.parser_error(
        ~msg=?None,
        ~content,
        ~start=lexbuf.lex_start_p,
        ~end_=lexbuf.lex_curr_p,
      ),
    ))

  | Parser_unexpected(msg) => CCResult.Error((Bridge.Parser_unexpected, msg))
  | Parser_error(msg) => CCResult.Error((Bridge.Parser_error, msg))
  | Parser_parameter_error(msg) =>
    CCResult.Error((Bridge.Parser_parameter_error, msg))
  | Parser_unsupported(msg, pos) =>
    CCResult.Error((
      Bridge.Parser_unsupported(pos),
      Error.parser_error_with_info(~msg, ~content, pos),
    ))
  | Optimizer_error(msg) => CCResult.Error((Bridge.Optimizer_error, msg))
  | other_exn => CCResult.Error((Js_unknown, Printexc.to_string(other_exn)))
  };
};

let print__reason = (content, config) => {
  switch (print__structure(content, config)) {
  | Error(e) => Bridge.Error(e)
  | Ok(str) =>
    try(
      {
        Reason_toolchain.RE.print_implementation_with_comments(
          Format.str_formatter,
          (str, []),
        );
        Bridge.Ok(Format.flush_str_formatter() |> CCString.trim);
      }
    ) {
    | Syntaxerr.Error(error) =>
      let loc = error |> Syntaxerr.location_of_error;
      let reported_excerpt =
        Format.flush_str_formatter()
        |> CCString.trim
        |> Error.parser_error(
             ~msg=?None,
             ~content=_,
             ~start=loc.loc_start,
             ~end_=loc.loc_end,
           );
      Bridge.Error((Syntax_error, reported_excerpt));
    | Reason_errors.Reason_error(err, loc) =>
      let reported_excerpt =
        Format.flush_str_formatter()
        |> CCString.trim
        |> Error.parser_error(
             ~msg=?None,
             ~content=_,
             ~start=loc.loc_start,
             ~end_=loc.loc_end,
           );

      Reason_errors.report_error(Format.str_formatter, ~loc, err);
      let reported_error = Format.flush_str_formatter() |> CCString.trim;

      Bridge.Error((
        Reason_error,
        Pastel.(
          <Pastel>
            <Pastel> "\n" </Pastel>
            <Pastel bold=true color=Red> {reported_error ++ "\n"} </Pastel>
            <Pastel bold=true color=Red>
              "----------------------------------------------------"
            </Pastel>
            reported_excerpt
          </Pastel>
        ),
      ));
    | other_exn => Bridge.Error((Js_unknown, Printexc.to_string(other_exn)))
    }
  };
};

let print__ocaml = (content, config) => {
  switch (print__structure(content, config)) {
  | Error(e) => Bridge.Error(e)
  | Ok(str) =>
    try(
      {
        Reason_toolchain.ML.print_implementation_with_comments(
          Format.str_formatter,
          (str, []),
        );
        Bridge.Ok(Format.flush_str_formatter() |> CCString.trim);
      }
    ) {
    | Syntaxerr.Error(error) =>
      let loc = error |> Syntaxerr.location_of_error;
      let reported_excerpt =
        Format.flush_str_formatter()
        |> CCString.trim
        |> Error.parser_error(
             ~msg=?None,
             ~content=_,
             ~start=loc.loc_start,
             ~end_=loc.loc_end,
           );
      Bridge.Error((Syntax_error, reported_excerpt));
    | other_exn => Bridge.Error((Js_unknown, Printexc.to_string(other_exn)))
    }
  };
};

let run = (json: jsstr) =>
  Bridge.string_of_parse_result(
    try({
      let parse_request = Bridge.parse_request_of_string(Js.to_string(json));
      switch (parse_request) {
      | {language: Reason, content, config} => print__reason(content, config)
      | {language: Ocaml, content, config} => print__ocaml(content, config)
      };
    }) {
    | other_exn => Bridge.Error((Js_unknown, Printexc.to_string(other_exn)))
    },
  );

Js.export("run", run);

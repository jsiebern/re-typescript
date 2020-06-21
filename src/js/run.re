open Re_typescript_base;
open Re_typescript_config;
open Re_typescript_fs;
open Js_of_ocaml;

type jsstr = Js.t(Js.js_string);

let test_loader: module Loader.T = (module Fs.Loader);
let test_resolver: module Resolver.T =
  (module
   Resolver.Make({
     let config = {Resolver.loader: test_loader, tsconfig: None};
   }));

let print__structure = (~file_path, config) => {
  open Re_typescript_printer.Tree_utils.Exceptions;

  let r_content = ref(None);
  let r_lexbuf = ref(None);
  try(
    CCResult.Ok(
      Re_typescript_printer.structure_from_ts(
        ~ctx=config,
        ~parser=
          content => {
            let lexbuf = Lexing.from_string(content |> CCString.trim);
            r_content := Some(content);
            r_lexbuf := Some(lexbuf);
            Ok(Parser_incr.parse(lexbuf));
          },
        ~resolver=test_resolver,
        file_path,
      ),
    )
  ) {
  | Lexer.SyntaxError(msg) =>
    let content = r_content^ |> CCOpt.get_exn;
    let lexbuf = r_lexbuf^ |> CCOpt.get_exn;
    CCResult.Error((
      Bridge.Base_lexer_error,
      Error.parser_error(
        ~msg,
        ~content,
        ~start=lexbuf.lex_start_p,
        ~end_=lexbuf.lex_curr_p,
      ),
    ));
  | Parser_incr.Parsing_error(_)
  | Parser.Error =>
    let content = r_content^ |> CCOpt.get_exn;
    let lexbuf = r_lexbuf^ |> CCOpt.get_exn;
    CCResult.Error((
      Bridge.Base_parser_error,
      Error.parser_error(
        ~msg=?None,
        ~content,
        ~start=lexbuf.lex_start_p,
        ~end_=lexbuf.lex_curr_p,
      ),
    ));

  | Parser_unexpected(msg) => CCResult.Error((Bridge.Parser_unexpected, msg))
  | Parser_error(msg) => CCResult.Error((Bridge.Parser_error, msg))
  | Parser_parameter_error(msg) =>
    CCResult.Error((Bridge.Parser_parameter_error, msg))
  | Parser_unsupported(msg, pos) =>
    let content = r_content^ |> CCOpt.get_exn;
    CCResult.Error((
      Bridge.Parser_unsupported(pos),
      Error.parser_error_with_info(~msg, ~content, pos),
    ));
  | Optimizer_error(msg) => CCResult.Error((Bridge.Optimizer_error, msg))
  | other_exn => CCResult.Error((Js_unknown, Printexc.to_string(other_exn)))
  };
};

let print__reason = (~file_path, config) => {
  switch (print__structure(~file_path, config)) {
  | Error(e) => Bridge.Error(e)
  | Ok(str) =>
    try(
      {
        Reason_toolchain.RE.print_implementation_with_comments(
          Format.str_formatter,
          (str, []),
        );
        Bridge.Ok((Format.flush_str_formatter() |> CCString.trim, None));
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

let print__ocaml = (~file_path, config) => {
  switch (print__structure(~file_path, config)) {
  | Error(e) => Bridge.Error(e)
  | Ok(str) =>
    try(
      {
        Reason_toolchain.ML.print_implementation_with_comments(
          Format.str_formatter,
          (str, []),
        );
        Bridge.Ok((Format.flush_str_formatter() |> CCString.trim, None));
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
      switch (parse_request.content) {
      | Some(content) =>
        Sys_js.update_file(~name=parse_request.file_path, ~content)
      | None => ()
      };
      let file_path = Fp.absolute(parse_request.file_path);
      switch (
        parse_request.content,
        switch (parse_request, file_path) {
        | ({language: Reason, content, config, _}, Some(file_path)) =>
          print__reason(~file_path, config)
        | ({language: Ocaml, content, config, _}, Some(file_path)) =>
          print__ocaml(~file_path, config)
        | _ =>
          Bridge.Error((
            Js_unknown,
            Printf.sprintf(
              "Unknown error. Path \"%s\" probably not found.",
              parse_request.file_path,
            ),
          ))
        },
      ) {
      | (Some(_), r) => r
      | (None, Error(_) as e) => e
      | (None, Ok((code, _))) =>
        Ok((
          code,
          Some(Sys_js.read_file(parse_request.file_path) |> CCString.trim),
        ))
      };
    }) {
    | other_exn => Bridge.Error((Js_unknown, Printexc.to_string(other_exn)))
    },
  );

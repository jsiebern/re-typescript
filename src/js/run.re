open Re_typescript_base;
open Re_typescript_config;
open Re_typescript_fs;
open Js_of_ocaml;

type jsstr = Js.t(Js.js_string);

let print__structure = (~file_path, config) => {
  let content = Fs.Loader.file_read(file_path);

  switch (content) {
  | Error(e) => CCResult.Error((Bridge.Js_unknown, e))
  | Ok(content) =>
    let files = [(file_path |> Fp.toString, content)];

    CCResult.Ok(
      Re_typescript_ts_parser.structure_from_ts(~ctx=config, files),
    );
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
        Bridge.Ok((Format.flush_str_formatter() |> CCString.trim, ""));
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
        Bridge.Ok((Format.flush_str_formatter() |> CCString.trim, ""));
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
      Sys_js.update_file(
        ~name=parse_request.file_path,
        ~content=parse_request.content,
      );
      let file_path = Fp.absolute(parse_request.file_path);
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
      };
    }) {
    | other_exn => Bridge.Error((Js_unknown, Printexc.to_string(other_exn)))
    },
  );

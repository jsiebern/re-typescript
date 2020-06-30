module File = CCIO.File;

let parse = (files: list((string, string))) => {
  let files =
    files
    |> CCList.map(((name, src)) => {
         let path = Filename.temp_file(name, "re_typescript");
         CCIO.File.write(path, src) |> ignore;
         (name, path);
       });

  let append =
    files
    |> CCList.to_string(~sep=" ", ((name, path)) => name ++ ":" ++ path);
  CCUnix.with_process_in(
    "node /Users/jonathansiebern/git/Open_Source/re-typescript/lib/js/src_ts/bs/cmd.bs.js "
    ++ append,
    ~f=channel => {
      let jsonString = CCIO.read_all(channel);
      // Delete old source files
      files
      |> CCList.map(snd)
      |> CCList.filter(CCIO.File.exists)
      |> CCList.iter(CCIO.File.remove_noerr);
      // Print json to new one for debugging
      let root_path =
        Sys.getenv("PROJECT_ROOT") |> CCString.split_on_char(':');
      switch (root_path) {
      | [root_path, ..._] =>
        let parsed_path = Filename.concat(root_path, "_json");
        let file_name = "parsed.json";

        // Console.log("before is");
        if (!CCIO.File.exists(parsed_path)) {
          Unix.mkdir(parsed_path, 0o775);
        };

        let out_path = Filename.concat(parsed_path, file_name);
        if (File.exists(out_path)) {
          File.remove_noerr(out_path);
        };
        switch (
          CCIO.File.write(
            out_path,
            jsonString
            |> Yojson.Basic.from_string
            |> Yojson.Basic.pretty_to_string,
          )
        ) {
        | Error(e) => Console.log(e)
        | Ok(_) => ()
        };
      | _ => ()
      };

      jsonString;
    },
  );
};

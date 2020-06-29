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
      files
      |> CCList.map(snd)
      |> CCList.filter(CCIO.File.exists)
      |> CCList.iter(CCIO.File.remove_noerr);
      jsonString;
    },
  );
};

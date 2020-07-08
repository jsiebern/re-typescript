Logs.set_level(Some(Logs.App));
let srcServer =
  Logs.Src.create(~doc="Node process that handles parsing", "re-ts.server");
module LogsServer = (val Logs.src_log(srcServer): Logs.LOG);

let serverLogs: ref(list(string)) = ref([]);
let server_reporter = () => {
  let report = (src, level, ~over, k, msgf) =>
    if (Logs.Src.equal(src, srcServer)) {
      let reporter = Logs_fmt.reporter(~app=Format.str_formatter, ());
      let res = reporter.Logs.report(src, level, ~over=() => (), k, msgf);
      serverLogs := serverLogs^ @ [Format.flush_str_formatter()];
      res;
    } else {
      let reporter = Logs_fmt.reporter();
      reporter.Logs.report(src, level, ~over=() => (), k, msgf);
    };

  {Logs.report: report};
};
Logs.set_reporter(server_reporter());

open Lwt.Infix;

exception RunTests;
let (testsDone, testsResolver): (Lwt.t(unit), Lwt.u(unit)) = Lwt.wait();
let runTests = () => {
  Logs.debug(m => m("Forking process"));

  let pid = Lwt_unix.fork();
  if (pid === 0) {
    Logs.debug(m => m("Running tests"));
    // This is the child
    Lwt.fail(RunTests);
  } else {
    Lwt.finalize(
      () => {
        Logs.debug(m => m("Waiting for tests process"));
        Lwt_unix.waitpid([], pid);
      },
      () => {
        Logs.debug(m => m("Tests process finished"));
        Logs.debug(m => m("Waking up testsDone"));
        Lwt.wakeup(testsResolver, ()) |> Lwt.return;
      },
    );
  };
};

exception IsDone;
exception ServerError(Lwt_stream.t(string));
let server = () => {
  Logs.debug(m => m("Executing: node src_ws/service.bs.js 83"));

  Lwt_process.open_process_full(
    Lwt_process.shell("$(fnm exec -- which node) src_ws/service.bs.js 83"),
  )
  |> Lwt.return
  >>= (
    process => {
      let read_server = process => {
        let errors = Lwt_io.read_lines(process#stderr);
        Lwt.pick([
          Lwt_unix.sleep(0.5) >>= (_ => Lwt.return_none),
          errors |> Lwt_stream.peek,
        ])
        >>= (
          e =>
            switch (e) {
            | Some(_) => Lwt.fail(ServerError(errors))
            | None =>
              let lines = Lwt_io.read_lines(process#stdout);
              Lwt.pick([
                Lwt_unix.sleep(1.0) >>= (_ => Lwt.return_none),
                lines |> Lwt_stream.peek,
              ])
              >|= (
                ln => {
                  switch (ln) {
                  | None => []
                  | Some(_) => Lwt_stream.get_available(lines)
                  };
                }
              );
            }
        );
      };
      let rec read_forever = (process, _) =>
        read_server(process)
        >>= (
          str =>
            {
              switch (str) {
              | [str] when CCString.find(~sub="Server: Running", str) > (-1) =>
                LogsServer.debug(m => m("%s", str));
                runTests() |> Lwt.ignore_result;
              | [] => Logs.debug(m => m("No fresh input from re-ts server"))
              | lines =>
                lines
                |> CCList.iter(str => LogsServer.debug(m => m("%s", str)))
              };
              if (!Lwt.is_sleeping(testsDone)) {
                Logs.debug(m =>
                  m("Tests finished, terminating re-ts server")
                );
                process#terminate;
                Lwt_unix.waitpid([], process#pid)
                >>= (
                  ((pid, status)) => {
                    Logs.debug(m =>
                      m("re-ts server process terminated (pid: %i)", pid)
                    );
                    switch (status) {
                    | Unix.WEXITED(int) =>
                      Logs.debug(m => m("Process status: WEXITED (%i)", int))
                    | WSIGNALED(int) =>
                      Logs.debug(m =>
                        m("Process status: WSIGNALED (%i)", int)
                      )
                    | WSTOPPED(int) =>
                      Logs.debug(m => m("Process status: WSTOPPED (%i)", int))
                    };
                    Lwt.fail(IsDone);
                  }
                );
              } else {
                Logs.debug(m =>
                  m("Nothing to do, trying to read from re-ts server...")
                );
                Lwt.return_unit;
              };
            }
            >>= read_forever(process)
        );
      read_forever(process, ());
    }
  );
};

Lwt.catch(
  () => Lwt_main.run(server()),
  exn =>
    switch (exn) {
    | RunTests =>
      Re_typescript_test_lib.TestFramework.cli();
      if (Re_typescript_test_lib.TestFramework.connection.is_connected) {
        Re_typescript_test_lib.TestFramework.connection.close()
        |> Lwt.ignore_result;
      };
      exit(0);
    | IsDone => Logs.info(m => m("Process finished")) |> Lwt.return
    | ServerError(errorLines) =>
      errorLines
      |> Lwt_stream.to_list
      >>= (
        errorLines =>
          Logs.err(m =>
            m(
              "re-ts Server Error:\n\n%s",
              errorLines |> CCList.to_string(~sep="\n", a => a),
            )
          )
          |> Lwt.return
      )
    | other =>
      Logs.err(m => m("Unexpected error: %s", Printexc.to_string(other)))
      |> Lwt.return
    },
);

Console.log("\n\n");
Console.log(
  serverLogs^
  |> CCList.filter(v => v != "")
  |> CCList.to_string(~sep="", a => a),
);

exit(Logs.err_count() === 0 ? 0 : 1);

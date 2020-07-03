open Async;

let () =
  Command.async_spec(
    ~summary="Start an echo server",
    Command.Spec.(
      empty
      +> flag(
           "-uppercase",
           no_arg,
           ~doc=" Convert to uppercase before echoing back",
         )
      +> flag(
           "-port",
           optional_with_default(8765, int),
           ~doc=" Port to listen on (default 8765)",
         )
    ),
    (uppercase, port, ()) => {
    Test.client(None, None, Uri.of_string("ws://127.0.0.1:81"))
    >>= (
      x => {
        Console.log("con");
        Deferred.never();
      }
    )
  })
  |> Command.run;

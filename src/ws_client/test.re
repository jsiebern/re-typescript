open Core;
open Async;
open Websocket_async;

/* let () =
   Logs.set_reporter (Logs_async_reporter.reporter ()) */

let client = (protocol, extensions, uri) => {
  let host = Option.value_exn(~message="no host in uri", Uri.(host(uri)));
  let port =
    switch (Uri.port(uri), Uri_services.tcp_port_of_uri(uri)) {
    | (Some(p), _) => p
    | (None, Some(p)) => p
    | _ => invalid_arg("port cannot be inferred from URL")
    };
  let scheme =
    Option.value_exn(~message="no scheme in uri", Uri.(scheme(uri)));
  let tcp_fun = ((r, w)) => {
    module C = Cohttp;
    let extra_headers = C.Header.init();
    let extra_headers =
      Option.value_map(protocol, ~default=extra_headers, ~f=proto =>
        C.Header.add(extra_headers, "Sec-Websocket-Protocol", proto)
      );

    let extra_headers =
      Option.value_map(extensions, ~default=extra_headers, ~f=exts =>
        C.Header.add(extra_headers, "Sec-Websocket-Extensions", exts)
      );

    let (r, w) =
      client_ez(
        ~extra_headers,
        ~heartbeat=Time_ns.Span.(of_int_sec(5)),
        uri,
        r,
        w,
      );

    Deferred.all_unit([
      Pipe.transfer(Reader.(pipe @@ Lazy.force(stdin)), w, ~f=s =>
        String.chop_suffix_exn(s, ~suffix="\n")
      ),
      Pipe.transfer(r, Writer.(pipe @@ Lazy.force(stderr)), ~f=s =>
        s ++ "\n"
      ),
    ]);
  };

  Unix.Addr_info.get(~service=string_of_int(port), ~host, [])
  >>= (
    fun
    | [] => failwithf("DNS resolution failed for %s", host, ())
    | [{ai_addr, _}, ..._] => {
        let addr =
          switch (scheme, ai_addr) {
          | (_, ADDR_UNIX(path)) => `Unix_domain_socket(path)
          | ("https", [@implicit_arity] ADDR_INET(h, p))
          | ("wss", [@implicit_arity] ADDR_INET(h, p)) =>
            let h = Ipaddr_unix.of_inet_addr(h);
            `OpenSSL((h, p, Conduit_async.V2.Ssl.Config.create()));
          | (_, [@implicit_arity] ADDR_INET(h, p)) =>
            let h = Ipaddr_unix.of_inet_addr(h);
            `TCP((h, p));
          };

        Conduit_async.V2.connect(addr) >>= tcp_fun;
      }
  );
};

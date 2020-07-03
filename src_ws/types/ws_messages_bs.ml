(* Auto-generated from "ws_messages.atd" *)
              [@@@ocaml.warning "-27-32-35-39"]

type file_id = Ws_messages_t.file_id

type client_id = Ws_messages_t.client_id

type server_msg = Ws_messages_t.server_msg

type client_msg = Ws_messages_t.client_msg

let write_file_id = (
  Atdgen_codec_runtime.Encode.string
)
let read_file_id = (
  Atdgen_codec_runtime.Decode.string
)
let write_client_id = (
  Atdgen_codec_runtime.Encode.string
)
let read_client_id = (
  Atdgen_codec_runtime.Decode.string
)
let write_server_msg = (
  Atdgen_codec_runtime.Encode.make (fun (x : _) -> match x with
    | `Initialized x ->
    Atdgen_codec_runtime.Encode.constr1 "Initialized" (
      write_client_id
    ) x
    | `FileCreated x ->
    Atdgen_codec_runtime.Encode.constr1 "FileCreated" (
      write_file_id
    ) x
    | `FileContentsOk x ->
    Atdgen_codec_runtime.Encode.constr1 "FileContentsOk" (
      write_file_id
    ) x
  )
)
let read_server_msg = (
  Atdgen_codec_runtime.Decode.enum
  [
      (
      "Initialized"
      ,
        `Decode (
        read_client_id
        |> Atdgen_codec_runtime.Decode.map (fun x -> ((`Initialized x) : _))
        )
      )
    ;
      (
      "FileCreated"
      ,
        `Decode (
        read_file_id
        |> Atdgen_codec_runtime.Decode.map (fun x -> ((`FileCreated x) : _))
        )
      )
    ;
      (
      "FileContentsOk"
      ,
        `Decode (
        read_file_id
        |> Atdgen_codec_runtime.Decode.map (fun x -> ((`FileContentsOk x) : _))
        )
      )
  ]
)
let write_client_msg = (
  Atdgen_codec_runtime.Encode.make (fun (x : _) -> match x with
    | `Initialize ->
    Atdgen_codec_runtime.Encode.constr0 "Initialize"
    | `CreateFile x ->
    Atdgen_codec_runtime.Encode.constr1 "CreateFile" (
      Atdgen_codec_runtime.Encode.tuple2
        (
          write_client_id
        )
        (
          Atdgen_codec_runtime.Encode.string
        )
    ) x
    | `SetFileContents x ->
    Atdgen_codec_runtime.Encode.constr1 "SetFileContents" (
      Atdgen_codec_runtime.Encode.tuple3
        (
          write_client_id
        )
        (
          write_file_id
        )
        (
          Atdgen_codec_runtime.Encode.string
        )
    ) x
  )
)
let read_client_msg = (
  Atdgen_codec_runtime.Decode.enum
  [
      (
      "Initialize"
      ,
        `Single (`Initialize)
      )
    ;
      (
      "CreateFile"
      ,
        `Decode (
        Atdgen_codec_runtime.Decode.tuple2
          (
            read_client_id
          )
          (
            Atdgen_codec_runtime.Decode.string
          )
        |> Atdgen_codec_runtime.Decode.map (fun x -> ((`CreateFile x) : _))
        )
      )
    ;
      (
      "SetFileContents"
      ,
        `Decode (
        Atdgen_codec_runtime.Decode.tuple3
          (
            read_client_id
          )
          (
            read_file_id
          )
          (
            Atdgen_codec_runtime.Decode.string
          )
        |> Atdgen_codec_runtime.Decode.map (fun x -> ((`SetFileContents x) : _))
        )
      )
  ]
)

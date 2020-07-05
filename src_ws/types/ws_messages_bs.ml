(* Auto-generated from "ws_messages.atd" *)
              [@@@ocaml.warning "-27-32-35-39"]

type file_path = Ws_messages_t.file_path

type client_id = Ws_messages_t.client_id

type server_msg = Ws_messages_t.server_msg

type client_msg = Ws_messages_t.client_msg

let write_file_path = (
  Atdgen_codec_runtime.Encode.string
)
let read_file_path = (
  Atdgen_codec_runtime.Decode.string
)
let write_client_id = (
  Atdgen_codec_runtime.Encode.string
)
let read_client_id = (
  Atdgen_codec_runtime.Decode.string
)
let write__1 = (
  Atdgen_codec_runtime.Encode.list (
    Atdgen_codec_runtime.Encode.tuple2
      (
        write_file_path
      )
      (
        Atdgen_codec_runtime.Encode.string
      )
  )
)
let read__1 = (
  Atdgen_codec_runtime.Decode.list (
    Atdgen_codec_runtime.Decode.tuple2
      (
        read_file_path
      )
      (
        Atdgen_codec_runtime.Decode.string
      )
  )
)
let write_server_msg = (
  Atdgen_codec_runtime.Encode.make (fun (x : _) -> match x with
    | `Initialized x ->
    Atdgen_codec_runtime.Encode.constr1 "Initialized" (
      write_client_id
    ) x
    | `FileCreated x ->
    Atdgen_codec_runtime.Encode.constr1 "FileCreated" (
      write_file_path
    ) x
    | `FileContentsOk x ->
    Atdgen_codec_runtime.Encode.constr1 "FileContentsOk" (
      write_file_path
    ) x
    | `FileUpdated x ->
    Atdgen_codec_runtime.Encode.constr1 "FileUpdated" (
      write_file_path
    ) x
    | `FileDeleted x ->
    Atdgen_codec_runtime.Encode.constr1 "FileDeleted" (
      write_file_path
    ) x
    | `ParseOk x ->
    Atdgen_codec_runtime.Encode.constr1 "ParseOk" (
      Atdgen_codec_runtime.Encode.tuple1
        (
          write__1
        )
    ) x
    | `ParseError x ->
    Atdgen_codec_runtime.Encode.constr1 "ParseError" (
      Atdgen_codec_runtime.Encode.string
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
        read_file_path
        |> Atdgen_codec_runtime.Decode.map (fun x -> ((`FileCreated x) : _))
        )
      )
    ;
      (
      "FileContentsOk"
      ,
        `Decode (
        read_file_path
        |> Atdgen_codec_runtime.Decode.map (fun x -> ((`FileContentsOk x) : _))
        )
      )
    ;
      (
      "FileUpdated"
      ,
        `Decode (
        read_file_path
        |> Atdgen_codec_runtime.Decode.map (fun x -> ((`FileUpdated x) : _))
        )
      )
    ;
      (
      "FileDeleted"
      ,
        `Decode (
        read_file_path
        |> Atdgen_codec_runtime.Decode.map (fun x -> ((`FileDeleted x) : _))
        )
      )
    ;
      (
      "ParseOk"
      ,
        `Decode (
        Atdgen_codec_runtime.Decode.tuple1
          (
            read__1
          )
        |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ParseOk x) : _))
        )
      )
    ;
      (
      "ParseError"
      ,
        `Decode (
        Atdgen_codec_runtime.Decode.string
        |> Atdgen_codec_runtime.Decode.map (fun x -> ((`ParseError x) : _))
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
          write_file_path
        )
    ) x
    | `SetFileContents x ->
    Atdgen_codec_runtime.Encode.constr1 "SetFileContents" (
      Atdgen_codec_runtime.Encode.tuple3
        (
          write_client_id
        )
        (
          write_file_path
        )
        (
          Atdgen_codec_runtime.Encode.string
        )
    ) x
    | `CreateFileWithConent x ->
    Atdgen_codec_runtime.Encode.constr1 "CreateFileWithConent" (
      Atdgen_codec_runtime.Encode.tuple3
        (
          write_client_id
        )
        (
          write_file_path
        )
        (
          Atdgen_codec_runtime.Encode.string
        )
    ) x
    | `UpdateFile x ->
    Atdgen_codec_runtime.Encode.constr1 "UpdateFile" (
      Atdgen_codec_runtime.Encode.tuple3
        (
          write_client_id
        )
        (
          write_file_path
        )
        (
          Atdgen_codec_runtime.Encode.string
        )
    ) x
    | `DeleteFile x ->
    Atdgen_codec_runtime.Encode.constr1 "DeleteFile" (
      Atdgen_codec_runtime.Encode.tuple2
        (
          write_client_id
        )
        (
          write_file_path
        )
    ) x
    | `Parse x ->
    Atdgen_codec_runtime.Encode.constr1 "Parse" (
      write_client_id
    ) x
    | `Destroy x ->
    Atdgen_codec_runtime.Encode.constr1 "Destroy" (
      write_client_id
    ) x
    | `QuickParse x ->
    Atdgen_codec_runtime.Encode.constr1 "QuickParse" (
      Atdgen_codec_runtime.Encode.tuple2
        (
          write_file_path
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
            read_file_path
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
            read_file_path
          )
          (
            Atdgen_codec_runtime.Decode.string
          )
        |> Atdgen_codec_runtime.Decode.map (fun x -> ((`SetFileContents x) : _))
        )
      )
    ;
      (
      "CreateFileWithConent"
      ,
        `Decode (
        Atdgen_codec_runtime.Decode.tuple3
          (
            read_client_id
          )
          (
            read_file_path
          )
          (
            Atdgen_codec_runtime.Decode.string
          )
        |> Atdgen_codec_runtime.Decode.map (fun x -> ((`CreateFileWithConent x) : _))
        )
      )
    ;
      (
      "UpdateFile"
      ,
        `Decode (
        Atdgen_codec_runtime.Decode.tuple3
          (
            read_client_id
          )
          (
            read_file_path
          )
          (
            Atdgen_codec_runtime.Decode.string
          )
        |> Atdgen_codec_runtime.Decode.map (fun x -> ((`UpdateFile x) : _))
        )
      )
    ;
      (
      "DeleteFile"
      ,
        `Decode (
        Atdgen_codec_runtime.Decode.tuple2
          (
            read_client_id
          )
          (
            read_file_path
          )
        |> Atdgen_codec_runtime.Decode.map (fun x -> ((`DeleteFile x) : _))
        )
      )
    ;
      (
      "Parse"
      ,
        `Decode (
        read_client_id
        |> Atdgen_codec_runtime.Decode.map (fun x -> ((`Parse x) : _))
        )
      )
    ;
      (
      "Destroy"
      ,
        `Decode (
        read_client_id
        |> Atdgen_codec_runtime.Decode.map (fun x -> ((`Destroy x) : _))
        )
      )
    ;
      (
      "QuickParse"
      ,
        `Decode (
        Atdgen_codec_runtime.Decode.tuple2
          (
            read_file_path
          )
          (
            Atdgen_codec_runtime.Decode.string
          )
        |> Atdgen_codec_runtime.Decode.map (fun x -> ((`QuickParse x) : _))
        )
      )
  ]
)

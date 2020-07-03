(* Auto-generated from "ws_messages.atd" *)
              [@@@ocaml.warning "-27-32-35-39"]

type file_id = string

type client_id = string

type server_msg = [
    `Initialized of client_id
  | `FileCreated of file_id
  | `FileContentsOk of file_id
]

type client_msg = [
    `Initialize
  | `CreateFile of (client_id * string)
  | `SetFileContents of (client_id * file_id * string)
]

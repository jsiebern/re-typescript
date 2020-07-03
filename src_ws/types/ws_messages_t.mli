(* Auto-generated from "ws_messages.atd" *)
              [@@@ocaml.warning "-27-32-35-39"]

type file_path = string

type client_id = string

type server_msg = [
    `Initialized of client_id
  | `FileCreated of (file_path)
  | `FileContentsOk of file_path
  | `ParseOk of ((file_path * string) list)
]

type client_msg = [
    `Initialize
  | `CreateFile of (client_id * file_path)
  | `SetFileContents of (client_id * file_path * string)
  | `Parse of client_id
  | `Destroy of client_id
]

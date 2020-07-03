(* Auto-generated from "ws_messages.atd" *)
              [@@@ocaml.warning "-27-32-35-39"]

type file_id = Ws_messages_t.file_id

type client_id = Ws_messages_t.client_id

type server_msg = Ws_messages_t.server_msg

type client_msg = Ws_messages_t.client_msg

val read_file_id :  file_id Atdgen_codec_runtime.Decode.t

val write_file_id :  file_id Atdgen_codec_runtime.Encode.t

val read_client_id :  client_id Atdgen_codec_runtime.Decode.t

val write_client_id :  client_id Atdgen_codec_runtime.Encode.t

val read_server_msg :  server_msg Atdgen_codec_runtime.Decode.t

val write_server_msg :  server_msg Atdgen_codec_runtime.Encode.t

val read_client_msg :  client_msg Atdgen_codec_runtime.Decode.t

val write_client_msg :  client_msg Atdgen_codec_runtime.Encode.t


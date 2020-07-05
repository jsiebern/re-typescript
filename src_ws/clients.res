open Ts_morph
module StringSet = Belt.Set.String

type t = {
  id: Ws_messages_t.client_id,
  project: Project.t,
  files: StringSet.t,
}

let map = Hashtbl.create(0)

let create = (~client_id) => {
  Hashtbl.add(
    map,
    client_id,
    {
      id: client_id,
      project: Project.make(
        Some(
          Project.Config.make(
            ~useInMemoryFileSystem=true,
            ~compilerOptions={
              lib: ["lib", "lib.es2015.collection.d.ts"],
              noLib: false,
              importHelpers: true,
            },
            (),
          ),
        ),
      ),
      files: StringSet.empty,
    },
  )
  client_id
}

let createFile = (~client_id, file_path) => {
  let client = Hashtbl.find(map, client_id)
  Hashtbl.replace(map, client_id, {...client, files: client.files->StringSet.add(file_path)})
  file_path
}

let deleteFile = (~client_id, file_path) => {
  let client = Hashtbl.find(map, client_id)
  Hashtbl.replace(map, client_id, {...client, files: client.files->StringSet.remove(file_path)})
  switch client.project->Project.getSourceFile(file_path) {
  | None => ()
  | Some(sourceFile) => client.project->Project.removeSourceFile(sourceFile)
  }
  file_path
}

let setFileContents = (~client_id, ~file_path, contents) => {
  let client = Hashtbl.find(map, client_id)

  let sourceFile = switch client.project->Project.getSourceFile(file_path) {
  | None => client.project->Project.createSourceFile(file_path, contents)
  | Some(sourceFile) =>
    client.project->Project.removeSourceFile(sourceFile)
    client.project->Project.createSourceFile(file_path, contents)
  }
  sourceFile->SourceFile.saveSync
  file_path
}

let parse = (~client_id) => {
  let client = Hashtbl.find(map, client_id)

  Preparse.preParse(~project=client.project)

  let diagnostics =
    client.project
    ->Project.getPreEmitDiagnostics
    ->Project.formatDiagnosticsWithColorAndContext(client.project, _)
    ->Js.String.trim

  let file_list =
    client.files
    ->Belt.Set.String.toArray
    ->Belt.Array.keepMap(file_path =>
      client.project
      ->Project.getSourceFile(file_path)
      ->Belt.Option.map(sourceFile => (file_path, sourceFile))
    )
    ->Belt.Array.map(((file_path, sourceFile)) => {
      let node = sourceFile->SourceFile.compilerNodeJson
      let parsed = Typescript_bs.read_node(node)
      (file_path, parsed->Typescript_bs.write_node->Js.Json.stringify)
    })
    ->Belt.List.fromArray
  diagnostics == "" ? Ok(file_list) : Error(diagnostics)
}

let destroy = (~client_id) => {
  Hashtbl.remove(map, client_id)
}

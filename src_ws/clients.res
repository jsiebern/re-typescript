type t = {
  id: Ws_messages_t.client_id,
  project: Ts_morph.Project.t,
  files: array<(Ws_messages_t.file_id, string)>,
}

let map = Hashtbl.create(0)

let create = () => {
  open Ts_morph

  let newId = Shortid.generate()
  Hashtbl.add(
    map,
    newId,
    {
      id: newId,
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
      files: [],
    },
  )
  newId
}

let createFile = (~client_id, filePath) => {
  let fileId = Shortid.generate()
  let client = Hashtbl.find(map, client_id)
  Hashtbl.replace(
    map,
    client_id,
    {
      ...client,
      files: client.files->Belt.Array.concat([(fileId, filePath)]),
    },
  )
  fileId
}

let setFileContents = (~client_id, ~file_id, contents) => {
  open Ts_morph

  let client = Hashtbl.find(map, client_id)
  let filePath =
    client.files
    ->Belt.Array.keep(((f_id, _)) => f_id === file_id)
    ->Belt.Array.map(snd)
    ->Belt.Array.getExn(0)

  let sourceFile = client.project->Project.createSourceFile(filePath, contents)
  sourceFile->SourceFile.saveSync
  file_id
}

let parse = (~client_id, ~file_id as _) => {
  open Ts_morph
  let client = Hashtbl.find(map, client_id)

  Preparse.preParse(~project=client.project)

  client.project->Project.getSourceFiles->Belt.Array.forEach(sourceFile => {
    let node = sourceFile->SourceFile.compilerNodeJson
    let parsed = Typescript_bs.read_node(node)

    Js.log(parsed->Typescript_bs.write_node->Js.Json.stringify)
  })
}

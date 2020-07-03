type t = {
  id: Ws_messages_t.client_id,
  project: Ts_morph.Project.t,
  files: array<(Ws_messages_t.file_path)>,
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
  let client = Hashtbl.find(map, client_id)
  Hashtbl.replace(
    map,
    client_id,
    {
      ...client,
      files: client.files->Belt.Array.concat([(filePath)]),
    },
  )
  (filePath)
}

let setFileContents = (~client_id, ~file_path, contents) => {
  open Ts_morph

  let client = Hashtbl.find(map, client_id)
  let filePath =
    client.files
    ->Belt.Array.keep(((f_path)) => f_path === file_path)
    ->Belt.Array.getExn(0)

  let sourceFile = client.project->Project.createSourceFile(filePath, contents)
  sourceFile->SourceFile.saveSync
  (file_path)
}

let parse = (~client_id) => {
  open Ts_morph
  let client = Hashtbl.find(map, client_id)

  Preparse.preParse(~project=client.project)

  client.files->Belt.Array.map(((path)) => {
    let sourceFile = client.project->Project.getSourceFile(path);
    let node = sourceFile->SourceFile.compilerNodeJson;
    let parsed = Typescript_bs.read_node(node);
    (path, parsed->Typescript_bs.write_node->Js.Json.stringify);
  })->Belt.List.fromArray;
}

let destroy = (~client_id) => {
  Hashtbl.remove(map, client_id);
}
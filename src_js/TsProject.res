let project = Recoil.atom({
  key: "ts-project",
  default: None,
})

let examples: Recoil.readOnly<
  array<(Bridge_bs.example, array<(string, string, Js.Date.t)>)>,
> = Recoil.asyncSelector({
  key: "project-example-list",
  get: ({get}) => {
    let project = get(project)
    Js.Promise.make((~resolve, ~reject) => {
      WorkerBridge.queryWorker(ExampleList, (. result) => {
        switch result {
        | Res_ExampleList(lst) =>
          let examples = lst->Belt.List.toArray
          let examples = examples->Belt.Array.map(example => {
            let fileList = Utils.getFilesInExample(example)
            fileList->Belt.Array.forEach(((path, content)) => {
              project->Project.createSourceFile(path, content)->SourceFile.saveSync
            })
            (example, fileList->Belt.Array.map(((a, b)) => (a, b, Js.Date.make())))
          })
          resolve(. examples)
        | _ => reject(. Failure("Invalid return type for example list"))
        }
      })->ignore
    })
  },
})

type selected = {
  example: option<string>,
  file: option<string>,
  action: Js.Date.t,
}
let selected = Recoil.atom({
  key: "pr-selected",
  default: {
    example: None,
    file: None,
    action: Js.Date.make(),
  },
})

let selectedFile = Recoil.selectorWithWrite({
  key: "pr-selected-file",
  get: ({get}) => get(selected).file,
  set: ({get, set}, value) =>
    if get(selected).file != value {
      set(selected, {...get(selected), action: Js.Date.make(), file: value})
    },
})
let selectedExample = Recoil.selectorWithWrite({
  key: "pr-selected-example",
  get: ({get}) => get(selected).example,
  set: ({get, set}, value) => {
    set(selected, {...get(selected), action: Js.Date.make(), example: value})
  },
})
let selectedExampleResolved = Recoil.selector({
  key: "pr-selected-example-resolved",
  get: ({get}) => {
    let examples = get(examples)
    let selected = get(selectedExample)
    examples
    ->Belt.Array.keepMap(((e, _)) => Some(e.path) == selected ? Some(e) : None)
    ->Belt.Array.get(0)
  },
})
let selectedSource = Recoil.selectorWithWrite({
  key: "pr-selected-source",
  get: ({get}) => {
    let selected = get(selected)
    let filePath = selected.file->Belt.Option.getWithDefault("/")
    let project = get(project)
    let fs = project->Project.getFileSystem
    try Some((selected.action, fs->FileSystem.readFileSync(filePath))) catch {
    | _ => None
    }
  },
  set: ({get, set}, value) => {
    let filePath = get(selectedFile)->Belt.Option.getWithDefault("/")
    let project = get(project)
    let fs = project->Project.getFileSystem
    switch value {
    | Some((action, content)) =>
      fs->FileSystem.writeFileSync(filePath, content)
      set(selected, {...get(selected), action: action})
    | _ => ()
    }
  },
})

let parsedSource: Recoil.readOnly<Belt.Result.t<string, string>> = Recoil.asyncSelector({
  key: "pr-parsed-source",
  get: ({get}) => {
    let language = get(State.language)
    let project = get(project)
    let selected = get(selected)
    let filePath = selected.file
    let config = get(State.config)

    Js.Promise.make((~resolve, ~reject) => {
      let sourceFile = filePath->Belt.Option.flatMap(project->Project.getSourceFile)
      switch sourceFile {
      | None => reject(. Failure("Source file not found"))
      | Some(sourceFile) =>
        sourceFile->SourceFile.refreshFromFileSystemSync

        Preparse.preParse(~sourceFiles=[sourceFile], project)

        // let diagnostics =
        //   project
        //   ->Project.getPreEmitDiagnostics
        //   ->Project.formatDiagnosticsWithColorAndContext(project, _)
        //   ->Js.String.trim;

        let node = sourceFile->SourceFile.compilerNodeJson
        let parsed = Typescript_bs.read_node(node)
        let parsed = (
          sourceFile->SourceFile.getFilePath,
          parsed->Typescript_bs.write_node->Js.Json.stringify,
        )

        // diagnostics == "" ? resolve(. parsed) : reject(. Failure(diagnostics))
        resolve(. parsed)
      }
    }) |> Js.Promise.then_(((file_path, content)) => {
      Js.Promise.make((~resolve, ~reject as _) => {
        WorkerBridge.queryWorker(
          Parse({content: content, language: language, file_path: file_path, config: config}),
          (. result) => {
            switch result {
            | Bridge_bs.Res_Parse(Ok((parsed, _))) => resolve(. Ok(parsed))
            | Bridge_bs.Res_Parse(Error(e)) => resolve(. Error(e->snd))
            | _ => resolve(. Error("Invalid return type for example list"))
            }
          },
        )->ignore
      })
    }) |> Js.Promise.catch(e => Js.Promise.resolve(Belt.Result.Error((e->Obj.magic)["message"])))
  },
})

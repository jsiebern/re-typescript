module MonacoEditor = {
  @react.component @bs.module("@monaco-editor/react")
  external make: (
    ~height: string=?,
    ~width: string=?,
    ~language:  [ | #javascript | #typescript ],
    ~value: string,
    ~editorDidMount: ('a, 'b) => unit=?,
    ~onChange: ('a, string) => unit,
  ) => React.element = "ControlledEditor"
}

module EditorWindow = {
  let tmr: ref<option<Js.Global.timeoutId>> = ref(None)
  @react.component
  let make = () => {
    let (selectedSource, setSelectedSource) = Recoil.useRecoilState(TsProject.selectedSource);
    let ((updated, value), setValue) = React.useReducer((_, v) => v, (Js.Date.make(), ""))
    React.useEffect2(() => {
      switch (selectedSource) {
        | Some((sourceUpdated, _) as source) when sourceUpdated->Js.Date.valueOf > updated->Js.Date.valueOf =>
         setValue(source)
        | Some(_)
        | None => ()
      }
      None
    }, (selectedSource, updated))

    let tmr = React.useRef(-1->Obj.magic)

    React.useEffect2(() => { 
      tmr.current = Js.Global.setTimeout(() => {
        switch (value, selectedSource) {
        | (value, None) => setSelectedSource(_ => Some((updated, value)))
        | (value, Some(sourceUpdated,_)) when updated->Js.Date.valueOf >= sourceUpdated->Js.Date.valueOf  =>
         setSelectedSource(_ => Some((updated, value)))
        | _ => ()
        }
      }, 600)

      Some(() => Js.Global.clearTimeout(tmr.current))
    }, (updated, value))

    <MonacoEditor
      height="100%"
      width="100%"
      value={value == "" ? " " : value}
      onChange={(_, v) => setValue((Js.Date.make(), v))}
      language=#typescript
    />
  }
}

module Container = %styled.div(
  `
  height: 100%;
  width: 50vw;
  position: relative;
  background-color: #FFFFFF;
  display: flex;
  flex-direction: column;
`
)
module ContainerEditor = %styled.div(`
  flex-grow: 1;
  width: 100%;
  position: relative;
`)
let containerTabs = %css(`
  width: 100%;
  height: 65px;
`)

module ExampleLabel = {
  module Inner = {
    @react.component
    let make = () => {
      let example = Recoil.useRecoilValue(TsProject.selectedExampleResolved)
      switch example {
      | None => React.null
      | Some(example) => example.meta.name->React.string
      }
    }
  }

  @react.component
  let make = () => {
    open SemanticUi

    <Label attached="top right" className=%css(`z-index: 1;`)>
      <Icon name="file code outline" /> <Inner />
    </Label>
  }
}

module FileTabs = {
  module TabMenu = {
    @react.component
    let make = () => {
      let selectedExample = Recoil.useRecoilValue(TsProject.selectedExampleResolved)
      let (selectedFile, setSelectedFile) = Recoil.useRecoilState(TsProject.selectedFile)

      open SemanticUi

      <Menu tabular=true>
        {switch selectedExample {
        | None => React.null
        | Some(example) =>
          let path = Fp.absoluteExn(example.path)
          example.files->Belt.List.map(file =>
            switch file {
            | File((name, _)) =>
              let file_path = path->Fp.append(name)->Fp.toString
              <MenuItem
                key=name
                className="file-tab-menu-item"
                name
                active={selectedFile == Some(file_path)}
                onClick={(_, _) => setSelectedFile(_ => Some(file_path))}>
                {name->React.string}
              </MenuItem>
            | Folder(_) => React.null
            }
          )->Belt.List.toArray->React.array
        }}
      </Menu>
    }
  }

  @react.component
  let make = () => {
    open SemanticUi

    <React.Suspense fallback={<Menu tabular=true> React.null </Menu>}> <TabMenu /> </React.Suspense>
  }
}

@react.component
let make = () => {
  <Container>
    <div className=containerTabs> <FileTabs /> </div>
    <ContainerEditor> <EditorWindow /> </ContainerEditor>
    <React.Suspense fallback=React.null> <ExampleLabel /> </React.Suspense>
  </Container>
}

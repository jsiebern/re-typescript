open Bridge_t

module Async = {
  @react.component
  let make = () => {
    let (visible, setVisible) = Recoil.useRecoilState(State.examples_open)
    let examples = Recoil.useRecoilValue(TsProject.examples)

    let (selectedExample, setSelectedExample) = Recoil.useRecoilState(TsProject.selectedExample)
    let setSelectedFile = Recoil.useSetRecoilState(TsProject.selectedFile)
    React.useEffect1(() => {
      switch examples->Belt.Array.get(0) {
      | None => ()
      | Some((example, _)) =>
        setSelectedExample(_ => Some(example.path))
        setSelectedFile(_ => Some(
          Fp.absoluteExn(example.path)->Fp.append(example.meta.entry)->Fp.toString,
        ))
      }
      None
    }, [examples->Belt.Array.length])

    open SemanticUi

    <Sidebar
      visible animation=#push direction=#left width=#wide onHide={(_, _) => setVisible(_ => false)}>
      <Menu fluid=true vertical=true tabular=true> {examples->Belt.Array.map(((example, _)) =>
          <MenuItem
            key={example.path}
            name={example.path}
            active={selectedExample == Some(example.path)}
            onClick={(_, _) => {
              setSelectedExample(_ => Some(example.path))
              setSelectedFile(_ => Some(
                Fp.absoluteExn(example.path)->Fp.append(example.meta.entry)->Fp.toString,
              ))
            }}>
            <Icon name="file code outline" /> {example.meta.name->React.string}
          </MenuItem>
        )->React.array} </Menu>
    </Sidebar>
  }
}

@react.component
let make = () => {
  <React.Suspense fallback={"Loading..."->React.string}> <Async /> </React.Suspense>
}

module Styles = {
  let container = %css(
    `
    width: 100vw;
    height: 100vh;
    position: relative;
    display: flex;
    flex-direction: column;
  `
  )
  let header = %css(`
    width: 100vw;
    height: 65px;
  `)

  let panelheight = "calc(100vh - 65px)"
  let panelContainer = %css(
    j`
    width: 100vw;
    height: $panelheight;
    display: flex;
    flex-direction: row;
  `
  )
  let panelEditor = %css(j`
    width: 50vw;
    height: $panelheight;
    position: relative;
  `)
  let panelPreview = %css(j`
    width: 50vw;
    height: $panelheight;
    position: relative;
  `)
}

module PreviewLabel = {
  @react.component
  let make = () => {
    let lang = Recoil.useRecoilValue(State.language)

    open SemanticUi

    <Label attached="top right">
      {({
        switch lang {
        | Reason => "ReasonML"
        | Ocaml => "OCaml"
        }
      } ++ " Preview")->React.string}
    </Label>
  }
}

@react.component
let make = () => {
  <div className=Styles.container>
    <div className=Styles.header> <PageMenu /> </div>
    <ConfigurationPanel>
      <div className=Styles.panelContainer>
        <div className=Styles.panelEditor> <TsEditor /> </div>
        <div className=Styles.panelPreview> <PreviewLabel /> <React.Suspense fallback={React.null}> <Preview /> </React.Suspense> </div>
      </div>
    </ConfigurationPanel>
  </div>
}

@react.component
let make = () => {
  let (lang, setLang) = Recoil.useRecoilState(State.language)
  let (configOpen, setConfigOpen) = Recoil.useRecoilState(State.config_open)
  let (examplesOpen, setExamplesOpen) = Recoil.useRecoilState(State.examples_open)

  open SemanticUi

  <Menu stackable=true fluid=true icons=#labeled>
    <MenuItem name="main">
      <SemanticUi.Header
        image={SemanticUi.Header.Image.component(
          <Image
            src=Assets.logo_re_typescript
            size=#small
            href="https://github.com/jsiebern/re-typescript"
          />,
        )}
        content={"re-typescript"->React.string}
      />
    </MenuItem>
    <SemanticUi.Popup
      position="right center"
      content={"See a list of different examples"->React.string}
      trigger={<MenuItem
        name="examples" onClick={(_, _) => setExamplesOpen(o => !o)} active=configOpen>
        {examplesOpen
          ? <Icon color=#red name="close" />
          : <Icon color=#blue name="sticky note outline" />}
        {(examplesOpen ? "Close" : "Examples")->React.string}
      </MenuItem>}
    />
    <MenuMenu position=#right>
      <MenuItem
        name="lang_reason" active={lang === Reason} onClick={(_, _) => setLang(_ => Reason)}>
        <Image className="icon" src=Assets.logo_reason size=#mini spaced=Image.Spaced.right />
        {"ReasonML"->React.string}
      </MenuItem>
      <MenuItem name="lang_ocaml" active={lang === Ocaml} onClick={(_, _) => setLang(_ => Ocaml)}>
        <Image src=Assets.logo_ocaml size=#mini spaced=Image.Spaced.right /> {"OCaml"->React.string}
      </MenuItem>
      <SemanticUi.Popup
        position="left center"
        content={"Adjust code generation settings"->React.string}
        trigger={<MenuItem
          name="config" onClick={(_, _) => setConfigOpen(o => !o)} active=configOpen>
          {configOpen ? <Icon color=#red name="close" /> : <Icon color=#blue name="settings" />}
          {(configOpen ? "Close Config" : "Open Config")->React.string}
        </MenuItem>}
      />
    </MenuMenu>
  </Menu>
}

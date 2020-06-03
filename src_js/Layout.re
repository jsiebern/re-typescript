module Styles = {
  let container = [%css
    {|
    width: 100vw;
    height: 100vh;
    position: relative;
    display: flex;
    flex-direction: column;
  |}
  ];
  let header = [%css {|
    width: 100vw;
    height: 65px;
  |}];

  let panelheight = "calc(100vh - 65px)";
  let panelContainer = [%css
    {j|
    width: 100vw;
    height: $panelheight;
    display: flex;
    flex-direction: row;
  |j}
  ];
  let panelEditor = [%css
    {j|
    width: 50vw;
    height: $panelheight;
    position: relative;
  |j}
  ];
  let panelPreview = [%css
    {j|
    width: 50vw;
    height: $panelheight;
    position: relative;
  |j}
  ];
};

module PageMenu = {
  [@react.component]
  let make = () => {
    let (lang, setLang) = Recoil.useRecoilState(State.language);
    let (configOpen, setConfigOpen) =
      Recoil.useRecoilState(State.config_open);

    SemanticUi.(
      <Menu stackable=true fluid=true icons=`labeled>
        <MenuItem name="main">
          <SemanticUi.Header
            image={SemanticUi.Header.Image.component(
              <Image
                src=Assets.logo_re_typescript
                size=`small
                href="https://github.com/jsiebern/re-typescript"
              />,
            )}
            content={"re-typescript"->React.string}
          />
        </MenuItem>
        <SemanticUi.Popup
          position="right center"
          content={"Adjust code generation settings"->React.string}
          trigger={
            <MenuItem
              name="config"
              onClick={(_, _) => setConfigOpen(o => !o)}
              active=configOpen>
              {configOpen
                 ? <Icon color=`red name="close" />
                 : <Icon color=`blue name="settings" />}
              (configOpen ? "Close Config" : "Open Config")->React.string
            </MenuItem>
          }
        />
        <MenuMenu position=`right>
          <MenuItem
            name="lang_reason"
            active={lang === Reason}
            onClick={(_, _) => setLang(_ => Reason)}>
            <Image
              className="icon"
              src=Assets.logo_reason
              size=`mini
              spaced=Image.Spaced.right
            />
            "ReasonML"->React.string
          </MenuItem>
          <MenuItem
            name="lang_ocaml"
            active={lang === Ocaml}
            onClick={(_, _) => setLang(_ => Ocaml)}>
            <Image
              src=Assets.logo_ocaml
              size=`mini
              spaced=Image.Spaced.right
            />
            "OCaml"->React.string
          </MenuItem>
        </MenuMenu>
      </Menu>
    );
  };
};

module PreviewLabel = {
  [@react.component]
  let make = () => {
    let lang = Recoil.useRecoilValue(State.language);

    SemanticUi.(
      <Label attached="top right">
        {{
           {
             switch (lang) {
             | Reason => "ReasonML"
             | Ocaml => "OCaml"
             };
           }
           ++ " Preview";
         }
         ->React.string}
      </Label>
    );
  };
};

[@react.component]
let make = () => {
  <div className=Styles.container>
    <div className=Styles.header> <PageMenu /> </div>
    <ConfigurationPanel>
      <div className=Styles.panelContainer>
        <div className=Styles.panelEditor> <TsEditor /> </div>
        <div className=Styles.panelPreview> <PreviewLabel /> <Preview /> </div>
      </div>
    </ConfigurationPanel>
  </div>;
};
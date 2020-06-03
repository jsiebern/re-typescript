[@react.component]
let make = () => {
  let (visible, setVisible) = Recoil.useRecoilState(State.examples_open);

  SemanticUi.(
    <Sidebar
      visible
      animation=`push
      direction=`left
      width=`wide
      onHide={(_, _) => setVisible(_ => false)}>
      <Menu fluid=true vertical=true tabular=true>
        <MenuItem name="base_types">
          <Icon name="file code outline" />
          "Base Types"->React.string
        </MenuItem>
        <MenuItem name="base_types2" active=true>
          <Icon name="file code" />
          "More Types"->React.string
        </MenuItem>
        <MenuItem name="base_types3">
          <Icon name="file code outline" />
          "Nothing to see here yet, WIP"->React.string
        </MenuItem>
      </Menu>
    </Sidebar>
  );
};
module Async = {
  @react.component
  let make = () => {
    let (visible, setVisible) = Recoil.useRecoilState(State.examples_open);
    let examples = Recoil.useRecoilValue(State.example_list);
    let setSelectedExample = Recoil.useSetRecoilState(State.selected_example);
    let setSelectedFile = Recoil.useSetRecoilState(State.selected_file);
    let selected = Recoil.useRecoilValue(State.selected_example);
    React.useEffect1(
      () => {
        switch (examples->Belt.Array.get(0)) {
        | None => ()
        | Some(example) =>
          setSelectedExample(_ => Some(example.path));
          setSelectedFile(_ =>
            Some(
              Fp.absoluteExn(example.path)
              ->Fp.append(example.meta.entry)
              ->Fp.toString,
            )
          );
        };
        None;
      },
      [examples->Belt.Array.length],
    );

    open SemanticUi;

      <Sidebar
        visible
        animation=#push
        direction=#left
        width=#wide
        onHide={(_, _) => setVisible(_ => false)}>
        <Menu fluid=true vertical=true tabular=true>
          {examples
           ->Belt.Array.map(example =>
               <MenuItem
                 key={example.path}
                 name={example.path}
                 active={selected == Some(example.path)}
                 onClick={(_, _) => {
                   setSelectedExample(_ => Some(example.path));
                   setSelectedFile(_ =>
                     Some(
                       Fp.absoluteExn(example.path)
                       ->Fp.append(example.meta.entry)
                       ->Fp.toString,
                     )
                   );
                 }}>
                 <Icon name="file code outline" />
                 {example.meta.name->React.string}
               </MenuItem>
             )
           ->React.array}
        </Menu>
      </Sidebar>
    ;
  };
};

@react.component
let make = () => {
  <React.Suspense fallback={"Loading..."->React.string}>
    <Async />
  </React.Suspense>;
};
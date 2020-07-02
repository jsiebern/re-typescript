module MonacoEditor = {
  @react.component @bs.module("@monaco-editor/react")
  external make:
    (
      ~height: string=?,
      ~width: string=?,
      ~language: @bs.string [ | #javascript | #typescript],
      ~value: string,
      ~editorDidMount: ('a, 'b) => unit=?,
      ~onChange: ('a, string) => unit
    ) =>
    React.element =
    "ControlledEditor";
};

module EditorWindow = {
  let tmr: ref<option<Js.Global.timeoutId>> = ref(None);
  @react.component
  let make = () => {
    let (src, setSrc) = Recoil.useRecoilState(State.ts_source);
    let (value, setValue) = React.useReducer((_, v) => v, "");
    let parsedResult = Preview.useParsedResult();
    React.useEffect1(
      () => {
        switch (parsedResult) {
        | Ok((_, Some(nSrc))) =>
          setSrc(_ => Some(nSrc));
          setValue(nSrc);
        | _ => ()
        };
        None;
      },
      [parsedResult],
    );

    let tmr = React.useRef((-1)->Obj.magic);

    React.useEffect1(
      () => {
        tmr.current =
          Js.Global.setTimeout(
            () => {
              switch (value, src) {
              | ("", None) => ()
              | _ => setSrc(_ => Some(value))
              }
            },
            600,
          );

        Some(() => Js.Global.clearTimeout(tmr.current));
      },
      [value],
    );

    <MonacoEditor
      height="100%"
      width="100%"
      value={value == "" ? " " : value}
      onChange={(_, v) => setValue(v)}
      language=#typescript
    />;
  };
};

module Container = %styled.div(`
  height: 100%;
  width: 50vw;
  position: relative;
  background-color: #FFFFFF;
  display: flex;
  flex-direction: column;
`
);
module ContainerEditor = %styled.div(`
  flex-grow: 1;
  width: 100%;
  position: relative;
`
);
let containerTabs = %css(`
  width: 100%;
  height: 65px;
`);

module ExampleLabel = {
  module Inner = {
    @react.component
    let make = () => {
      let example = Recoil.useRecoilValue(State.selected_example_t);
      switch (example) {
      | None => React.null
      | Some(example) => example.meta.name->React.string
      };
    };
  };

  @react.component
  let make = () => {
    open SemanticUi;

      <Label attached="top right" className=%css(`z-index: 1;`)>
        <Icon name="file code outline" />
        <Inner />
      </Label>
    ;
  };
};

module FileTabs = {
  module TabMenu = {
    @react.component
    let make = () => {
      let selectedExample = Recoil.useRecoilValue(State.selected_example_t);
      let (selectedFile, setSelectedFile) =
        Recoil.useRecoilState(State.selected_file);

      open SemanticUi;

        <Menu tabular=true>
          {switch (selectedExample) {
           | None => React.null
           | Some(example) =>
             let path = Fp.absoluteExn(example.path);
             example.files
             ->Belt.List.map(file =>
                 switch (file) {
                 | File(name) =>
                   let file_path = path->Fp.append(name)->Fp.toString;
                   <MenuItem
                     key=name
                     className="file-tab-menu-item"
                     name
                     active={selectedFile == Some(file_path)}
                     onClick={(_, _) =>
                       setSelectedFile(_ => Some(file_path))
                     }>
                     {name->React.string}
                   </MenuItem>;
                 | Folder(_) => React.null
                 }
               )
             ->Belt.List.toArray
             ->React.array;
           }}
        </Menu>
      ;
    };
  };

  @react.component
  let make = () => {
    open SemanticUi;

      <React.Suspense fallback={<Menu tabular=true> React.null </Menu>}>
        <TabMenu />
      </React.Suspense>
    ;
  };
};

@react.component
let make = () => {
  <Container>
    <div className=containerTabs> <FileTabs /> </div>
    <ContainerEditor> <EditorWindow /> </ContainerEditor>
    <React.Suspense fallback=React.null> <ExampleLabel /> </React.Suspense>
  </Container>;
};
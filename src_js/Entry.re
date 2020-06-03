module ReactSyntaxHighlighter = {
  type style;
  [@bs.val] [@bs.module "react-syntax-highlighter/dist/esm/styles/prism"]
  external docco: style = "atomDark";
  [@react.component] [@bs.module "react-syntax-highlighter"]
  external make:
    (
      ~language: [@bs.string] [
                   | [@bs.as "reason"] `Reason
                   | [@bs.as "ocaml"] `Ocaml
                 ],
      ~style: style,
      ~children: string,
      ~showLineNumbers: option(bool)=?
    ) =>
    React.element =
    "Prism";
};

module MonacoEditor = {
  [@react.component] [@bs.module "@monaco-editor/react"]
  external make:
    (
      ~height: string=?,
      ~width: string=?,
      ~language: [@bs.string] [ | `javascript | `typescript],
      ~value: string,
      ~onChange: ('a, string) => unit
    ) =>
    React.element =
    "ControlledEditor";
};

let tmr: ref(option(Js.Global.timeoutId)) = ref(None);
module Editor = {
  [@react.component]
  let make = () => {
    let (src, setSrc) = Recoil.useRecoilState(State.ts_source);
    let (value, setValue) = React.useReducer((_, v) => v, src);
    let tmr = React.useRef((-1)->Obj.magic);

    React.useEffect1(
      () => {
        tmr.current = Js.Global.setTimeout(() => {setSrc(_ => value)}, 600);

        Some(() => Js.Global.clearTimeout(tmr.current));
      },
      [|value|],
    );

    <MonacoEditor
      height="100%"
      width="70vw"
      value
      onChange={(_, v) => setValue(v)}
      language=`typescript
    />;
  };
};

module Layout = [%styled.div
  {|
  display: flex;
  flex-direction: column;
  position: fixed;

  height: 100vh;
  width: 100vw;
|}
];

module Grid = [%styled.div {|
  display: flex;
  flex-grow: 1;
|}];
module Display = [%styled
  (~customheight) => {j|
  position: relative;
  display: inline-block;
  width: 50%;
  height: $customheight;
  overflow-y: auto;
  background-color: #282c34;
  color: white;
  position: relative;
|j}
];
module Error = [%styled.div
  {|
  position: absolute;
  width: 100%;
  height: 100%;
  left: 0;
  top: 0;
  padding: 15px;
  overflow: hidden;
  background-color: rgba(0, 0, 0, 0.8);
  color: white;
  display: flex;
  justify-content: center;
  align-items: center;
|}
];

module Result = {
  [@react.component]
  let make = () => {
    let lang = Recoil.useRecoilValue(State.language);
    let result = Recoil.useRecoilValueLoadable(State.parse_result);
    let (printed, setPrinted) = React.useReducer((_, v) => v, "");
    let (error, setError) = React.useReducer((_, v) => v, None);

    React.useEffect1(
      () => {
        switch (result->Recoil.Loadable.state) {
        | state when state === Recoil.Loadable.State.hasValue =>
          switch (result->Recoil.Loadable.getValue) {
          | Ok(src) =>
            setPrinted(src);
            setError(None);
          | Error((_, msg)) => setError(Some(msg))
          }
        | _ => ()
        };
        None;
      },
      [|result|],
    );
    <>
      {switch (error) {
       | None => React.null
       | Some(e) =>
         <Error key="error">
           <div
             key="sub_error"
             style={ReactDOMRe.Style.make(~whiteSpace="pre-wrap", ())}
             dangerouslySetInnerHTML={"__html": e}
           />
         </Error>
       }}
      <ReactSyntaxHighlighter
        showLineNumbers=true
        style=ReactSyntaxHighlighter.docco
        language={
          switch (lang) {
          | Reason => `Reason
          | Ocaml => `Ocaml
          }
        }>
        printed
      </ReactSyntaxHighlighter>
    </>;
  };
};

module X = {
  [@react.component]
  let make = () => {
    <Layout>
      <Header />
      <Grid>
        <Editor key="editor" />
        <Display customheight="calc(100vh - 50px)" key="display">
          <Result key="result" />
        </Display>
      </Grid>
    </Layout>;
  };
};

ReactDOMRe.renderToElementWithId(
  <Recoil.RecoilRoot> <X /> </Recoil.RecoilRoot>,
  "app",
);
module ReactSyntaxHighlighter = {
  type style
  @bs.val @bs.module("react-syntax-highlighter/dist/esm/styles/prism")
  external docco: style = "atomDark"
  @react.component @bs.module("react-syntax-highlighter")
  external make: (
    ~language: @bs.string [ | @bs.as("reason") #Reason | @bs.as("ocaml") #Ocaml ],
    ~style: style,
    ~children: string,
    ~showLineNumbers: option<bool>=?,
  ) => React.element = "Prism"
}

module Error = %styled.div(
  (~h) =>
    `
  position: fixed;
  width: 50vw;
  height: $h;
  right: 0;
  top: 50px;
  padding: 15px;
  overflow: hidden;
  background-color: rgba(0, 0, 0, 0.8);
  color: white;
  display: flex;
  justify-content: center;
  align-items: center;
`
)

let mt = "-2rem!important"
module Container = %styled.div(
  (~h) =>
    `
  height: $h;
  width: 50vw;
  top: $mt;
  position: relative;
  background-color: rgb(29, 31, 33);

  overflow-y: auto;
`
)

let useParsedResult = () => {
  Recoil.useRecoilValue(TsProject.parsedSource)
  // let result = Recoil.useRecoilValueLoadable(State.parse_result);
  // let (res, setRes) = React.useReducer((_, v) => v, Ok(("", "")));

  // React.useEffect1(
  // () => {
  // switch (result->Recoil.Loadable.state) {
  // | state when state === Recoil.Loadable.State.hasValue =>
  // switch (result->Recoil.Loadable.getValue) {
  // | Ok(r) => setRes(Ok(r))
  // | Error((_, msg)) => setRes(Error(msg))
  // }
  // | _ => ()
  // };
  // None;
  // },
  // [result],
  // );

  // res;
}

@react.component
let make = () => {
  let lang = Recoil.useRecoilValue(State.language)
  let result = useParsedResult()
  let (lastOk, setLastOk) = React.useReducer((_, v) => v, "")
  React.useEffect1(() => {
    switch result {
    | Ok(src) => setLastOk(src)
    | Error(_) => ()
    }
    None
  }, [result])

  <Container h="calc(100vh - 65px)">
    {switch result {
    | Error(e) =>
      <Error h="calc(100vh - 65px)" key="error">
        <div
          key="sub_error"
          style={ReactDOMRe.Style.make(~whiteSpace="pre-wrap", ())}
          dangerouslySetInnerHTML={"__html": e}
        />
      </Error>
    | Ok(_) => React.null
    }}
    <ReactSyntaxHighlighter
      showLineNumbers=true
      style=ReactSyntaxHighlighter.docco
      language={switch lang {
      | Reason => #Reason
      | Ocaml => #Ocaml
      }}>
      lastOk
    </ReactSyntaxHighlighter>
  </Container>
}

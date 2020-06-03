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
    width="50vw"
    value
    onChange={(_, v) => setValue(v)}
    language=`typescript
  />;
};
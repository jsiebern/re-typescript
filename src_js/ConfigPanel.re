let makeId = () => "i_" ++ string_of_int(Random.int(1000000));
let useId = () => {
  React.useMemo0(() => makeId());
};

module Switch = {
  [@react.component]
  let make = (~label, ~selector) => {
    let (value, setValue) = Recoil.useRecoilState(selector);
    let id = useId();

    <div className="form-group row">
      <div className="col">
        <label className="form-check-label" htmlFor=id>
          label->React.string
        </label>
      </div>
      <div className="col">
        <input
          checked=value
          onChange={_ => setValue(value => !value)}
          type_="checkbox"
          className="form-check-input"
          id
        />
      </div>
    </div>;
  };
};

module Radio = {
  [@react.component]
  let make = (~label, ~to_string, ~from_string, ~values, ~selector) => {
    let id = useId();
    let (value, setValue) = Recoil.useRecoilState(selector);
    let values =
      React.useMemo0(() =>
        values->Belt.Array.mapU((. (v, l)) => (v, l, makeId()))
      );

    <div className="form-group row">
      <div className="col">
        <label className="form-check-label" htmlFor=id>
          label->React.string
        </label>
      </div>
      <div className="col">
        {values
         ->Belt.Array.mapU((. (v, l, id)) =>
             <div className="form-check-inline" key=id>
               <input
                 className="form-check-input"
                 id
                 type_="radio"
                 name={"rad_" ++ id}
                 value={v->to_string}
                 checked={v === value}
                 onChange={e =>
                   setValue(_ => e->ReactEvent.Form.target##value->from_string)
                 }
               />
               <label className="form-check-label" htmlFor=id>
                 l->React.string
               </label>
             </div>
           )
         ->React.array}
      </div>
    </div>;
  };
};

module Form = {
  [@react.component]
  let make = (~close) => {
    <form className=[%css {|overflow-y: auto|}]>
      <Switch
        label="Suppress warning for extended records"
        selector=State.suppress_warning_for_extended_records
      />
      <Switch
        label="Omit extended, unreferenced records"
        selector=State.omit_extended_unreferenced_records
      />
      <Radio
        values=[|
          (Config_t.Float, "Float"),
          (Int, "Int"),
          (Unboxed, "Unboxed"),
        |]
        to_string={v => v->Config_bs.write_number_mode->Js.Json.stringify}
        from_string={v => v->Js.Json.parseExn->Config_bs.read_number_mode}
        label="Number mode"
        selector=State.number_mode
      />
      <Radio
        values=[|(Config_t.Array, "Array"), (List, "List")|]
        to_string={v => v->Config_bs.write_array_mode->Js.Json.stringify}
        from_string={v => v->Js.Json.parseExn->Config_bs.read_array_mode}
        label="Array mode"
        selector=State.array_mode
      />
      <Switch label="Generate parsers (n/a)" selector=State.generate_parser />
      <Switch
        label="Generate serializers (n/a)"
        selector=State.generate_serializer
      />
      <div> <b> "Intersection Types"->React.string </b> </div>
      {let values = [|
         (Config_bs.Merge, "Merge"),
         (Tuple, "Tuple"),
         (Ignore, "Ignore"),
       |];
       let from_string = v =>
         v->Js.Json.parseExn->Config_bs.read_intersection_def;
       let to_string = v =>
         v->Config_bs.write_intersection_def->Js.Json.stringify;
       <>
         <Radio
           values
           to_string
           from_string
           label="Intersection Mode for Objects"
           selector=State.intersection_objects
         />
         <Radio
           values
           to_string
           from_string
           label="Intersection Mode for Unions"
           selector=State.intersection_unions
         />
         <Radio
           values
           to_string
           from_string
           label="Intersection Mode for Classes"
           selector=State.intersection_classes
         />
         <Radio
           values
           to_string
           from_string
           label="Intersection Mode for Functions"
           selector=State.intersection_functions
         />
         <Radio
           values
           to_string
           from_string
           label="Intersection Mode for Other"
           selector=State.intersection_other
         />
         <Switch
           label="Make tuple members optional"
           selector=State.intersection_tuple_members_optional
         />
       </>}
      <button onClick={_ => close()}> "Close"->React.string </button>
    </form>;
  };
};

module Container = [%styled.div
  (~h) => {|
    height: $h;
    width: 60vw;
    top: 0;
    left: 0;
    position: absolute;
    z-index: 2;
    background-color: white;
    overflow-y: auto;
    padding: 15px;
|}
];

[@react.component]
let make = () => {
  let (open_, setOpen) = Recoil.useRecoilState(State.config_open);

  open_
    ? <Container h="calc(100vh - 50px)">
        <Form close={_ => setOpen(_ => false)} />
      </Container>
    : React.null;
};
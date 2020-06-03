let makeId = () => "i_" ++ string_of_int(Random.int(1000000));
let useId = () => {
  React.useMemo0(() => makeId());
};

module Tooltip = {
  [@react.component]
  let make = (~tooltip) => {
    SemanticUi.(
      {
        switch (tooltip) {
        | None =>
          <Button
            compact=true
            circular=true
            icon="help"
            size=`mini
            color=`blue
            className=[%css {|visibility: hidden;|}]
          />
        | Some(tt) =>
          <Popup
            trigger={
              <Button
                compact=true
                circular=true
                icon="help"
                size=`mini
                color=`blue
              />
            }
            content={tt->React.string}
          />
        };
      }
    );
  };
};

module ConfigSwitch = {
  [@react.component]
  let make = (~tooltip=?, ~disabled=?, ~label, ~selector) => {
    let (value, setValue) = Recoil.useRecoilState(selector);
    let id = useId();

    SemanticUi.(
      <FormField inline=true>
        <Tooltip tooltip />
        <Checkbox
          label={label->React.string}
          ?disabled
          id
          toggle=true
          checked=value
          onChange={(_, {checked}) => setValue(_ => checked)}
        />
      </FormField>
    );
  };
};

module ConfigSelect = {
  [@react.component]
  let make =
      (
        ~tooltip=?,
        ~disabled=?,
        ~label,
        ~to_string: 'a => string,
        ~from_string: string => 'a,
        ~values,
        ~selector,
      ) => {
    let id = useId();
    let (value, setValue) = Recoil.useRecoilState(selector);
    let values =
      React.useMemo0(() =>
        values->Belt.Array.mapU((. (v, l)) => (v, l, makeId()))
      );

    let zero = "0px!important";
    let ifl = "inline-flex!important";
    let cen = "center!important";
    SemanticUi.(
      <FormField inline=true>
        <Grid columns=Grid.Columns.equal>
          <GridColumn
            className=[%css
              {j|padding: $zero; align-items: $cen; display: $ifl;|j}
            ]>
            <label> <Tooltip tooltip /> label->React.string </label>
          </GridColumn>
          <GridColumn
            className=[%css
              {j|padding: $zero; align-items: $cen; display: $ifl;|j}
            ]>
            <FormSelect
              className=[%css {|display: inline|}]
              fluid=false
              inline=true
              ?disabled
              options={
                values->Belt.Array.mapU((. (v, l, _)) =>
                  {
                    FormSelect.key: v->to_string,
                    value: FormSelect.Value.string(v->to_string),
                    text: l,
                  }
                )
              }
              id
              value={FormSelect.Value.string(value->to_string)}
              onChange={(_, {value}) => {
                setValue(_ => value->Obj.magic->from_string)
              }}
            />
          </GridColumn>
        </Grid>
      </FormField>
    );
  };
};

module ConfigForm = {
  module Styles = {
    let panelheight = "calc(100vh - 65px)";
    let formContainer = [%css
      {|
        height: $panelheight;
        background-color: white;
        padding: 0.8rem;
        overflow: hidden;
        |}
    ];
  };

  [@react.component]
  let make = () => {
    SemanticUi.(
      <div className=Styles.formContainer>
        <Grid>
          <GridColumn>
            <Form>
              <Header> "Interface Extension"->React.string </Header>
              <ConfigSwitch
                tooltip="Interfaces are literally extended, so you end up with multiple records that share some field names. Bucklescript will display a warning (code 30) for this. When this option is enabled, a warning attribute will be added at the top of the type definitions (only if necessary)."
                label="Suppress warning for extended interfaces"
                selector=State.suppress_warning_for_extended_records
              />
              <ConfigSwitch
                label="Omit extended, unreferenced interfaces"
                selector=State.omit_extended_unreferenced_records
              />
              <Header> "Base Types"->React.string </Header>
              <ConfigSelect
                values=[|
                  (Config_t.Float, "Float"),
                  (Int, "Int"),
                  (Unboxed, "Unboxed"),
                |]
                to_string={v =>
                  v->Config_bs.write_number_mode->Js.Json.stringify
                }
                from_string={v =>
                  v->Js.Json.parseExn->Config_bs.read_number_mode
                }
                label="Number mode"
                selector=State.number_mode
              />
              <ConfigSelect
                values=[|(Config_t.Array, "Array"), (List, "List")|]
                to_string={v =>
                  v->Config_bs.write_array_mode->Js.Json.stringify
                }
                from_string={v =>
                  v->Js.Json.parseExn->Config_bs.read_array_mode
                }
                label="Array mode"
                selector=State.array_mode
              />
              <Header> "Intersection generation"->React.string </Header>
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
                 <ConfigSelect
                   values
                   to_string
                   from_string
                   label="For Objects"
                   selector=State.intersection_objects
                 />
                 <ConfigSelect
                   values
                   to_string
                   from_string
                   label="For Unions"
                   selector=State.intersection_unions
                 />
                 <ConfigSelect
                   values
                   to_string
                   from_string
                   label="For Classes"
                   selector=State.intersection_classes
                 />
                 <ConfigSelect
                   values
                   to_string
                   from_string
                   label="For Functions"
                   selector=State.intersection_functions
                 />
                 <ConfigSelect
                   values
                   to_string
                   from_string
                   label="For Other"
                   selector=State.intersection_other
                 />
                 <ConfigSwitch
                   label="Make tuple members optional"
                   selector=State.intersection_tuple_members_optional
                 />
               </>}
              <Header> "Parser / Serializer"->React.string </Header>
              <ConfigSwitch
                label="Generate parsers"
                selector=State.generate_parser
                disabled=true
              />
              <ConfigSwitch
                label="Generate serializers"
                selector=State.generate_serializer
                disabled=true
              />
            </Form>
          </GridColumn>
        </Grid>
      </div>
    );
  };
};

let ti = "transparent!important";
[@react.component]
let make = (~children: React.element) => {
  let (visible, setVisible) = Recoil.useRecoilState(State.config_open);
  SemanticUi.(
    <SidebarPushable>
      <Sidebar visible animation=`overlay direction=`left width=`very_wide>
        <div className=[%css {|position: relative; margin-top: -2rem;|}]>
          <Label
            onClick={(_, _) => setVisible(_ => false)}
            className=[%css {|border-color: $ti; z-index: 103;|}]
            attached="top right"
            basic=true>
            <Icon name="close" color=`red />
          </Label>
          <ConfigForm />
        </div>
      </Sidebar>
      <SidebarPusher dimmed=visible> children </SidebarPusher>
    </SidebarPushable>
  );
};
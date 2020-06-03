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

let zero = "0px!important";
let ifl = "inline-flex!important";
let cen = "center!important";
module ConfigSwitch = {
  [@react.component]
  let make = (~tooltip=?, ~disabled=?, ~label, ~selector) => {
    let (value, setValue) = Recoil.useRecoilState(selector);
    let id = useId();

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
            <Checkbox
              ?disabled
              id
              toggle=true
              checked=value
              onChange={(_, {checked}) => setValue(_ => checked)}
            />
          </GridColumn>
        </Grid>
      </FormField>
    );
  };
};

module ConfigSelect = {
  external asString: 'a => string = "%identity";
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
                setValue(_ => value->asString->from_string)
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
        overflow-x: hidden;
        overflow-y: auto;
        |}
    ];
  };

  module IntSet = Belt.Set.Int;
  let index_state =
    Recoil.atom({key: "index_state", default: IntSet.fromArray([|5|])});
  module AsAccordion = {
    [@react.component]
    let make = (~label, ~index, ~children) => {
      let (state, setState) = Recoil.useRecoilState(index_state);
      let active =
        React.useMemo1(() => state->IntSet.has(index), [|state|]);
      let handleClick = (_, _) =>
        setState(state =>
          state->IntSet.has(index)
            ? state->IntSet.remove(index) : state->IntSet.add(index)
        );

      SemanticUi.(
        <>
          <AccordionTitle index=0 active onClick=handleClick>
            <Icon name="dropdown" />
            label->React.string
          </AccordionTitle>
          <AccordionContent active> children </AccordionContent>
        </>
      );
    };
  };
  [@react.component]
  let make = () => {
    let output_type = Recoil.useRecoilValue(State.output_type);
    let (bs_number_value, set_bs_number_value) =
      Recoil.useRecoilState(State.bs_number_value);

    SemanticUi.(
      <div className=Styles.formContainer>
        <Grid>
          <GridColumn>
            <Form>
              <Header> "Configuration"->React.string </Header>
              <Accordion styled=true fluid=true>
                <AsAccordion index=5 label="Generator Target">
                  <ConfigSelect
                    values=[|
                      (Config_t.Bucklescript, "Bucklescript"),
                      (BucklescriptBindings, "BucklescriptBindings"),
                      (Native, "Native"),
                    |]
                    to_string={v =>
                      v->Config_bs.write_output_type->Js.Json.stringify
                    }
                    from_string={v =>
                      v->Js.Json.parseExn->Config_bs.read_output_type
                    }
                    label="Output Mode"
                    selector=State.output_type
                  />
                  {switch (output_type) {
                   | Native => React.null
                   | Bucklescript
                   | BucklescriptBindings =>
                     <>
                       <ConfigSelect
                         values=[|
                           (`Variant, "Variant"),
                           (`PolyVariant, "PolyVariant"),
                           (`BsInline, "BsInline"),
                         |]
                         to_string={v =>
                           v
                           ->Config_bs.write_string_variant_mode
                           ->Js.Json.stringify
                         }
                         from_string={v =>
                           v
                           ->Js.Json.parseExn
                           ->Config_bs.read_string_variant_mode
                         }
                         label="String Literals Mode"
                         selector=State.bs_string_variant_mode
                       />
                       <ConfigSelect
                         values=[|
                           (`BsUnboxed, "BsUnboxed"),
                           (`Variant, "Variant"),
                           (`PolyVariant, "PolyVariant"),
                         |]
                         to_string={v =>
                           v
                           ->Config_bs.write_mixed_variant_mode
                           ->Js.Json.stringify
                         }
                         from_string={v =>
                           v
                           ->Js.Json.parseExn
                           ->Config_bs.read_mixed_variant_mode
                         }
                         label="Mixed Literals Mode"
                         selector=State.bs_mixed_variant_mode
                       />
                       <Divider />
                       <ConfigSelect
                         values=[|
                           (`Variant(bs_number_value), "Variant"),
                           (`PolyVariant(bs_number_value), "PolyVariant"),
                           (`BsInline(bs_number_value), "BsInline"),
                         |]
                         to_string=(
                           fun
                           | `Variant(_) => "Variant"
                           | `PolyVariant(_) => "PolyVariant"
                           | `BsInline(_) => "BsInline"
                         )
                         from_string={(v: string): Config_bs.number_variant_mode =>
                           switch (v) {
                           | "Variant" => `Variant(bs_number_value)
                           | "PolyVariant" => `PolyVariant(bs_number_value)
                           | "BsInline" => `BsInline(bs_number_value)
                           | _ => raise(Not_found)
                           }
                         }
                         label="Numeric Literals Mode"
                         selector=State.bs_number_variant_mode
                       />
                       <FormField inline=true>
                         <Grid columns=Grid.Columns.equal>
                           <GridColumn
                             className=[%css
                               {j|padding: $zero; align-items: $cen; display: $ifl;|j}
                             ]>
                             <label>
                               <Tooltip tooltip=None />
                               "Custom Pre/Suffix"->React.string
                             </label>
                           </GridColumn>
                           <GridColumn
                             className=[%css
                               {j|padding: $zero; align-items: $cen; display: $ifl;|j}
                             ]>
                             <Checkbox
                               toggle=true
                               checked={bs_number_value !== None}
                               onChange={(_, _) =>
                                 set_bs_number_value(v =>
                                   switch (v) {
                                   | None => Some({prefix: "_", suffix: ""})
                                   | Some(_) => None
                                   }
                                 )
                               }
                             />
                           </GridColumn>
                         </Grid>
                       </FormField>
                       {switch (bs_number_value) {
                        | None => React.null
                        | Some({prefix, suffix}) =>
                          <>
                            <FormField inline=true>
                              <Grid columns=Grid.Columns.equal>
                                <GridColumn
                                  className=[%css
                                    {j|padding: $zero; align-items: $cen; display: $ifl;|j}
                                  ]>
                                  <label>
                                    <Tooltip tooltip=None />
                                    "Prefix"->React.string
                                  </label>
                                </GridColumn>
                                <GridColumn
                                  className=[%css
                                    {j|padding: $zero; align-items: $cen; display: $ifl;|j}
                                  ]>
                                  <input
                                    value=prefix
                                    onChange={e =>
                                      set_bs_number_value(_ =>
                                        Some({
                                          prefix:
                                            e->ReactEvent.Form.target##value,
                                          suffix,
                                        })
                                      )
                                    }
                                  />
                                </GridColumn>
                              </Grid>
                            </FormField>
                            <FormField inline=true>
                              <Grid columns=Grid.Columns.equal>
                                <GridColumn
                                  className=[%css
                                    {j|padding: $zero; align-items: $cen; display: $ifl;|j}
                                  ]>
                                  <label>
                                    <Tooltip tooltip=None />
                                    "Suffix"->React.string
                                  </label>
                                </GridColumn>
                                <GridColumn
                                  className=[%css
                                    {j|padding: $zero; align-items: $cen; display: $ifl;|j}
                                  ]>
                                  <input
                                    value=suffix
                                    onChange={e =>
                                      set_bs_number_value(_ =>
                                        Some({
                                          prefix,
                                          suffix:
                                            e->ReactEvent.Form.target##value,
                                        })
                                      )
                                    }
                                  />
                                </GridColumn>
                              </Grid>
                            </FormField>
                          </>
                        }}
                     </>
                   }}
                </AsAccordion>
                <AsAccordion index=0 label="Interface Extension">
                  <ConfigSwitch
                    tooltip="Interfaces are literally extended, so you end up with multiple records that share some field names. Bucklescript will display a warning (code 30) for this. When this option is enabled, a warning attribute will be added at the top of the type definitions (only if necessary)."
                    label="Suppress warning 30"
                    selector=State.suppress_warning_for_extended_records
                  />
                  <ConfigSwitch
                    label="Omit empty interfaces"
                    selector=State.omit_extended_unreferenced_records
                  />
                </AsAccordion>
                <AsAccordion index=1 label="Base Types">
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
                </AsAccordion>
                <AsAccordion index=2 label="Intersection generation">
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
                </AsAccordion>
                <AsAccordion index=3 label="Parser / Serializer">
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
                </AsAccordion>
              </Accordion>
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
      <ExamplePanel />
      <Sidebar
        visible
        animation=`push
        direction=`right
        width=`very_wide
        onHide={(_, _) => setVisible(_ => false)}>
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
      <SidebarPusher> children </SidebarPusher>
    </SidebarPushable>
  );
};
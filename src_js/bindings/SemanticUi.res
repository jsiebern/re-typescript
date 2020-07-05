module Grid = {
  module Columns: {
    type t
    let number: int => t
    let equal: t
  } = {
    @unboxed
    type rec t = Any('a): t
    let number = (v: int) => Any(v)
    let equal = Any("equal")
  }
  module Padded: {
    type t
    let horizontally: t
    let vertically: t
    let boolean: bool => t
  } = {
    @unboxed
    type rec t = Any('a): t
    let horizontally = Any("horizontally")
    let vertically = Any("vertically")
    let boolean = (v: bool) => Any(v)
  }
  @react.component @bs.module("semantic-ui-react")
  external make: (
    ~className: string=?,
    ~stretched: bool=?,
    ~container: bool=?,
    ~columns: Columns.t=?,
    ~padded: Padded.t=?,
    ~children: React.element,
  ) => React.element = "Grid"
}

module GridColumn = {
  @react.component @bs.module("semantic-ui-react")
  external make: (~className: string=?, ~width: int=?, ~children: React.element) => React.element =
    "GridColumn"
}

module GridRow = {
  @react.component @bs.module("semantic-ui-react")
  external make: (
    ~className: string=?,
    ~stretched: bool=?,
    ~children: React.element,
  ) => React.element = "GridRow"
}

module Segment = {
  @react.component @bs.module("semantic-ui-react")
  external make: (
    ~className: string=?,
    ~basic: bool=?,
    ~circular: bool=?,
    ~compact: bool=?,
    ~padded: bool=?,
    ~placeholder: bool=?,
    ~raised: bool=?,
    ~secondary: bool=?,
    ~vertical: bool=?,
    ~children: React.element,
  ) => React.element = "Segment"
}

module Image = {
  module Spaced: {
    type t
    let left: t
    let right: t
    let true_: t
    let false_: t
  } = {
    @unboxed
    type rec t = Any('a): t
    let left = Any("left")
    let right = Any("right")
    let true_ = Any(true)
    let false_ = Any(false)
  }
  @react.component @bs.module("semantic-ui-react")
  external make: (
    ~className: string=?,
    ~href: string=?,
    ~spaced: Spaced.t=?,
    ~size: @bs.string [ | #mini | #tiny | #small | #medium | #large | #big | #huge | #massive ]=?,
    ~inline: bool=?,
    ~src: string,
  ) => React.element = "Image"
}

module Menu = {
  @react.component @bs.module("semantic-ui-react")
  external make: (
    ~className: string=?,
    ~fluid: bool=?,
    ~stackable: bool=?,
    ~vertical: bool=?,
    ~tabular: bool=?,
    ~icons: @bs.string [ | #labeled ]=?,
    ~children: React.element,
  ) => React.element = "Menu"
}

module MenuMenu = {
  @react.component @bs.module("semantic-ui-react")
  external make: (
    ~className: string=?,
    ~position: @bs.string [ | #left | #right ]=?,
    ~children: React.element,
  ) => React.element = "MenuMenu"
}

module MenuItem = {
  type clickReturn = {name: string}
  @react.component @bs.module("semantic-ui-react")
  external make: (
    ~className: string=?,
    ~name: string=?,
    ~active: bool=?,
    ~fitted: bool=?,
    ~style: ReactDOMRe.Style.t=?,
    ~onClick: (ReactEvent.Mouse.t, clickReturn) => unit=?,
    ~children: React.element,
  ) => React.element = "MenuItem"
}

module Header = {
  module Image: {
    type t
    let src: string => t
    let component: React.element => t
  } = {
    @unboxed
    type rec t = Any('a): t
    let src = (v: string) => Any(v)
    let component = (v: React.element) => Any(v)
  }

  @react.component @bs.module("semantic-ui-react")
  external make: (
    ~className: string=?,
    ~_as: string=?,
    ~image: Image.t=?,
    ~icon: string=?,
    ~content: React.element=?,
    ~children: React.element=?,
  ) => React.element = "Header"
}

module Label = {
  type clickReturn = {name: string}
  @react.component @bs.module("semantic-ui-react")
  external make: (
    ~className: string=?,
    ~attached: string=?,
    ~icon: string=?,
    ~basic: bool=?,
    ~corner: bool=?,
    ~empty: bool=?,
    ~floating: bool=?,
    ~horizontal: bool=?,
    ~circular: bool=?,
    ~onClick: (ReactEvent.Mouse.t, 'a) => unit=?,
    ~onRemove: (ReactEvent.Mouse.t, 'a) => unit=?,
    ~color: @bs.string
    [
    | #red
    | #orange
    | #yellow
    | #olive
    | #green
    | #teal
    | #blue
    | #violet
    | #purple
    | #pink
    | #brown
    | #grey
    | #black
    ]=?,
    ~children: React.element=?,
  ) => React.element = "Label"
}

module Icon = {
  type clickReturn = {name: string}
  @react.component @bs.module("semantic-ui-react")
  external make: (
    ~className: string=?,
    ~color: @bs.string
    [
    | #red
    | #orange
    | #yellow
    | #olive
    | #green
    | #teal
    | #blue
    | #violet
    | #purple
    | #pink
    | #brown
    | #grey
    | #black
    ]=?,
    ~name: string=?,
  ) => React.element = "Icon"
}

module Popup = {
  type clickReturn = {name: string}
  @react.component @bs.module("semantic-ui-react")
  external make: (
    ~className: string=?,
    ~content: React.element=?,
    ~trigger: React.element,
    ~offset: string=?,
    ~pinned: bool=?,
    ~position: string=?,
  ) => React.element = "Popup"
}

module Sidebar = {
  @bs.val @bs.module("@stardust-ui/react-component-event-listener")
  external documentRef: React.ref<Dom.element> = "documentRef"

  type clickReturn = {name: string}
  @react.component @bs.module("semantic-ui-react")
  external make: (
    ~onHide: ('a, 'b) => unit,
    ~className: string=?,
    ~animation: @bs.string
    [
    | #overlay
    | #push
    | @bs.as("scale down")
    #scale_down
    | #uncover
    | @bs.as("slide out")
    #slide_out
    | @bs.as("slide along")
    #slide_along
    ]=?,
    ~direction: @bs.string [ | #top | #right | #bottom | #left ]=?,
    ~visible: bool=?,
    ~width: @bs.string
    [ | @bs.as("very thin") #very_thin | #thin | #wide | @bs.as("very wide") #very_wide ]=?,
    ~vertical: bool=?,
    ~target: Dom.element=?,
    ~_as: 'a=?,
    ~content: React.element=?,
    ~children: React.element,
  ) => React.element = "Sidebar"
}

module SidebarPusher = {
  type clickReturn = {name: string}
  @react.component @bs.module("semantic-ui-react")
  external make: (
    ~className: string=?,
    ~dimmed: bool=?,
    ~children: React.element,
  ) => React.element = "SidebarPusher"
}

module SidebarPushable = {
  @react.component @bs.module("semantic-ui-react")
  external make: (~className: string=?, ~children: React.element) => React.element =
    "SidebarPushable"
}

module Form = {
  @react.component @bs.module("semantic-ui-react")
  external make: (~className: string=?, ~children: React.element) => React.element = "Form"
}

module FormGroup = {
  module Widths: {
    type t
    let number: int => t
    let equal: t
  } = {
    @unboxed
    type rec t = Any('a): t
    let number = (v: int) => Any(v)
    let equal = Any("equal")
  }

  @react.component @bs.module("semantic-ui-react")
  external make: (
    ~className: string=?,
    ~inline: bool=?,
    ~widths: Widths.t=?,
    ~children: React.element,
  ) => React.element = "FormGroup"
}

module FormField = {
  @react.component @bs.module("semantic-ui-react")
  external make: (
    ~className: string=?,
    ~label: React.element=?,
    ~inline: bool=?,
    ~children: React.element,
  ) => React.element = "FormField"
}

module Checkbox = {
  type on_change = {checked: bool}
  @react.component @bs.module("semantic-ui-react")
  external make: (
    ~className: string=?,
    ~toggle: bool=?,
    ~checked: bool=?,
    ~defaultChecked: bool=?,
    ~defaultIndeterminate: bool=?,
    ~indeterminate: bool=?,
    ~disabled: bool=?,
    ~fitted: bool=?,
    ~radio: bool=?,
    ~readOnly: bool=?,
    ~slider: bool=?,
    ~value: string=?,
    ~id: string=?,
    ~name: string=?,
    ~label: React.element=?,
    ~onChange: (ReactEvent.Form.t, on_change) => unit,
  ) => React.element = "Checkbox"
}

module Button = {
  @react.component @bs.module("semantic-ui-react")
  external make: (
    ~className: string=?,
    ~attached: string=?,
    ~icon: string=?,
    ~circular: bool=?,
    ~compact: bool=?,
    ~color: @bs.string
    [
    | #red
    | #orange
    | #yellow
    | #olive
    | #green
    | #teal
    | #blue
    | #violet
    | #purple
    | #pink
    | #brown
    | #grey
    | #black
    ]=?,
    ~size: @bs.string [ | #mini | #tiny | #small | #medium | #large | #big | #huge | #massive ]=?,
    ~children: React.element=?,
  ) => React.element = "Button"
}

module FormSelect = {
  module Value: {
    type t
    let boolean: bool => t
    let number: int => t
    let string: string => t
  } = {
    @unboxed
    type rec t = Any('a): t
    let boolean = (v: bool) => Any(v)
    let number = (v: int) => Any(v)
    let string = (v: string) => Any(v)
  }
  type select_option = {
    key: string,
    value: Value.t,
    text: string,
  }

  @react.component @bs.module("semantic-ui-react")
  external make: (
    ~className: string=?,
    ~id: string=?,
    ~placeholder: string=?,
    ~closeOnBlur: bool=?,
    ~inline: bool=?,
    ~closeOnEscape: bool=?,
    ~closeOnChange: bool=?,
    ~defaultValue: Value.t=?,
    ~value: Value.t=?,
    ~disabled: bool=?,
    ~fluid: bool=?,
    ~options: array<select_option>=?,
    ~onChange: (ReactEvent.Form.t, select_option) => unit,
  ) => React.element = "FormSelect"
}

module Accordion = {
  @react.component @bs.module("semantic-ui-react")
  external make: (
    ~className: string=?,
    ~fluid: bool=?,
    ~styled: bool=?,
    ~children: React.element,
  ) => React.element = "Accordion"
}

module AccordionTitle = {
  @react.component @bs.module("semantic-ui-react")
  external make: (
    ~className: string=?,
    ~active: bool=?,
    ~icon: string=?,
    ~index: int=?,
    ~onClick: (ReactEvent.Mouse.t, 'a) => unit,
    ~children: React.element,
  ) => React.element = "AccordionTitle"
}

module AccordionContent = {
  @react.component @bs.module("semantic-ui-react")
  external make: (
    ~className: string=?,
    ~active: bool=?,
    ~children: React.element,
  ) => React.element = "AccordionContent"
}
module Divider = {
  @react.component @bs.module("semantic-ui-react")
  external make: unit => React.element = "Divider"
}

module Tab = {
  type pane = {
    menuItem: React.element,
    render: unit => React.element,
  }
  type menu = {
    fluid: bool,
    vertical: bool,
    tabular: bool,
  }

  @react.component @bs.module("semantic-ui-react")
  external make: (~menu: menu=?, ~panes: array<pane>) => React.element = "Tab"
}

module TabPane = {
  @react.component @bs.module("semantic-ui-react")
  external make: (~children: React.element) => React.element = "TabPane"
}

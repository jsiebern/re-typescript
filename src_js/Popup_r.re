[@react.component] [@bs.module "reactjs-popup"]
external make:
  (
    ~trigger: React.element=?,
    ~position: string=?,
    ~closeOnDocumentClick: bool=?,
    ~defaultOpen: bool=?,
    ~_open: bool=?,
    ~arrow: bool=?,
    ~modal: bool=?,
    ~lockScroll: bool=?,
    ~closeOnEscape: bool=?,
    ~repositionOnResize: bool=?,
    ~contentStyle: Js.t({..})=?,
    ~_on: [@bs.string] [ | `hover | `click | `focus]=?,
    ~children: (. (unit => unit)) => React.element
  ) =>
  React.element =
  "default";
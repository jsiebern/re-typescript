type color =
  | Red;

[@react.component]
let make = (~italic=?, ~bold=?, ~color: option(color)=?, ~children) => {
  <div
    style={ReactDOMRe.Style.make(
      ~fontWeight=?bold->Belt.Option.map(_ => "bold"),
      ~textDecoration=?italic->Belt.Option.map(_ => "italic"),
      ~color=?
        color->Belt.Option.map(
          fun
          | Red => "red",
        ),
      (),
    )}>
    children->Obj.magic
  </div>;
};
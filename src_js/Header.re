module Container = [%styled.div
  {|
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 0 15px;
  min-height: 50px;
  width: 100%;
  border-bottom: 1px solid #d6d4d4;
|}
];
module Headline = [%styled.span {|
  font-size: 1.5rem;
|}];

module Button = [%styled.button
  {|
  height: 50px;
  display: flex;
  align-items: center;
  cursor: pointer;
  outline: 0px solid transparent;
  border: 0px solid transparent;
  font-size: 1rem;
  padding: 0 15px;
  background-color: white;
  border-bottom: 1px solid #d6d4d4;

  &:hover {
    color: orange;
  }
|}
];

module SwitchLanguageButton = {
  [@react.component]
  let make = () => {
    let (lang, setLang) = Recoil.useRecoilState(State.language);
    <Button onClick={_ => setLang(lang => lang === Reason ? Ocaml : Reason)}>
      {switch (lang) {
       | Reason => "ReasonML"
       | Ocaml => "ocaml"
       }}
      ->React.string
    </Button>;
  };
};

[@react.component]
let make = () => {
  <Container>
    <Headline> "re-typescript"->React.string </Headline>
    <SwitchLanguageButton />
  </Container>;
};
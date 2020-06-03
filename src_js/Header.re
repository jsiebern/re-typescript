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
  padding: 5px;
  cursor: pointer;
  outline: 0px solid transparent;
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
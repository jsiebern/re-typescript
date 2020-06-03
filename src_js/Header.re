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
module Headline = [%styled.div
  {|
  font-size: 1.5rem;
  height: 50px;
  border-bottom: 1px solid #d6d4d4;
  display: flex;
  align-items: center;
|}
];

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
  min-width: 125px;
  text-align: left;

  &:hover {
    color: orange;
  }
|}
];

type img = {default: string};
[@bs.val] external img: string => img = "require";
let logo_reason = img("./../../../src_js/assets/reasonml.png");
let logo_ocaml = img("./../../../src_js/assets/ocaml.png");
let logo_ret = img("./../../../src_js/assets/re_typescript.png");
let logo = [%css {|
  max-height: 1rem;
  margin-right: 0.2rem;
|}];
let logo_r = [%css
  {|
  max-height: 1.8rem;
  margin-right: 0.4rem;
  margin-top: 0.2rem;
|}
];

module SwitchLanguageButton = {
  [@react.component]
  let make = () => {
    let (lang, setLang) = Recoil.useRecoilState(State.language);
    <Button onClick={_ => setLang(lang => lang === Reason ? Ocaml : Reason)}>
      {switch (lang) {
       | Reason =>
         <>
           <img className=logo src={logo_reason.default} />
           "ReasonML"->React.string
         </>
       | Ocaml =>
         <>
           <img className=logo src={logo_ocaml.default} />
           "OCaml"->React.string
         </>
       }}
    </Button>;
  };
};

[@react.component]
let make = () => {
  <Container>
    <Headline>
      <img className=logo_r src={logo_ret.default} />
      "re-typescript"->React.string
    </Headline>
    <SwitchLanguageButton />
  </Container>;
};
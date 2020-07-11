module Generic: {
  class t:
    (Ojs.t) =>
    {
      inherit Ts_morph.Node.t;
      pub set_kindName: string => unit;
    };
  [@js.cast]
  let cast: Ts_morph.Node.t => t;
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module TypeDeclaration: {
  class t:
    (Ojs.t) =>
    {
      inherit Ts_morph.Node.t;
    };
  [@js.cast]
  let cast: Generic.t => t;
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module Identifier: {
  class t:
    (Ojs.t) =>
    {
      inherit Ts_morph.Node.t;
    };
  [@js.cast]
  let cast: Generic.t => t;
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};
module StringKeyword: {
  class t:
    (Ojs.t) =>
    {
      inherit Ts_morph.Node.t;
    };
  [@js.cast]
  let cast: Generic.t => t;
  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};

module Identify: {
  [@js.sum "kindName"]
  type nodeKind =
    | [@js.arg "node"] TypeAliasDeclaration(TypeDeclaration.t)
    | [@js.arg "node"] Identifier(Identifier.t)
    | [@js.arg "node"] StringKeyword(StringKeyword.t)
    | [@js.default] Unknown(Ojs.t);

  let nodeKind_of_js: Ojs.t => nodeKind;
  let nodeKind_to_js: nodeKind => Ojs.t;

  type genericWithKindName;
  [@js.builder]
  let makeGenericWithKindName:
    (~kindName: string, ~node: Generic.t, unit) => genericWithKindName;
  let genericWithKindName_of_js: Ojs.t => genericWithKindName;
  let genericWithKindName_to_js: genericWithKindName => Ojs.t;
};

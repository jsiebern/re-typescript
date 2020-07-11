let identifyNode = (node: Ts_morph.Node.t) => {
  open Ts_nodes.Identify;
  let node = node |> Ts_nodes.Generic.cast;
  let forIdentification =
    makeGenericWithKindName(~kindName=node#getKindName(), ~node, ());
  nodeKind_of_js(genericWithKindName_to_js(forIdentification));
};

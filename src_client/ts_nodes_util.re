let rec identifyNode = (node: Ts_nodes.Generic.t) => {
  Ts_nodes.Identify.(identifyGenericNode(node));
}
and identifyGenericNode = (node: Ts_nodes.Generic.t) => {
  open Ts_nodes.Identify;
  let forIdentification =
    makeGenericWithKindName(~kindName=node#getKindName(), ~node, ());
  nodeKind_of_js(genericWithKindName_to_js(forIdentification));
};

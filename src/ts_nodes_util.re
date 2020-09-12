let rec identifyNode = (node: Ts_nodes.Generic.t) => {
  Ts_nodes.Identify.(identifyGenericNode(node));
}
and identifyGenericNode = (node: Ts_nodes.Generic.t) => {
  open Ts_nodes.Identify;
  let forIdentification =
    makeGenericWithKindName(~kindName=node#getKindName(), ~node, ());
  nodeKind_of_js(genericWithKindName_to_js(forIdentification));
};

external from_node_kind_child: 'a => Ts_nodes.Generic.t = "%identity";
let unwrap_identified = (node: Ts_nodes.nodeKind) =>
  switch (node) {
  | TypeAliasDeclaration(node) => node |> from_node_kind_child
  | ConditionalType(node) => node |> from_node_kind_child
  | NamespaceDeclaration(node) => node |> from_node_kind_child
  | ModuleDeclaration(node) => node |> from_node_kind_child
  | Identifier(node) => node |> from_node_kind_child
  | SourceFile(node) => node |> from_node_kind_child
  | MappedType(node) => node |> from_node_kind_child
  | TypeParameter(node) => node |> from_node_kind_child
  | NamedDeclaration(node) => node |> from_node_kind_child
  | HeritageClause(node) => node |> from_node_kind_child
  | TypeElement(node) => node |> from_node_kind_child
  | StringKeyword(node) => node |> from_node_kind_child
  | BooleanLiteral(node) => node |> from_node_kind_child
  | BigIntLiteral(node) => node |> from_node_kind_child
  | RegularExpressionLiteral(node) => node |> from_node_kind_child
  | NullLiteral(node) => node |> from_node_kind_child
  | NumberKeyword(node) => node |> from_node_kind_child
  | NeverKeyword(node) => node |> from_node_kind_child
  | ObjectKeyword(node) => node |> from_node_kind_child
  | UndefinedKeyword(node) => node |> from_node_kind_child
  | BooleanKeyword(node) => node |> from_node_kind_child
  | FalseKeyword(node) => node |> from_node_kind_child
  | TrueKeyword(node) => node |> from_node_kind_child
  | VoidKeyword(node) => node |> from_node_kind_child
  | AnyKeyword(node) => node |> from_node_kind_child
  | NullKeyword(node) => node |> from_node_kind_child
  | VariableDeclarationList(node) => node |> from_node_kind_child
  | VariableDeclaration(node) => node |> from_node_kind_child
  | TypeLiteral(node) => node |> from_node_kind_child
  | LiteralType(node) => node |> from_node_kind_child
  | PropertySignature(node) => node |> from_node_kind_child
  | TypeOperator(node) => node |> from_node_kind_child
  | NodeWithTypeArguments(node) => node |> from_node_kind_child
  | TypeReference(node) => node |> from_node_kind_child
  | QualifiedName(node) => node |> from_node_kind_child
  | EnumDeclaration(node) => node |> from_node_kind_child
  | EnumMember(node) => node |> from_node_kind_child
  | UnionType(node) => node |> from_node_kind_child
  | TupleType(node) => node |> from_node_kind_child
  | NamedTupleMember(node) => node |> from_node_kind_child
  | RestType(node) => node |> from_node_kind_child
  | OptionalType(node) => node |> from_node_kind_child
  | StringLiteral(node) => node |> from_node_kind_child
  | NumericLiteral(node) => node |> from_node_kind_child
  | CallSignatureDeclaration(node) => node |> from_node_kind_child
  | IndexSignatureDeclaration(node) => node |> from_node_kind_child
  | ConstructSignatureDeclaration(node) => node |> from_node_kind_child
  | SignatureDeclarationBase(node) => node |> from_node_kind_child
  | FunctionType(node) => node |> from_node_kind_child
  | FunctionLikeDeclarationBase(node) => node |> from_node_kind_child
  | ArrayType(node) => node |> from_node_kind_child
  | FunctionDeclaration(node) => node |> from_node_kind_child
  | MethodSignature(node) => node |> from_node_kind_child
  | IndexSignature(node) => node |> from_node_kind_child
  | CallSignature(node) => node |> from_node_kind_child
  | Parameter(node) => node |> from_node_kind_child
  | BindingElement(node) => node |> from_node_kind_child
  | PropertyDeclaration(node) => node |> from_node_kind_child
  | IntersectionType(node) => node |> from_node_kind_child
  | IndexedAccessType(node) => node |> from_node_kind_child
  | ParenthesizedType(node) => node |> from_node_kind_child
  | LiteralLikeNode(node) => node |> from_node_kind_child
  | InterfaceDeclaration(node) => node |> from_node_kind_child
  | Other(node) => node |> from_node_kind_child
  };

module Type = {
  let has_flag = (t: Ts_nodes.Type.t, f: Ts_nodes.Type.flags) =>
    Int.logand(t#getFlags(), Ojs.int_of_js(Ts_nodes.Type.flags_to_js(f)))
    > 0;
  let has_object_flag = (t: Ts_nodes.Type.t, f: Ts_nodes.Type.object_flags) =>
    Int.logand(
      t#getObjectFlags(),
      Ojs.int_of_js(Ts_nodes.Type.object_flags_to_js(f)),
    )
    > 0;
};

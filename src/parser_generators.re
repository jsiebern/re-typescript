open Ast;
open Node;
open Parser_utils;

let generate_string_literal_list = (strings: array(string)) => {
  Variant(
    strings
    |> CCArray.map(name =>
         {
           VariantConstructor.name: Identifier.VariantIdentifier(name),
           arguments: [||],
         }
       ),
  );
};

let generate_number_literal_list = (floats: array(float)) => {
  Variant(
    floats
    |> CCArray.map(num =>
         {
           VariantConstructor.name:
             Identifier.VariantIdentifier(Printf.sprintf("%.0f", num)),
           arguments: [||],
         }
       ),
  );
};

let generate_mixed_literal_list =
    (literals: array(Node.node(Node.Constraint.exactlyLiteral))) => {
  // TODO: Implement this as inline module
  Basic(Any);
};

let generate_intersection_body =
    (types: array(Node.node(Node.Constraint.assignable))) =>
  Tuple(types);

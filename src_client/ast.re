[@ocaml.warning "-30"];

module rec Node_id: {
  type t = pri int;
  let make: int => t;
} = {
  type t = int;
  let make = v => v;
}
and Identifier: {
  type path = array(t)
  and t =
    | Module(string)
    | TypeName(string)
    | SubName(string)
    | SubIdent(int);
} = Identifier
and Project: {type t = array(Node.node(Node.Constraint.exactlySourceFile));} = Project
and TypeDeclaration: {
  type t = {
    path: array(Identifier.t),
    extracted_nodes:
      array(Node.node(Node.Constraint.exactlyExtractedReference)),
    name: Identifier.t,
    annot: Node.node(Node.Constraint.assignable),
    params: array(Node.node(Node.Constraint.exactlyTypeParameter)),
  };
} = TypeDeclaration
and Node: {
  module Constraint: {
    type exactlySourceFile = [ | `SourceFile];
    type atLeastSourceFile('a) = [> | `SourceFile] as 'a;

    type exactlyLiteral = [ | `Literal];
    type atLeastLiteral('a) = [> | `Literal] as 'a;

    type exactlyBasic = [ | `Basic];
    type atLeastBasic('a) = [> | `Basic] as 'a;

    type exactlyTypeDeclaration = [ | `TypeDeclaration];
    type atLeastTypeDeclaration('a) = [> | `TypeDeclaration] as 'a;

    type exactlyArray = [ | `Array];
    type atLeastArray('a) = [> | `Array] as 'a;

    type exactlyOptional = [ | `Optional];
    type atLeastOptional('a) = [> | `Optional] as 'a;

    type exactlyNullable = [ | `Nullable];
    type atLeastNullable('a) = [> | `Nullable] as 'a;

    type exactlyReference = [ | `Reference];
    type atLeastReference('a) = [> | `Reference] as 'a;

    type exactlyExtractedReference = [ | `ExtractedReference];
    type atLeastExtractedReference('a) = [> | `ExtractedReference] as 'a;

    type exactlyTypeParameter = [ | `TypeParameter];
    type atLeastTypeParameter('a) = [> | `TypeParameter] as 'a;

    type any = [
      exactlyLiteral
      | exactlyBasic
      | exactlyArray
      | exactlyOptional
      | exactlyNullable
      | exactlySourceFile
      | exactlyReference
      | exactlyExtractedReference
      | exactlyTypeDeclaration
    ];

    type assignable = [
      exactlyLiteral
      | exactlyBasic
      | exactlyArray
      | exactlyOptional
      | exactlyNullable
      | exactlyReference
    ];

    module Literal: {
      type exactlyString = [ | `String];
      type atLeastString('a) = [> | `String] as 'a;

      type exactlyNumber = [ | `Number];
      type atLeastNumber('a) = [> | `Number] as 'a;

      type exactlyBoolean = [ | `Boolean];
      type atLeastBoolean('a) = [> | `Boolean] as 'a;

      type any = [ exactlyString | exactlyNumber | exactlyBoolean];
    };

    module Basic: {
      type exactlyString = [ | `String];
      type atLeastString('a) = [> | `String] as 'a;
      type exactlyNumber = [ | `Number];
      type atLeastNumber('a) = [> | `Number] as 'a;
      type exactlyBoolean = [ | `Boolean];
      type atLeastBoolean('a) = [> | `Boolean] as 'a;
      type exactlyVoid = [ | `Void];
      type atLeastVoid('a) = [> | `Void] as 'a;
      type exactlyAny = [ | `Any];
      type atLeastAny('a) = [> | `Any] as 'a;
      type exactlyNull = [ | `Null];
      type atLeastNull('a) = [> | `Null] as 'a;
      type exactlyUndefined = [ | `Undefined];
      type atLeastUndefined('a) = [> | `Undefined] as 'a;
      type exactlyNever = [ | `Never];
      type atLeastNever('a) = [> | `Never] as 'a;
      type exactlyThis = [ | `This];
      type atLeastThis('a) = [> | `This] as 'a;
      type exactlyRelevantKeyword = [ | `RelevantKeyword];
      type atLeastRelevantKeyword('a) = [> | `RelevantKeyword] as 'a;

      type any = [
        exactlyString
        | exactlyNumber
        | exactlyBoolean
        | exactlyVoid
        | exactlyAny
        | exactlyNull
        | exactlyUndefined
        | exactlyNever
        | exactlyThis
        | exactlyRelevantKeyword
      ];
    };
  };

  type node('tag) =
    | Literal(kind_literal(Constraint.Literal.any))
      : node(Constraint.atLeastLiteral('poly))
    | Basic(kind_basic(Constraint.Basic.any))
      : node(Constraint.atLeastBasic('poly))
    | Array(Node.node(Constraint.assignable))
      : node(Constraint.atLeastArray('poly))
    | Optional(Node.node(Constraint.assignable))
      : node(Constraint.atLeastOptional('poly))
    | Nullable(Node.node(Constraint.assignable))
      : node(Constraint.atLeastNullable('poly))
    | SourceFile({
        name: string,
        path: string,
        types: array(Node.node(Constraint.exactlyTypeDeclaration)),
      })
      : node(Constraint.atLeastSourceFile('poly))
    | Reference: node(Constraint.atLeastReference('poly))
    | ExtractedReference: node(Constraint.atLeastExtractedReference('poly))
    | TypeDeclaration(TypeDeclaration.t)
      : node(Constraint.atLeastTypeDeclaration('poly))
  and kind_basic('tag) =
    | RelevantKeyword(string)
      : kind_basic(Constraint.Basic.atLeastRelevantKeyword('poly))
    | String: kind_basic(Constraint.Basic.atLeastString('poly))
    | Number: kind_basic(Constraint.Basic.atLeastNumber('poly))
    | Boolean: kind_basic(Constraint.Basic.atLeastBoolean('poly))
    | Void: kind_basic(Constraint.Basic.atLeastVoid('poly))
    | Any: kind_basic(Constraint.Basic.atLeastAny('poly))
    | Null: kind_basic(Constraint.Basic.atLeastNull('poly))
    | Undefined: kind_basic(Constraint.Basic.atLeastUndefined('poly))
    | Never: kind_basic(Constraint.Basic.atLeastNever('poly))
    | This: kind_basic(Constraint.Basic.atLeastThis('poly))
  and kind_literal('tag) =
    | String(string): kind_literal(Constraint.Literal.atLeastString('poly))
    | Number(float): kind_literal(Constraint.Literal.atLeastNumber('poly))
    | Boolean(bool): kind_literal(Constraint.Literal.atLeastBoolean('poly));

  module Escape: {
    external toAssignable: node('a) => node(Constraint.assignable) =
      "%identity";
    external toAny: node('a) => node(Constraint.any) = "%identity";
  };
} = Node;

type node_order = array(Identifier.path);
type node_references = Hashtbl.t(Identifier.path, array(Identifier.path));

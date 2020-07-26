[@ocaml.warning "-30"];

module rec Node_id: {
  type t = pri int;
  let make: int => t;
} = {
  type t = int;
  let make = v => v;
}
and Identifier: {
  module Constraint: {
    type exactlyModule = [ | `Module];
    type atLeastModule('a) = [> | `Module] as 'a;
    type exactlyTypeName = [ | `TypeName];
    type atLeastTypeName('a) = [> | `TypeName] as 'a;
    type exactlyPropertyName = [ | `PropertyName];
    type atLeastPropertyName('a) = [> | `PropertyName] as 'a;
    type exactlyVariantIdentifier = [ | `VariantIdentifier];
    type atLeastVariantIdentifier('a) = [> | `VariantIdentifier] as 'a;
    type exactlySubName = [ | `SubName];
    type atLeastSubName('a) = [> | `SubName] as 'a;
    type exactlySubIdent = [ | `SubIdent];
    type atLeastSubIdent('a) = [> | `SubIdent] as 'a;
    type exactlyTypeParameter = [ | `TypeParameter];
    type atLeastTypeParameter('a) = [> | `TypeParameter] as 'a;

    type any = [
      exactlyModule
      | exactlyTypeName
      | exactlyPropertyName
      | exactlyVariantIdentifier
      | exactlySubName
      | exactlySubIdent
      | exactlyTypeParameter
    ];
  };

  type path = array(t(Constraint.any))
  and t('kind) =
    | TypeParameter(string): t(Constraint.atLeastTypeParameter('poly))
    | Module(string): t(Constraint.atLeastModule('poly))
    | TypeName(string): t(Constraint.atLeastTypeName('poly))
    | PropertyName(string): t(Constraint.atLeastPropertyName('poly))
    | VariantIdentifier(string)
      : t(Constraint.atLeastVariantIdentifier('poly))
    | SubName(string): t(Constraint.atLeastSubName('poly))
    | SubIdent(int): t(Constraint.atLeastSubIdent('poly));

  module Escape: {external toAny: t('a) => t(Constraint.any) = "%identity";};
} = Identifier
and Project: {type t = array(Node.node(Node.Constraint.exactlyModule));} = Project
and TypeDeclaration: {
  type t = {
    path: array(Identifier.t(Identifier.Constraint.any)),
    name: Identifier.t(Identifier.Constraint.exactlyTypeName),
    annot: Node.node(Node.Constraint.assignable),
    params: array(Identifier.t(Identifier.Constraint.exactlyTypeParameter)),
  };
} = TypeDeclaration
and VariantConstructor: {
  type t = {
    name: Identifier.t(Identifier.Constraint.exactlyVariantIdentifier),
    arguments: array(Node.node(Node.Constraint.assignable)),
  };
} = VariantConstructor
and Node: {
  module Constraint: {
    type exactlyModule = [ | `Module];
    type atLeastModule('a) = [> | `Module] as 'a;
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
    type exactlyVariant = [ | `Variant];
    type atLeastVariant('a) = [> | `Variant] as 'a;
    type exactlyFixture = [ | `Fixture];
    type atLeastFixture('a) = [> | `Fixture] as 'a;
    type exactlyTuple = [ | `Tuple];
    type atLeastTuple('a) = [> | `Tuple] as 'a;
    type exactlyFunction = [ | `Function];
    type atLeastFunction('a) = [> | `Function] as 'a;
    type exactlyParameter = [ | `Paramter];
    type atLeastParameter('a) = [> | `Paramter] as 'a;
    type exactlyRecord = [ | `Record];
    type atLeastRecord('a) = [> | `Record] as 'a;
    type exactlyGenericReference = [ | `GenericReference];
    type atLeastGenericReference('a) = [> | `GenericReference] as 'a;
    type exactlySafeDict = [ | `SafeDict];
    type atLeastSafeDict('a) = [> | `SafeDict] as 'a;

    type any = [
      exactlyLiteral
      | exactlyVariant
      | exactlyBasic
      | exactlyArray
      | exactlyOptional
      | exactlyNullable
      | exactlyModule
      | exactlyReference
      | exactlyTypeDeclaration
      | exactlyFixture
      | exactlyTuple
      | exactlyFunction
      | exactlyParameter
      | exactlyRecord
      | exactlyGenericReference
      | exactlySafeDict
    ];

    type assignable = [
      exactlyLiteral
      | exactlyBasic
      | exactlyArray
      | exactlyOptional
      | exactlyNullable
      | exactlyReference
      | exactlyVariant
      | exactlyTuple
      | exactlyFunction
      | exactlyRecord
      | exactlyGenericReference
      | exactlySafeDict
    ];

    type moduleLevel = [
      exactlyTypeDeclaration
      | exactlyFixture
      | exactlyModule
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

    module Fixture: {
      type exactlyAnyUnboxed = [ | `AnyUnboxed];
      type atLeastAnyUnboxed('a) = [> | `AnyUnboxed] as 'a;
      type exactlyTUnboxed = [ | `TUnboxed];
      type atLeastTUnboxed('a) = [> | `TUnboxed] as 'a;

      type any = [ exactlyAnyUnboxed | exactlyTUnboxed];
    };
  };

  type node('tag) =
    | Parameter({
        name: Identifier.t(Identifier.Constraint.exactlyPropertyName),
        is_optional: bool,
        type_: node(Constraint.assignable),
        named: bool,
      })
      : node(Constraint.atLeastParameter('poly))
    | Function({
        parameters: array(node(Constraint.exactlyParameter)),
        return_type: node(Constraint.assignable),
      })
      : node(Constraint.atLeastFunction('poly))
    | SafeDict(node(Constraint.assignable))
      : node(Constraint.atLeastSafeDict('poly))
    | Record(array(node(Constraint.exactlyParameter)))
      : node(Constraint.atLeastRecord('poly))
    | Variant(array(VariantConstructor.t))
      : node(Constraint.atLeastVariant('poly))
    | Tuple(array(node(Constraint.assignable)))
      : node(Constraint.atLeastTuple('poly))
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
    | Module({
        name: string,
        path: string,
        types: array(Node.node(Constraint.moduleLevel)),
      })
      : node(Constraint.atLeastModule('poly))
    | Reference({
        target: Identifier.path,
        params: list((string, node(Constraint.assignable))),
      })
      : node(Constraint.atLeastReference('poly))
    | GenericReference(
        Identifier.t(Identifier.Constraint.exactlyTypeParameter),
      )
      : node(Constraint.atLeastGenericReference('poly))
    | TypeDeclaration(TypeDeclaration.t)
      : node(Constraint.atLeastTypeDeclaration('poly))
    | Fixture(kind_fixture(Constraint.Fixture.any))
      : node(Constraint.atLeastFixture('poly))
  and kind_fixture('tag) =
    | AnyUnboxed: kind_fixture(Constraint.Fixture.atLeastAnyUnboxed('poly))
    | TUnboxed: kind_fixture(Constraint.Fixture.atLeastTUnboxed('poly))
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

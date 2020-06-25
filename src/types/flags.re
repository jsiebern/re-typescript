module IntShift = {
  let (&) = Int.logand;
  let (||) = Int.logor;
  let (<<) = Int.shift_left;
  let (>>) = Int.shift_right;
  let (>>>) = Int.shift_right_logical;
  let (~~) = Int.lognot;
  let (&&) = (v1, v2) => (v1 & v2) > 0;

  type t = Int.t;
};

module Type: {
  type t = pri Int.t;

  let (&): (t, t) => t;
  let (||): (t, t) => t;
  let (<<): (t, t) => t;
  let (>>): (t, t) => t;
  let (>>>): (t, t) => t;
  let (~~): t => t;
  let (&&): (t, t) => bool;

  let any: t;
  let unknown: t;
  let string: t;
  let number: t;
  let boolean: t;
  let enum: t;
  let bigInt: t;
  let stringLiteral: t;
  let numberLiteral: t;
  let booleanLiteral: t;
  let enumLiteral: t;
  let bigIntLiteral: t;
  let eSSymbol: t;
  let uniqueESSymbol: t;
  let void: t;
  let undefined: t;
  let null: t;
  let never: t;
  let typeParameter: t;
  let object_: t;
  let union: t;
  let intersection: t;
  let index: t;
  let indexedAccess: t;
  let conditional: t;
  let substitution: t;
  let nonPrimitive: t;

  let anyOrUnknown: t;
  let nullable: t;
  let literal: t;
  let unit: t;
  let stringOrNumberLiteral: t;
  let stringOrNumberLiteralOrUnique: t;
  let definitelyFalsy: t;
  let possiblyFalsy: t;
  let intrinsic: t;
  let primitive: t;
  let stringLike: t;
  let numberLike: t;
  let bigIntLike: t;
  let booleanLike: t;
  let enumLike: t;
  let eSSymbolLike: t;
  let voidLike: t;
  let disjointDomains: t;
  let unionOrIntersection: t;
  let structuredType: t;
  let typeVariable: t;
  let instantiableNonPrimitive: t;
  let instantiablePrimitive: t;
  let instantiable: t;
  let structuredOrInstantiable: t;
  let objectFlagsType: t;
  let simplifiable: t;
  let substructure: t;
  let narrowable: t;
  let notUnionOrUnit: t;
  let notPrimitiveUnion: t;
  let includesMask: t;
  let includesStructuredOrInstantiable: t;
  let includesNonWideningType: t;
  let includesWildcard: t;
  let includesEmptyObject: t;
} = {
  include IntShift;
  type t = Int.t;

  let any = 1 << 0;
  let unknown = 1 << 1;
  let string = 1 << 2;
  let number = 1 << 3;
  let boolean = 1 << 4;
  let enum = 1 << 5;
  let bigInt = 1 << 6;
  let stringLiteral = 1 << 7;
  let numberLiteral = 1 << 8;
  let booleanLiteral = 1 << 9;
  let enumLiteral = 1 << 10; // Always combined with StringLiteral, NumberLiteral, or Union
  let bigIntLiteral = 1 << 11;
  let eSSymbol = 1 << 12; // Type of symbol primitive introduced in ES6
  let uniqueESSymbol = 1 << 13; // unique symbol
  let void = 1 << 14;
  let undefined = 1 << 15;
  let null = 1 << 16;
  let never = 1 << 17; // Never type
  let typeParameter = 1 << 18; // Type parameter
  let object_ = 1 << 19; // Object type
  let union = 1 << 20; // Union (T || U)
  let intersection = 1 << 21; // Intersection (T & U)
  let index = 1 << 22; // keyof T
  let indexedAccess = 1 << 23; // T[K]
  let conditional = 1 << 24; // T extends U ? X : Y
  let substitution = 1 << 25; // Type parameter substitution
  let nonPrimitive = 1 << 26; // intrinsic object type

  let anyOrUnknown = any || unknown;
  let nullable = undefined || null;
  let literal =
    stringLiteral || numberLiteral || bigIntLiteral || booleanLiteral;
  let unit = literal || uniqueESSymbol || nullable;
  let stringOrNumberLiteral = stringLiteral || numberLiteral;
  let stringOrNumberLiteralOrUnique =
    stringLiteral || numberLiteral || uniqueESSymbol;
  let definitelyFalsy =
    stringLiteral
    || numberLiteral
    || bigIntLiteral
    || booleanLiteral
    || void
    || undefined
    || null;
  let possiblyFalsy = definitelyFalsy || string || number || bigInt || boolean;
  let intrinsic =
    any
    || unknown
    || string
    || number
    || bigInt
    || boolean
    || booleanLiteral
    || eSSymbol
    || void
    || undefined
    || null
    || never
    || nonPrimitive;
  let primitive =
    string
    || number
    || bigInt
    || boolean
    || enum
    || enumLiteral
    || eSSymbol
    || void
    || undefined
    || null
    || literal
    || uniqueESSymbol;
  let stringLike = string || stringLiteral;
  let numberLike = number || numberLiteral || enum;
  let bigIntLike = bigInt || bigIntLiteral;
  let booleanLike = boolean || booleanLiteral;
  let enumLike = enum || enumLiteral;
  let eSSymbolLike = eSSymbol || uniqueESSymbol;
  let voidLike = void || undefined;
  let disjointDomains =
    nonPrimitive
    || stringLike
    || numberLike
    || bigIntLike
    || booleanLike
    || eSSymbolLike
    || voidLike
    || null;
  let unionOrIntersection = union || intersection;
  let structuredType = object_ || union || intersection;
  let typeVariable = typeParameter || indexedAccess;
  let instantiableNonPrimitive = typeVariable || conditional || substitution;
  let instantiablePrimitive = index;
  let instantiable = instantiableNonPrimitive || instantiablePrimitive;
  let structuredOrInstantiable = structuredType || instantiable;
  let objectFlagsType =
    any || nullable || never || object_ || union || intersection;
  let simplifiable = indexedAccess || conditional;
  let substructure =
    object_
    || union
    || intersection
    || index
    || indexedAccess
    || conditional
    || substitution;
  let narrowable =
    any
    || unknown
    || structuredOrInstantiable
    || stringLike
    || numberLike
    || bigIntLike
    || booleanLike
    || eSSymbol
    || uniqueESSymbol
    || nonPrimitive;
  let notUnionOrUnit = any || unknown || eSSymbol || object_ || nonPrimitive;
  let notPrimitiveUnion =
    any || unknown || enum || void || never || structuredOrInstantiable;
  let includesMask =
    any
    || unknown
    || primitive
    || never
    || object_
    || union
    || intersection
    || nonPrimitive;
  let includesStructuredOrInstantiable = typeParameter;
  let includesNonWideningType = index;
  let includesWildcard = indexedAccess;
  let includesEmptyObject = conditional;
};

module Symbol: {
  type t = pri int;

  let (&): (t, t) => t;
  let (||): (t, t) => t;
  let (<<): (t, t) => t;
  let (>>): (t, t) => t;
  let (>>>): (t, t) => t;
  let (~~): t => t;
  let (&&): (t, t) => bool;

  let none: t;
  let functionScopedVariable: t;
  let blockScopedVariable: t;
  let property: t;
  let enumMember: t;
  let function_: t;
  let class_: t;
  let interface: t;
  let constEnum: t;
  let regularEnum: t;
  let valueModule: t;
  let namespaceModule: t;
  let typeLiteral: t;
  let objectLiteral: t;
  let method: t;
  let constructor: t;
  let getAccessor: t;
  let setAccessor: t;
  let signature: t;
  let typeParameter: t;
  let typeAlias: t;
  let exportValue: t;
  let alias: t;
  let prototype: t;
  let exportStar: t;
  let optional: t;
  let transient: t;
  let assignment: t;
  let moduleExports: t;
  let deprecated: t;

  let all: t;
  let enum: t;
  let variable: t;
  let value: t;
  let type_: t;
  let namespace: t;
  let module_: t;
  let accessor: t;
  let functionScopedVariableExcludes: t;
  let blockScopedVariableExcludes: t;
  let parameterExcludes: t;
  let propertyExcludes: t;
  let enumMemberExcludes: t;
  let functionExcludes: t;
  let class_Excludes: t;
  let interfaceExcludes: t;
  let regularEnumExcludes: t;
  let constEnumExcludes: t;
  let valueModuleExcludes: t;
  let namespaceModuleExcludes: t;
  let methodExcludes: t;
  let getAccessorExcludes: t;
  let setAccessorExcludes: t;
  let typeParameterExcludes: t;
  let typeAliasExcludes: t;
  let aliasExcludes: t;
  let moduleMember: t;
  let exportHasLocal: t;
  let blockScoped: t;
  let propertyOrAccessor: t;
  let class_Member: t;
  let exportSupportsDefaultModifier: t;
  let exportDoesNotSupportDefaultModifier: t;
  let class_ifiable: t;
  let lateBindingContainer: t;
} = {
  include IntShift;

  type t = Int.t;

  let none = 0;
  let functionScopedVariable = 1 << 0; // Variable (var) or parameter
  let blockScopedVariable = 1 << 1; // A block-scoped variable (let or const)
  let property = 1 << 2; // Property or enum member
  let enumMember = 1 << 3; // Enum member
  let function_ = 1 << 4; // Function
  let class_ = 1 << 5; // Class
  let interface = 1 << 6; // Interface
  let constEnum = 1 << 7; // Const enum
  let regularEnum = 1 << 8; // Enum
  let valueModule = 1 << 9; // Instantiated module
  let namespaceModule = 1 << 10; // Uninstantiated module
  let typeLiteral = 1 << 11; // Type Literal or mapped type
  let objectLiteral = 1 << 12; // Object Literal
  let method = 1 << 13; // Method
  let constructor = 1 << 14; // Constructor
  let getAccessor = 1 << 15; // Get accessor
  let setAccessor = 1 << 16; // Set accessor
  let signature = 1 << 17; // Call, construct, or index signature
  let typeParameter = 1 << 18; // Type parameter
  let typeAlias = 1 << 19; // Type alias
  let exportValue = 1 << 20; // Exported value marker (see comment in declareModuleMember in binder)
  let alias = 1 << 21; // An alias for another symbol (see comment in isAliasSymbolDeclaration in checker)
  let prototype = 1 << 22; // Prototype property (no source representation)
  let exportStar = 1 << 23; // Export * declaration
  let optional = 1 << 24; // Optional property
  let transient = 1 << 25; // Transient symbol (created during type check)
  let assignment = 1 << 26; // Assignment treated as declaration (eg `this.prop = 1`)
  let moduleExports = 1 << 27; // Symbol for CommonJS `module` of `module.exports`
  let deprecated = 1 << 28; // Symbol has Deprecated declaration tag (eg `@deprecated`)

  let all =
    functionScopedVariable
    || blockScopedVariable
    || property
    || enumMember
    || function_
    || class_
    || interface
    || constEnum
    || regularEnum
    || valueModule
    || namespaceModule
    || typeLiteral
    || objectLiteral
    || method
    || constructor
    || getAccessor
    || setAccessor
    || signature
    || typeParameter
    || typeAlias
    || exportValue
    || alias
    || prototype
    || exportStar
    || optional
    || transient
    || deprecated;
  let enum = regularEnum || constEnum;
  let variable = functionScopedVariable || blockScopedVariable;
  let value =
    variable
    || property
    || enumMember
    || objectLiteral
    || function_
    || class_
    || enum
    || valueModule
    || method
    || getAccessor
    || setAccessor;
  let type_ =
    class_
    || interface
    || enum
    || enumMember
    || typeLiteral
    || typeParameter
    || typeAlias;
  let namespace = valueModule || namespaceModule || enum;
  let module_ = valueModule || namespaceModule;
  let accessor = getAccessor || setAccessor;
  let functionScopedVariableExcludes = value & ~~functionScopedVariable;
  let blockScopedVariableExcludes = value;
  let parameterExcludes = value;
  let propertyExcludes = none;
  let enumMemberExcludes = value || type_;
  let functionExcludes = value & ~~(function_ || valueModule || class_);
  let class_Excludes =
    (value || type_) & ~~(valueModule || interface || function_);
  let interfaceExcludes = type_ & ~~(interface || class_);
  let regularEnumExcludes = (value || type_) & ~~(regularEnum || valueModule);
  let constEnumExcludes = (value || type_) & ~~constEnum;
  let valueModuleExcludes =
    value & ~~(function_ || class_ || regularEnum || valueModule);
  let namespaceModuleExcludes = 0;
  let methodExcludes = value & ~~method;
  let getAccessorExcludes = value & ~~setAccessor;
  let setAccessorExcludes = value & ~~getAccessor;
  let typeParameterExcludes = type_ & ~~typeParameter;
  let typeAliasExcludes = type_;
  let aliasExcludes = alias;
  let moduleMember =
    variable
    || function_
    || class_
    || interface
    || enum
    || module_
    || typeAlias
    || alias;
  let exportHasLocal = function_ || class_ || enum || valueModule;
  let blockScoped = blockScopedVariable || class_ || enum;
  let propertyOrAccessor = property || accessor;
  let class_Member = method || accessor || property;
  let exportSupportsDefaultModifier = class_ || function_ || interface;
  let exportDoesNotSupportDefaultModifier = ~~exportSupportsDefaultModifier;
  let class_ifiable =
    class_
    || enum
    || typeAlias
    || interface
    || typeParameter
    || module_
    || alias;
  let lateBindingContainer =
    class_ || interface || typeLiteral || objectLiteral || function_;
};

module Node: {
  type t = pri Int.t;

  let (&): (t, t) => t;
  let (||): (t, t) => t;
  let (<<): (t, t) => t;
  let (>>): (t, t) => t;
  let (>>>): (t, t) => t;
  let (~~): t => t;
  let (&&): (t, t) => bool;

  let none: t;
  let let_: t;
  let const: t;

  let blockScoped: t;
} = {
  include IntShift;

  let none = 0;
  let let_ = 1 << 0;
  let const = 1 << 1;

  let blockScoped = let_ || const;
};

module Modifier: {
  type t = pri Int.t;

  let (&): (t, t) => t;
  let (||): (t, t) => t;
  let (<<): (t, t) => t;
  let (>>): (t, t) => t;
  let (>>>): (t, t) => t;
  let (~~): t => t;
  let (&&): (t, t) => bool;

  let none: t;
  let export: t;
  let ambient: t;
  let public: t;
  let private: t;
  let protected: t;
  let static: t;
  let readonly: t;
  let abstract: t;
  let async: t;
  let default: t;
  let const: t;
  let hasComputedJSDocModifiers: t;
  let deprecated: t;
  let hasComputedFlags: t;

  let accessibilityModifier: t;
  let parameterPropertyModifier: t;
  let nonPublicAccessibilityModifier: t;

  let typeScriptModifier: t;
  let exportDefault: t;
  let all: t;
} = {
  include IntShift;

  let none = 0;
  let export = 1 << 0; // Declarations
  let ambient = 1 << 1; // Declarations
  let public = 1 << 2; // Property/Method
  let private = 1 << 3; // Property/Method
  let protected = 1 << 4; // Property/Method
  let static = 1 << 5; // Property/Method
  let readonly = 1 << 6; // Property/Method
  let abstract = 1 << 7; // Class/Method/ConstructSignature
  let async = 1 << 8; // Property/Method/Function
  let default = 1 << 9; // Function/Class (export default declaration)
  let const = 1 << 11; // Const enum
  let hasComputedJSDocModifiers = 1 << 12; // Indicates the computed modifier flags include modifiers from JSDoc.
  let deprecated = 1 << 13; // Deprecated tag.
  let hasComputedFlags = 1 << 29; // Modifier flags have been computed

  let accessibilityModifier = public || private || protected;
  let parameterPropertyModifier = accessibilityModifier || readonly;
  let nonPublicAccessibilityModifier = private || protected;

  let typeScriptModifier =
    ambient || public || private || protected || readonly || abstract || const;
  let exportDefault = export || default;
  let all =
    export
    || ambient
    || public
    || private
    || protected
    || static
    || readonly
    || abstract
    || async
    || default
    || const
    || deprecated;
};

module Type: {
  class t:
    (Ojs.t) =>
    {
      inherit Ojs.obj;
      pub getText: unit => string;
      // Type getters
      pub getApparentType: unit => option(t);
      pub getArrayElementType: unit => option(t);
      pub getBaseTypeOfLiteralType: unit => option(t);
      pub getConstraint: unit => option(t);
      pub getDefault: unit => option(t);
      pub getNonNullableType: unit => option(t);
      pub getNumberIndexType: unit => option(t);
      pub getStringIndexType: unit => option(t);
      pub getTargetType: unit => option(t);
      pub getAliasTypeArguments: unit => option(array(t));
      pub getBaseTypes: unit => option(array(t));
      pub getIntersectionTypes: unit => option(array(t));
      pub getTupleElements: unit => option(array(t));
      pub getTypeArguments: unit => option(array(t));
      pub getUnionTypes: unit => option(array(t));
      // Type Checks
      pub isAny: unit => bool;
      pub isArray: unit => bool;
      pub isBoolean: unit => bool;
      pub isString: unit => bool;
      pub isNumber: unit => bool;
      pub isLiteral: unit => bool;
      pub isBooleanLiteral: unit => bool;
      pub isUnion: unit => bool;
      pub isEnumLiteral: unit => bool;
      pub isNumberLiteral: unit => bool;
      pub isStringLiteral: unit => bool;
      pub isClass: unit => bool;
      pub isClassOrInterface: unit => bool;
      pub isInterface: unit => bool;
      pub isObject: unit => bool;
      pub isEnum: unit => bool;
      pub isTypeParameter: unit => bool;
      pub isTuple: unit => bool;
      pub isIntersection: unit => bool;
      pub isUnionOrIntersection: unit => bool;
      pub isUnknown: unit => bool;
      pub isNull: unit => bool;
      pub isUndefined: unit => bool;
      pub isAnonymous: unit => bool;
      // Raw
      pub compilerType: Ts_raw.Type.t;
    };

  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};

module Node: {
  class t:
    (Ojs.t) =>
    {
      inherit Ojs.obj;
      // Type checks
      pub isSourceFile: unit => bool;
      // Tree Traversal
      pub forEachDescendant: (t => unit) => unit;
      pub forEachChild: (t => unit) => unit;
      pub getChildren: unit => array(t);
      pub getDescendants: unit => array(t);
      pub getExportedDeclarations: unit => array(t);
      // Type Information
      pub getTypeParameters: unit => array(t);
      pub getText: unit => string;
      pub getKind: unit => int;
      pub getPos: unit => int;
      pub getEnd: unit => int;
      pub getStartLineNumber: unit => int;
      pub getEndLineNumber: unit => int;
      pub getKindName: unit => string;
      pub getType: unit => option(Type.t);
      // Raw
      pub compilerNode: Ts_raw.Node.t;
    };

  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};

module SourceFile: {
  class t:
    (Ojs.t) =>
    {
      inherit Node.t;
      pub saveSync: unit => unit;
      pub refreshFromFileSystemSync: unit => unit;
      pub getFilePath: unit => string;
      pub compilerNode: Ts_raw.Node.t;
      pub getBaseName: unit => string;
      pub getBaseNameWithoutExtension: unit => string;
    };
  [@js.cast]
  let castToNode: t => Node.t;

  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};

module Symbol: {
  class t:
    (Ojs.t) =>
    {
      inherit Ojs.obj;
      // Type Information
      pub getDeclaredType: unit => option(Type.t);
      pub getName: unit => string;
      pub getFullyQualifiedName: unit => string;
      pub getDeclarations: unit => array(Node.t);
      // Raw
      pub compilerSymbol: Ts_raw.Symbol.t;
    };

  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};

module FileSystem: {
  class t:
    (Ojs.t) =>
    {
      inherit Ojs.obj;
      pub readFileSync: string => string;
      pub writeFileSync: (string, string) => unit;
    };

  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};

module TypeChecker: {
  class t:
    (Ojs.t) =>
    {
      inherit Ojs.obj;
      pub getSymbolAtLocation: Node.t => option(Symbol.t);
      pub getAliasedSymbol: Symbol.t => option(Symbol.t);
      // Raw
      pub compilerObject: Ts_raw.TypeChecker.t;
    };

  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};

module Project: {
  module Config: {
    module CompilerOptions: {
      type t;
      [@js.builder]
      let make:
        (
          ~lib: array(string)=?,
          ~noLib: bool=?,
          ~importHelpers: bool=?,
          unit
        ) =>
        t;
    };

    type t;
    [@js.builder]
    let make:
      (
        ~compilerOptions: CompilerOptions.t=?,
        ~useInMemoryFileSystem: bool,
        unit
      ) =>
      t;
  };

  type diagnostic;
  class t:
    (Ojs.t) =>
    {
      inherit Ojs.obj;
      // Source Files
      pub createSourceFile: (string, string) => SourceFile.t;
      pub removeSourceFile: SourceFile.t => unit;
      pub getSourceFile: string => option(SourceFile.t);
      pub getSourceFiles: unit => array(SourceFile.t);
      pub saveSync: unit => unit;
      // Tools
      pub getFileSystem: unit => FileSystem.t;
      pub getTypeChecker: unit => TypeChecker.t;
      // Diagnostics
      pub getPreEmitDiagnostics: unit => array(diagnostic);
      pub formatDiagnosticsWithColorAndContext: array(diagnostic) => string;
    };

  [@js.new "project"]
  let make: Config.t => t;

  let t_of_js: Ojs.t => t;
  let t_to_js: t => Ojs.t;
};

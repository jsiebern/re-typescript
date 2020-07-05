module TypeChecker = {
  type t

  @bs.get external compilerObject: t => Ts_Typechecker.t = "compilerObject"
}

module Type = {
  type t

  @bs.set external setKindName: (t, string) => unit = "kindName"

  @bs.get external compilerType: t => Typescript_raw.type_ = "compilerType"
  @bs.send external getText: t => string = "getText"

  @bs.send external getApparentType: t => option<t> = "getApparentType"
  @bs.send
  external getArrayElementType: t => option<t> = "getArrayElementType"
  @bs.send
  external getBaseTypeOfLiteralType: t => option<t> = "getBaseTypeOfLiteralType"
  @bs.send external getConstraint: t => option<t> = "getConstraint"
  @bs.send external getDefault: t => option<t> = "getDefault"
  @bs.send
  external getNonNullableType: t => option<t> = "getNonNullableType"
  @bs.send
  external getNumberIndexType: t => option<t> = "getNumberIndexType"
  @bs.send
  external getStringIndexType: t => option<t> = "getStringIndexType"
  @bs.send external getTargetType: t => option<t> = "getTargetType"
  @bs.send
  external getAliasTypeArguments: t => option<array<t>> = "getAliasTypeArguments"
  @bs.send external getBaseTypes: t => option<array<t>> = "getBaseTypes"
  @bs.send
  external getIntersectionTypes: t => option<array<t>> = "getIntersectionTypes"
  @bs.send
  external getTupleElements: t => option<array<t>> = "getTupleElements"
  @bs.send
  external getTypeArguments: t => option<array<t>> = "getTypeArguments"
  @bs.send external getUnionTypes: t => option<array<t>> = "getUnionTypes"

  @bs.send external isAny: t => bool = "isAny"
  @bs.send external isArray: t => bool = "isArray"
  @bs.send external isBoolean: t => bool = "isBoolean"
  @bs.send external isString: t => bool = "isString"
  @bs.send external isNumber: t => bool = "isNumber"
  @bs.send external isLiteral: t => bool = "isLiteral"
  @bs.send external isBooleanLiteral: t => bool = "isBooleanLiteral"
  @bs.send external isUnion: t => bool = "isUnion"
  @bs.send external isEnumLiteral: t => bool = "isEnumLiteral"
  @bs.send external isNumberLiteral: t => bool = "isNumberLiteral"
  @bs.send external isStringLiteral: t => bool = "isStringLiteral"
  @bs.send external isClass: t => bool = "isClass"
  @bs.send external isClassOrInterface: t => bool = "isClassOrInterface"
  @bs.send external isInterface: t => bool = "isInterface"
  @bs.send external isObject: t => bool = "isObject"
  @bs.send external isEnum: t => bool = "isEnum"
  @bs.send external isTypeParameter: t => bool = "isTypeParameter"
  @bs.send external isTuple: t => bool = "isTuple"
  @bs.send external isIntersection: t => bool = "isIntersection"
  @bs.send
  external isUnionOrIntersection: t => bool = "isUnionOrIntersection"
  @bs.send external isUnknown: t => bool = "isUnknown"
  @bs.send external isNull: t => bool = "isNull"
  @bs.send external isUndefined: t => bool = "isUndefined"
  @bs.send external isAnonymous: t => bool = "isAnonymous"
}

module Symbol = {
  type t

  @bs.set external setResolvedType: (t, Type.t) => unit = "resolvedType"

  @bs.send
  external getDeclaredType: t => option<Type.t> = "getDeclaredType"
  @bs.get
  external compilerSymbol: t => Typescript_raw.symbol = "compilerSymbol"
  @bs.send external getName: t => string = "getName"
  @bs.send
  external getFullyQualifiedName: t => string = "getFullyQualifiedName"
}

module Node = {
  type t
  @bs.val external isSourceFile: t => bool = "isSourceFile"

  @bs.set
  external setResolvedSymbol: (t, Symbol.t) => unit = "resolvedSymbol"
  @bs.set external setKindName: (t, string) => unit = "kindName"

  @bs.send external getType: t => option<Type.t> = "getType"
  @bs.send external getSymbol: t => option<Symbol.t> = "getSymbol"
  @bs.send external getKindName: t => string = "getKindName"
  @bs.send external getKind: t => int = "getKind"
  @bs.send external getPos: t => int = "getPos"
  @bs.send external getEnd: t => int = "getEnd"
  @bs.send external getStartLineNumber: t => int = "getStartLineNumber"
  @bs.send external getEndLineNumber: t => int = "getEndLineNumber"
  @bs.send external getText: t => string = "getText"
  @bs.send
  external forEachDescendant: (t, (. t) => unit) => unit = "forEachDescendant"
  @bs.send external getChildren: t => array<t> = "getChildren"
  @bs.get external compilerNode: t => Typescript_raw.node = "compilerNode"
}

module SourceFile = {
  type t

  external toJson: t => Js.Json.t = "%identity"
  external toNode: t => Node.t = "%identity"
  @bs.send external saveSync: t => unit = "saveSync"
  @bs.send external refreshFromFileSystemSync: t => unit = "refreshFromFileSystemSync"
  @bs.get external compilerNode: t => Typescript_raw.node = "compilerNode"
  @bs.get external compilerNodeJson: t => Js.Json.t = "compilerNode"
  @bs.send external getFilePath: t => string = "getFilePath";
}

module Diagnostics = {
  type t
}

module FileSystem = {
  type t

  @bs.send external readFileSync: (t, string) => string = "readFileSync"
  @bs.send external writeFileSync: (t, string, string) => unit = "writeFileSync"
}

module Project = {
  type t

  module Config = {
    type compilerOptions = {
      lib: array<string>,
      importHelpers: bool,
      noLib: bool,
    }
    @bs.deriving(abstract)
    type t = {
      @bs.optional
      useInMemoryFileSystem: bool,
      @bs.optional
      compilerOptions: compilerOptions,
    }
    let make = t
  }

  @bs.send
  external createSourceFile: (t, string, string) => SourceFile.t = "createSourceFile"
  @bs.send
  external getPreEmitDiagnostics: t => array<Diagnostics.t> = "getPreEmitDiagnostics"
  @bs.send
  external formatDiagnosticsWithColorAndContext: (t, array<Diagnostics.t>) => string =
    "formatDiagnosticsWithColorAndContext"
  @bs.send
  external getSourceFiles: t => array<SourceFile.t> = "getSourceFiles"
  @bs.send
  external getSourceFile: (t, string) => option<SourceFile.t> = "getSourceFile"
  @bs.send
  external removeSourceFile: (t, SourceFile.t) => unit = "removeSourceFile"
  @bs.send external getTypeChecker: t => TypeChecker.t = "getTypeChecker"
  @bs.send external getFileSystem: t => FileSystem.t = "getFileSystem"
  @bs.send external saveSync: t => unit = "saveSync";

  @bs.new @bs.module("ts-morph")
  external make: option<Config.t> => t = "Project"
}

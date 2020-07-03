open Ts_morph

let getKindNameOfType = (. t: Type.t) =>
  Runtime.typeChecker
  ->Ts_Typechecker.typeToTypeNode(t->Type.compilerType)
  ->Belt.Option.map(Typescript_raw.Node.kind)
  ->Belt.Option.flatMap(id => Typescript_syntaxkind.kindCache->Belt.Map.Int.get(id))
let isTypeReference = (. t: Type.t) => {
  getKindNameOfType(. t) === Some("TypeReference")
}
let isVoid = (. t: Type.t) => {
  getKindNameOfType(. t) === Some("VoidKeyword")
}
let isObjectKeyword = (. t: Type.t) => {
  getKindNameOfType(. t) === Some("ObjectKeyword")
}

let identifyType = (. t: Type.t) => {
  open Type

  if isAny(t) {
    "Any"
  } else if isAnonymous(t) {
    "Anonymous"
  } else if isArray(t) {
    "Array"
  } else if isBoolean(t) {
    "Boolean"
  } else if isString(t) {
    "String"
  } else if isNumber(t) {
    "Number"
  } else if isEnumLiteral(t) {
    "EnumLiteral"
  } else if isNumberLiteral(t) {
    "NumberLiteral"
  } else if isStringLiteral(t) {
    "StringLiteral"
  } else if isLiteral(t) {
    "Literal"
  } else if isBooleanLiteral(t) {
    "BooleanLiteral"
  } else if isUnion(t) {
    "Union"
  } else if isClass(t) {
    "Class"
  } else if isClassOrInterface(t) {
    "ClassOrInterface"
  } else if isInterface(t) {
    "Interface"
  } else if isObject(t) {
    "Object"
  } else if isEnum(t) {
    "Enum"
  } else if isTypeParameter(t) {
    "TypeParameter"
  } else if isTuple(t) {
    "Tuple"
  } else if isIntersection(t) {
    "Intersection"
  } else if isUnionOrIntersection(t) {
    "UnionOrIntersection"
  } else if isUnknown(t) {
    "Unknown"
  } else if isNull(t) {
    "Null"
  } else if isUndefined(t) {
    "Undefined"
  } else if isTypeReference(. t) {
    "TypeReference"
  } else if isObjectKeyword(. t) {
    "ObjectKeyword"
  } else if isVoid(. t) {
    "Void"
  } else {
    "Unidentified"
  }
}

module Set = Belt.HashSet.Int
let idCache = Belt.HashSet.Int.make(~hintSize=50)

exception UnidentifiedType(Type.t)

let rec visitType = (. t: Type.t) => {
  switch t->Type.compilerType->Typescript_raw.Type.id {
  | None => ()
  | Some(id) =>
    if idCache->Set.has(id) {
      ()
    } else {
      idCache->Set.add(id)
      visitTypeAfterCache(. t)
    }
  }
}
and visitTypeAfterCache = (. t: Type.t) => {
  let stringifiedType = identifyType(. t)
  if stringifiedType === "Unidentified" {
    raise(UnidentifiedType(t))
  }
  t->Type.compilerType->Typescript_raw.Type.setKindName(stringifiedType)
  if stringifiedType === "Unidentified" {
    switch getKindNameOfType(. t) {
    | None => ()
    | Some(nodeKind) => t->Type.compilerType->Typescript_raw.Type.setNodeKind(nodeKind)
    }
  }
  if t->Type.isUnion {
    visitTypeOptArray(. t->Type.getUnionTypes)
  }
  if t->Type.isIntersection {
    visitTypeOptArray(. t->Type.getIntersectionTypes)
  }
  if isTypeReference(. t) {
    visitTypeOpt(. t->Type.getTargetType)
  }
  if t->Type.isTuple {
    visitTypeOptArray(. t->Type.getTupleElements)
  }
  if t->Type.isArray {
    visitTypeOpt(. t->Type.getArrayElementType)
  }
}
and visitTypeOpt = (. t) =>
  switch t {
  | None => ()
  | Some(t) => visitType(. t)
  }
and visitTypeOptArray = (. t) =>
  switch t {
  | None => ()
  | Some(t) => t->Belt.Array.forEachU(visitType)
  }

let rec visitNode = (. node: Node.t) => {
  switch node->Node.getSymbol {
  | None => ()
  | Some(symbol) =>
    switch symbol->Symbol.getDeclaredType {
    | None => ()
    | Some(t) =>
      try visitType(. t) catch {
      | UnidentifiedType(t) =>
        Js.logMany([
          node->Node.getStartLineNumber,
          node->Node.getEndLineNumber,
          node->Node.getPos,
          node->Node.getEnd,
          t->Type.getText->Obj.magic,
          node->Node.getText->Obj.magic,
        ])
      }
      symbol->Symbol.compilerSymbol->Typescript_raw.Symbol.setResolvedType(t->Type.compilerType)
      symbol
      ->Symbol.compilerSymbol
      ->Typescript_raw.Symbol.setFullyQualifiedName(symbol->Symbol.getFullyQualifiedName)
    }
    node->Node.compilerNode->Typescript_raw.Node.setResolvedSymbol(symbol->Symbol.compilerSymbol)
  }
  switch node->Node.getType {
  | None => ()
  | Some(t) =>
    try visitType(. t) catch {
    | UnidentifiedType(t) =>
      Js.logMany([
        node->Node.getStartLineNumber,
        node->Node.getEndLineNumber,
        node->Node.getPos,
        node->Node.getEnd,
        t->Type.getText->Obj.magic,
        node->Node.getText->Obj.magic,
      ])
    }
    node->Node.compilerNode->Typescript_raw.Node.setResolvedType(t->Type.compilerType)
  }
  node->Node.compilerNode->Typescript_raw.Node.setKindName(node->Node.getKindName)
  node->Node.getChildren->Belt.Array.forEachU(visitNode)
}

let preParse = (~project) => {
  Set.clear(idCache)

  project->Project.getSourceFiles->Belt.Array.forEachU((. sourceFile) => {
    sourceFile
    ->SourceFile.toNode
    ->Node.compilerNode
    ->Typescript_raw.Node.setKindName(sourceFile->SourceFile.toNode->Node.getKindName)
    sourceFile->SourceFile.toNode->Node.getChildren->Belt.Array.forEachU(visitNode)
  })
}

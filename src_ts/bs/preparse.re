open Ts_morph;

let isTypeReference = (t: Type.t) => {
  switch (
    Runtime.typeChecker->Ts_Typechecker.typeToTypeNode(t->Type.compilerType)
  ) {
  | None => false
  | Some(node) => node->Obj.magic##getKindName === "TypeReference"
  };
};
let identifyType =
  (. t: Type.t) =>
    Type.(
      if (isAny(t)) {
        "Any";
      } else if (isAnonymous(t)) {
        "Anonymous";
      } else if (isArray(t)) {
        "Array";
      } else if (isBoolean(t)) {
        "Boolean";
      } else if (isString(t)) {
        "String";
      } else if (isNumber(t)) {
        "Number";
      } else if (isEnumLiteral(t)) {
        "EnumLiteral";
      } else if (isNumberLiteral(t)) {
        "NumberLiteral";
      } else if (isStringLiteral(t)) {
        "StringLiteral";
      } else if (isLiteral(t)) {
        "Literal";
      } else if (isBooleanLiteral(t)) {
        "BooleanLiteral";
      } else if (isUnion(t)) {
        "Union";
      } else if (isClass(t)) {
        "Class";
      } else if (isClassOrInterface(t)) {
        "ClassOrInterface";
      } else if (isInterface(t)) {
        "Interface";
      } else if (isObject(t)) {
        "Object";
      } else if (isEnum(t)) {
        "Enum";
      } else if (isTypeParameter(t)) {
        "TypeParameter";
      } else if (isTuple(t)) {
        "Tuple";
      } else if (isIntersection(t)) {
        "Intersection";
      } else if (isUnionOrIntersection(t)) {
        "UnionOrIntersection";
      } else if (isUnknown(t)) {
        "Unknown";
      } else if (isNull(t)) {
        "Null";
      } else if (isUndefined(t)) {
        "Undefined";
      } else if (isTypeReference(t)) {
        "TypeReference";
      } else {
        "Unidentified";
      }
    );

let rec visitType =
  (. t: Type.t) => {
    t->Type.compilerType->Typescript_raw.Type.setKindName(identifyType(. t));
    if (t->Type.isUnion) {
      visitTypeOptArray(. t->Type.getUnionTypes);
    };
    if (t->Type.isIntersection) {
      visitTypeOptArray(. t->Type.getIntersectionTypes);
    };
    if (t->isTypeReference) {
      visitTypeOpt(. t->Type.getTargetType);
    };
    if (t->Type.isTuple) {
      visitTypeOptArray(. t->Type.getTupleElements);
    };
    if (t->Type.isArray) {
      visitTypeOpt(. t->Type.getArrayElementType);
    };
  }
and visitTypeOpt =
  (. t) =>
    switch (t) {
    | None => ()
    | Some(t) => visitType(. t)
    }
and visitTypeOptArray =
  (. t) =>
    switch (t) {
    | None => ()
    | Some(t) => t->Belt.Array.forEachU(visitType)
    };

let rec visitNode =
  (. node: Node.t) => {
    switch (node->Node.getSymbol) {
    | None => ()
    | Some(symbol) =>
      switch (symbol->Symbol.getDeclaredType) {
      | None => ()
      | Some(t) =>
        visitType(. t);
        symbol
        ->Symbol.compilerSymbol
        ->Typescript_raw.Symbol.setResolvedType(t->Type.compilerType);
      };
      node
      ->Node.compilerNode
      ->Typescript_raw.Node.setResolvedSymbol(symbol->Symbol.compilerSymbol);
    };
    node
    ->Node.compilerNode
    ->Typescript_raw.Node.setKindName(node->Node.getKindName);
    node->Node.getChildren->Belt.Array.forEachU(visitNode);
  };

let preParse = () => {
  Runtime.project
  ->Project.getSourceFiles
  ->Belt.Array.forEachU((. sourceFile) => {
      sourceFile
      ->SourceFile.toNode
      ->Node.compilerNode
      ->Typescript_raw.Node.setKindName(
          sourceFile->SourceFile.toNode->Node.getKindName,
        );

      visitNode(. sourceFile->SourceFile.toNode);
    });
};

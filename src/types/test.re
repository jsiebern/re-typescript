let json = {|
{
  "symbols": [
    [
      1,
      {
        "flags": 524288,
        "name": "str",
        "escapedName": "str",
        "fullyQualifiedName": "str",
        "exportSymbol": 1,
        "declarations": [21574],
        "members": [],
        "declaredType": 56,
        "id": 1
      }
    ],
    [
      2,
      {
        "flags": 524288,
        "name": "num",
        "escapedName": "num",
        "fullyQualifiedName": "num",
        "exportSymbol": 2,
        "declarations": [21576],
        "members": [],
        "declaredType": 58,
        "id": 2
      }
    ],
    [
      3,
      {
        "flags": 524288,
        "name": "boo",
        "escapedName": "boo",
        "fullyQualifiedName": "boo",
        "exportSymbol": 3,
        "declarations": [21578],
        "members": [],
        "declaredType": 59,
        "id": 3
      }
    ],
    [
      4,
      {
        "flags": 524288,
        "name": "zoo",
        "escapedName": "zoo",
        "fullyQualifiedName": "zoo",
        "exportSymbol": 4,
        "declarations": [21580],
        "members": [],
        "declaredType": 61,
        "id": 4
      }
    ]
  ],
  "nodes": [
    [
      21575,
      {
        "pos": 5,
        "end": 9,
        "flags": 8388608,
        "kind": 75,
        "kindName": "Identifier",
        "symbol": 1,
        "id": 21575,
        "escapedText": "str"
      }
    ],
    [
      21574,
      {
        "pos": 0,
        "end": 19,
        "flags": 8388608,
        "kind": 247,
        "kindName": "TypeAliasDeclaration",
        "symbol": 1,
        "id": 21574,
        "name": 21575,
        "parameters": [],
        "type": 56,
        "modifiers": []
      }
    ],
    [
      21577,
      {
        "pos": 24,
        "end": 28,
        "flags": 8388608,
        "kind": 75,
        "kindName": "Identifier",
        "symbol": 2,
        "id": 21577,
        "escapedText": "num"
      }
    ],
    [
      21576,
      {
        "pos": 19,
        "end": 38,
        "flags": 8388608,
        "kind": 247,
        "kindName": "TypeAliasDeclaration",
        "symbol": 2,
        "id": 21576,
        "name": 21577,
        "parameters": [],
        "type": 58,
        "modifiers": []
      }
    ],
    [
      21579,
      {
        "pos": 43,
        "end": 47,
        "flags": 8388608,
        "kind": 75,
        "kindName": "Identifier",
        "symbol": 3,
        "id": 21579,
        "escapedText": "boo"
      }
    ],
    [
      21578,
      {
        "pos": 38,
        "end": 58,
        "flags": 8388608,
        "kind": 247,
        "kindName": "TypeAliasDeclaration",
        "symbol": 3,
        "id": 21578,
        "name": 21579,
        "parameters": [],
        "type": 59,
        "modifiers": []
      }
    ],
    [
      21581,
      {
        "pos": 63,
        "end": 67,
        "flags": 8388608,
        "kind": 75,
        "kindName": "Identifier",
        "symbol": 4,
        "id": 21581,
        "escapedText": "zoo"
      }
    ],
    [
      21580,
      {
        "pos": 58,
        "end": 75,
        "flags": 8388608,
        "kind": 247,
        "kindName": "TypeAliasDeclaration",
        "symbol": 4,
        "id": 21580,
        "name": 21581,
        "parameters": [],
        "type": 61,
        "modifiers": []
      }
    ],
    [
      21573,
      {
        "pos": 0,
        "end": 76,
        "flags": 8388672,
        "kind": 290,
        "kindName": "SourceFile",
        "id": 21573,
        "filePath": "/simple.d.ts",
        "baseName": "simple.d.ts",
        "baseNameWithoutExtension": "simple",
        "extension": ".d.ts",
        "scriptKind": 3,
        "isDeclarationFile": true,
        "isFromExternalLibrary": false,
        "isInNodeModules": false,
        "fullSource": "\ntype str = string;\ntype num = number;\ntype boo = boolean;\ntype zoo = true;\n",
        "symbols": [1, 2, 3, 4],
        "statements": [21574, 21576, 21578, 21580]
      }
    ]
  ],
  "sourceFiles": [["/simple.d.ts", 21573]],
  "types": [
    [56, { "id": 56, "flags": 524288, "symbol": 1, "aliasTypeArguments": [] }],
    [58, { "id": 58, "flags": 524288, "symbol": 2, "aliasTypeArguments": [] }],
    [59, { "id": 59, "flags": 262144, "symbol": 3, "aliasTypeArguments": [] }],
    [61, { "id": 61, "flags": 524288, "symbol": 4, "aliasTypeArguments": [] }]
  ]
}
|};

let parsed = Ts_ast_j.result_of_string(json);
let go = () => {
  Console.log(parsed);
};

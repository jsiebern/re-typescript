let content = {|
{
  "identifierCount": 2,
  "nodeCount": 8,
  "isDeclarationFile": true,
  "languageVariant": 0,
  "fileName": "/test.d.ts",
  "languageVersion": 99,
  "text": "\n// type bla = {\n//   a: string;\n//   b: string;\n// };\n// type c = keyof bla;\n// export type d = ['s1', 's2', string];\ntype str = string;\ntype num = number;\n",
  "statements": [
    {
      "type": {
        "flags": 8388608,
        "kind": 143,
        "end": 136,
        "pos": 129,
        "kindName": "StringKeyword"
      },
      "name": {
        "escapedText": "str",
        "resolvedSymbol": {
          "name": "str",
          "fullyQualifiedName": "str",
          "resolvedType": { "flags": 4, "kindName": "String" },
          "id": 21328,
          "flags": 524288
        },
        "flags": 8388608,
        "kind": 75,
        "end": 127,
        "pos": 123,
        "kindName": "Identifier"
      },
      "resolvedSymbol": {
        "name": "str",
        "fullyQualifiedName": "str",
        "resolvedType": { "flags": 4, "kindName": "String" },
        "id": 21328,
        "flags": 524288
      },
      "flags": 8388608,
      "kind": 247,
      "end": 137,
      "pos": 0,
      "kindName": "TypeAliasDeclaration"
    },
    {
      "type": {
        "flags": 8388608,
        "kind": 140,
        "end": 155,
        "pos": 148,
        "kindName": "NumberKeyword"
      },
      "name": {
        "escapedText": "num",
        "resolvedSymbol": {
          "name": "num",
          "fullyQualifiedName": "num",
          "resolvedType": { "flags": 8, "kindName": "Number" },
          "id": 21329,
          "flags": 524288
        },
        "flags": 8388608,
        "kind": 75,
        "end": 146,
        "pos": 142,
        "kindName": "Identifier"
      },
      "resolvedSymbol": {
        "name": "num",
        "fullyQualifiedName": "num",
        "resolvedType": { "flags": 8, "kindName": "Number" },
        "id": 21329,
        "flags": 524288
      },
      "flags": 8388608,
      "kind": 247,
      "end": 156,
      "pos": 137,
      "kindName": "TypeAliasDeclaration"
    }
  ],
  "flags": 8388672,
  "kind": 290,
  "end": 157,
  "pos": 0,
  "kindName": "SourceFile"
}
|};

let run = () => {
  let content = Typescript_j.node_of_string(content);
  Console.log(content);
};

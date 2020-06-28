type t;

[@bs.send]
external getSymbolAtLocation:
  (t, Typescript_raw.node) => option(Typescript_raw.symbol) =
  "getSymbolAtLocation";
[@bs.send]
external getDeclaredTypeOfSymbol:
  (t, Typescript_raw.symbol) => option(Typescript_raw.type_) =
  "getDeclaredTypeOfSymbol";
[@bs.send]
external getTypeArguments:
  (t, Typescript_raw.type_) => option(array(Typescript_raw.type_)) =
  "getTypeArguments";
[@bs.send]
external typeToString: (t, Typescript_raw.type_) => string = "typeToString";
[@bs.send]
external typeToTypeNode:
  (t, Typescript_raw.type_) => option(Typescript_raw.node) =
  "typeToTypeNode";

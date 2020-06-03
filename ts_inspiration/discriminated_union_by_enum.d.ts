export enum ShapeKind {
  Circle = "circle",
  Square = "square",
}

interface Circle {
  kind: ShapeKind.Circle;
  radius: number;
}

interface Square {
  kind: ShapeKind.Square;
  sideLength: number;
}

declare namespace buh {
  let c: Circle;
}

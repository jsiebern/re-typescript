interface A {
  x: string;
  y: number;
}
type B = A | undefined;
type keys = keyof B;

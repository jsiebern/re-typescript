type AsRequired<T> = {
  [P in keyof T]-?: T[P];
};
type AsPartial<T> = {
  [P in keyof T]?: T[P];
};
type AsNothing<T> = {
  [P in keyof T]: T[P];
};
interface A {
  x: string;
  y?: number;
  z: boolean;
}
type same = AsNothing<A>;
type req = AsRequired<A>;
type part = AsPartial<A>;

interface Dictionary<T> {
  [key: string]: T;
}
export let keys: keyof Dictionary<number>; // string | number
export let value: Dictionary<number>["foo"]; // number

type Proxy<T> = {
  get(): T;
  set(value: T): void;
};
type Proxify<T> = {
  [P in keyof T]: Proxy<T[P]>;
};

interface A {
  x: string;
  y: number;
  z: boolean;
}
type proxied = Proxify<A>;

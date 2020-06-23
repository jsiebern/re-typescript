interface Dictionary<T> {
  [key: string]: T;
}
export let keys: keyof Dictionary<number>; // string | number
export let value: Dictionary<number>["foo"]; // number

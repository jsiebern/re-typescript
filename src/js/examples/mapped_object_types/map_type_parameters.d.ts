interface Keys<A> {
  key_1: string;
  key_2: number;
  key_3: A;
}
type with_keys = { [K in keyof Keys<{ key_4: boolean }>["key_3"]]: boolean };

// ---

type IPick<T, K extends keyof T> = {
  [P in K]: T[P];
};
interface A {
  x: string;
  y: number;
  z: boolean;
}

type keys = "x" | "y";
type stripped = IPick<A, keys>;

// ---

type IRecord<K extends keyof any, T> = {
  [P in K]: T;
};

type record = IRecord<"a" | "b", { field: boolean }>;

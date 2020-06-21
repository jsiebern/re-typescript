interface A {
  x: string;
  y: number;
  z: boolean;
}

type PartialX<T> = {
  [P in keyof T]?: T[P];
};

type PartialWithNewMember<T> = {
  [P in keyof T]?: T[P];
} & { newMember: boolean };

type a_partial = PartialX<A>;
type b_partial = PartialWithNewMember<A>;

// type Keys = "option1" | "option2";
// type Flags = { [K in Keys]: boolean };

// interface C {
//   key_1: string;
//   key_2: number;
//   flags: { [K in keyof C]: boolean };
// }

interface Keys<A> {
  key_1: string;
  key_2: number;
  key_3: A;
}
type with_keys = { [K in keyof Keys<{ key_4: boolean }>["key_3"]]: boolean };

// interface A {
//   x: string;
//   y: number;
//   z: boolean;
// }

// type PartialX<T> = {
//   [P in keyof T]?: T[P];
// };

// type PartialWithNewMember<T> = {
//   [P in keyof T]?: T[P];
// } & { newMember: boolean };

// type a_partial = PartialX<A>;
// type b_partial = PartialWithNewMember<A>;

// type Keys = "option1" | "option2";
// type Flags = { [K in Keys]: boolean };

// interface C {
//   key_1: string;
//   key_2: number;
//   flags: { [K in keyof C]: boolean };
// }

// interface Keys {
//   key_1: string;
//   key_2: number;
//   key_3: {
//     snd: boolean;
//   };
// }

// type XX = "key_1" | "key_2";
// type y = Keys[XX | "key_3"];
// type xbla = { key: string; snd: number }["key"];

// type Pick<T, K extends keyof T> = {
//   [P in K]: T[P];
// };

interface A {
  x: string;
  y: number;
  z: boolean;
}

type keys = "x" | "y";
type stripped = Pick<A, keys>;

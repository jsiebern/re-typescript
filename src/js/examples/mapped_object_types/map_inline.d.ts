// --- Resolve an inline mapped type
type Keys = "option1" | "option2";
interface A {
  key_1: string;
  key_2: number;
  flags: { [K in Keys]: boolean };
}

// --- Resolve a recursive inline mapped type
interface B {
  key_1: string;
  key_2: number;
  flags: { [K in keyof B]: boolean };
}

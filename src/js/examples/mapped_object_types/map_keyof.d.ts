// --- Map over a keyof type
type KeysObj = { option1: string; option2: number };
type Flags_1 = { [K in keyof KeysObj]: boolean };

// --- Map over a keyof inline type
type Flags = { [K in keyof { option1: string; option2: number }]: boolean };

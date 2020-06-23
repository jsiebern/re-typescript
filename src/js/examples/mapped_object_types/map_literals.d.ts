// --- Map over an existing string literal type as keys
type Keys = "option1" | "option2";
type Flags_1 = { [K in Keys]: boolean };

// --- Map through a reference
type KeysRef = Keys;
type Flags_2 = { [K in KeysRef]: boolean };

// --- Map over an inline string literal
type Flags_3 = { [K in "option1" | "option2"]: boolean };

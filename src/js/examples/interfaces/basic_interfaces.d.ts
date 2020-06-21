// --- Interfaces & basic interface inheritance
interface IOne {
  key_1: string;
  key_2: boolean;
}
interface ITwo extends IOne {
  key_3: number;
}

// --- Interfaces & obj types are generated equal as records
type obj = { key: string };

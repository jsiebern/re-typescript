// --- Basic type extraction
type obj_1 = { key: string };
interface IExtraction {
  key_1: boolean;
  key_2: obj_1;
}
type extracted_bool = IExtraction["key_1"];
type extracted_string_nested = IExtraction["key_2"]["key"];

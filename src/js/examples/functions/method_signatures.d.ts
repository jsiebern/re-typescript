// --- Method signature in interfaces
interface method_signature {
  func1: (x: number) => number; // Function type literal
  func2(x: number): number; // Method signature
  func3(x: string, y: boolean): void; // Method signature for extraction
}
type extracted_method_signature = method_signature["func3"];

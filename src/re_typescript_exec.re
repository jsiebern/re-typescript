let test = (
  "/test.d.ts",
  {|

interface method_signature {
              func1: (x: number) => number;       // Function type literal
              func2(x: number): number;           // Method signature
              func3(x: string, y: boolean): void; // Method signature for extraction
            }
            type extracted_method_signature = method_signature['func3'];

// ------------------------------------------------------

|},
);

switch (Re_typescript_lib.quick_parse([|test|])) {
| Error(e) => raise(Failure(e))
| Ok(parsed) => Console.log(parsed)
};

Console.log(
  "\n-------------------------------------------------------------------\n",
);

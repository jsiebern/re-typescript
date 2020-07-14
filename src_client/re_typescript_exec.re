let test = (
  "/test.d.ts",
  {|


   export default function(a: string, b?: number): void;

   export function motha(one: string, two: boolean, three: [string, number]): string[];


|},
);
switch (Re_typescript_lib.quick_parse([|test|])) {
| Error(e) => raise(Failure(e))
| Ok(parsed) => Console.log(parsed)
};

Console.log(
  "\n-------------------------------------------------------------------\n",
);

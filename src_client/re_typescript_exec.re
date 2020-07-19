let test = (
  "/test.d.ts",
  {|

// type PickX<T, K extends keyof T> = {
//     [P in K]: T[P];
// };
// interface A {
//   x: string;
//   y: number;
//   z: boolean;
// }

// type keys = 'x' | 'y';
// type stripped = PickX<A, keys>;

// ------------------------------------------------------

// interface IMain {
//   simple: string,
//   inline: {
//     num: number
//   }
// }
// type str = IMain['simple']
// type obj = IMain['inline'];
// type num = obj['num'];

// module Test = {
//   type iMain_inline = {num: float};
//   type iMain = {
//     simple: string,
//     inline: iMain_inline,
//   };
//   type str = string;
//   type obj_inline = {num: float};
//   type obj = obj_inline; <<- THIS SHOULD BE 'iMain_inline' !!!
//   type num = float;
// };



interface method_signature {
  func1: (x: number) => number;
  func2(x: number): number;
  func3(x: string, y: boolean): void;
}
type extracted_method_signature = method_signature['func3' | 'func2'];



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

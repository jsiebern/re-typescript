let test = (
  "/test.d.ts",
  {|

// type parameters that are defined inline will be bubbled up the tree
export interface Map<A,B> {
  a: A;
  b: B;
}
export interface RecoilRootProps {
  initializeState?: (options: {
    set: <T>(recoilVal: T, newVal: T) => void; // Ignores type params on inline functions
    setUnvalidatedAtomValues: (atomMap: Map<string, any>) => void;
  }) => void;
}

// bubbling type parameters can be combined with regular ones
// export interface Map<A,B> {
//   a: A;
//   b: B;
// }
// export interface RecoilRootProps<C> {
//   initializeState?: (options: {
//     set: <T>(recoilVal: T, newVal: T) => void;
//     setUnvalidatedAtomValues: (atomMap: Map<string, C>) => void;
//   }) => void;
// }

// unresolvable type params get passed on as far down as possible
// export interface Map<A,B> {
//   a: A;
//   b: B;
// }
// export interface RecoilRootProps<C> {
//   initializeState?: (options: {
//     set: <T>(recoilVal: T, newVal: T) => void;
//     setUnvalidatedAtomValues: (atomMap: Map<string, C>) => void;
//   }) => void;
// }

// type use_it = RecoilRootProps<string>;



// works with array / nullable on the original
// type x = 3 | 4;
// type y = Array<1 | x | 2 | null>;








// type animation = 'overlay' | 'push' | 'scale down' | 'uncover' | 'slide out' | 'slide along'

// export declare type Subset<T, U> = {
//   [key in keyof T]: key extends keyof U ? T[key] : never;
// };

// type xyz = Subset<animation, 'push' | 'scale down'>

|},
);

Console.log(
  "\n-------------------------------------------------------------------\n",
);

switch (Re_typescript_lib.quick_parse([|test|])) {
| Error(e) => raise(Failure(e))
| Ok(parsed) => Console.log(parsed)
};

Console.log(
  "\n-------------------------------------------------------------------\n",
);

// --------------------------------------------------------

export interface Map<A, B> {
  a: A;
  b: B;
}
export interface RecoilRootProps<C> {
  initializeState?: (options: {
    set: <T>(recoilVal: T, newVal: T) => void;
    setUnvalidatedAtomValues: (atomMap: Map<string, C>) => void;
  }) => void;
}

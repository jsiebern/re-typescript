interface IPromise<X> {
  field: X;
}

type ResouceResult<T> = T & {
  promise: IPromise<T>;
  resolved: boolean;
};

// --------------------------------------------------------

export interface RecoilRootProps {
  initializeState?: (options: {
    set: <T>(recoilVal: T, newVal: T) => void; // Ignores type params on inline functions
    setUnvalidatedAtomValues: (atomMap: Map<string, any>) => void;
  }) => void;
}

// --------------------------------------------------------

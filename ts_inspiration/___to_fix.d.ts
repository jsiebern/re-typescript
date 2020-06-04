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

type x = string[];
type a<Props> = Props | x | y<Props>;
type y<Props> = { x: Props };

// module IS_CODE = {
//   type x = array(string)
//   and y('Props) = {x: 'Props};
//   module A: {
//     type t;
//     let props: 'Props => t;
//     let x: x => t;
//     let y: y('Props) => t;
//   } = {
//     [@unboxed]
//     type t =
//       | Any('a): t;
//     let props = (v: 'Props) => Any(v);
//     let x = (v: x) => Any(v);
//     let y = (v: y('Props)) => Any(v);
//   };
//   type a('Props) = A.t;

//   let a: a(string) = A.props(3);
//   let b: a(string) = A.y({x: 3});
// };

// module SHOULD_BE_CODE = {
//   type x = array(string)
//   and y('Props) = {x: 'Props};
//   module A: {
//     type t('Props);
//     let props: 'Props => t('Props);
//     let x: x => t('Props);
//     let y: y('Props) => t('Props);
//   } = {
//     [@unboxed]
//     type t('Props) =
//       | Any('a): t('Props);
//     let props = (v: 'Props) => Any(v);
//     let x = (v: x) => Any(v);
//     let y = (v: y('Props)) => Any(v);
//   };
//   type a('Props) = A.t('Props);

//   let a: a(string) = A.props(3);
//   let b: a(string) = A.y({x: 3});
// };

// --------------------------------------------------------

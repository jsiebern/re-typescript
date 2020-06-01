export declare type Subset<T, U> = {
  [key in keyof T]: key extends keyof U ? T[key] : never;
};

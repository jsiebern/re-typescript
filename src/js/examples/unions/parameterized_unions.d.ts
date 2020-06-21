type ValueOrObj<T> = T | { obj_value: T };
type ValueOrArray<T> = T | Array<ValueOrArray<T>>;

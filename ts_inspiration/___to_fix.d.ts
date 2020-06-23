interface Dictionary<T> {
  [key: string]: T;
}
export let keys: keyof Dictionary<number>; // string | number
export let value: Dictionary<number>["foo"]; // number

//////////////////////////////////////////////////////////

type TypeName<T> = T extends string
  ? "string"
  : T extends number
  ? "number"
  : T extends boolean
  ? "boolean"
  : T extends undefined
  ? "undefined"
  : T extends Function
  ? "function"
  : "object";

type T0 = TypeName<string>; // "string"
type T1 = TypeName<"a">; // "string"
type T2 = TypeName<true>; // "boolean"
type T3 = TypeName<() => void>; // "function"
type T4 = TypeName<string[]>; // "object"

//////////////////////////////////////////////////////////

interface A {
  x: string;
  y: number;
  z: boolean;
}

type Exclude<T, U> = T extends U ? never : T;
type stripped = Exclude<keyof A, "y">;

export enum StringEnum {
  A = "A",
  B = "B",
}

export enum NumberEnum {
  A,
  B,
}

//////////////////////////////////////////////////////////

type StrictNumericEnumParam<
  Enum extends number,
  Param extends Enum
> = true extends ({ [key: number]: false } & { [P in Enum]: true })[Enum]
  ? true extends ({ [key: number]: false } & { [P in Enum]: true })[Param]
    ? Param
    : never
  : Enum;

export enum Foo {
  A,
  B,
  C,
}

export enum Bar {
  D,
  E,
  F,
}

// typically would be written as function doFoo(value: Foo): void
declare function doFoo<Value extends Foo>(
  value: StrictNumericEnumParam<Foo, Value>
): void;

declare const foo: Foo;
declare const bar: Bar;
declare const n: number;

type x = "xxx" extends string ? true : false;

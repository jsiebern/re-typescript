type x = {
  y: string;
};

type y = x;

interface A<Param> {
  x: string;
  y: number;
  z: Param;
}
declare namespace XY {
  type keys = keyof A<boolean>;
  type bla = string;
}
export type xx = XY.bla;

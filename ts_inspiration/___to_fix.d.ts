// let content = {|
//     import { funky, other } from './global';

//     type x = funky;
//     type y = other;
//     |};
//     let global = {|
//     export type funky = (arg: other) => boolean;
//     export type other = number;
//     |};

type x = keyof { x: string; y: number; 3: boolean };
type y = keyof {};

export var f: x;
export var z: typeof f;

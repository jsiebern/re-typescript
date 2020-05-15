let map = (f, v) => Belt.Option.map(v, f);
let default = (f, v) => Belt.Option.getWithDefault(v, f);
let map_default = (f,d,v) => v->Belt.Option.map(f)->Belt.Option.getWithDefault(d);
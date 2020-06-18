module type Config = {let tbl: Hashtbl.t(Fp.t(Fp.absolute), string);};
module Make: (Config) => Loader.T;

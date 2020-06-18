module Resolver = Resolver;
module Loader = Loader;
module Loader_virtual = Loader_virtual;

let default_loader: module Loader.T =
  (module
   Loader_virtual.Make({
     let tbl = Hashtbl.create(0);
   }));
let default_resolver: module Resolver.T =
  (module
   Resolver.Make({
     let config = {Resolver.loader: default_loader, tsconfig: None};
   }));

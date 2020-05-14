type m('a) = {copy_structure: 'a};
let migrate: ('a, 'b) => m('c) = (_, _) => ""->Obj.magic;
let ocaml_406 = ""->Obj.magic;
let ocaml_current = ""->Obj.magic;

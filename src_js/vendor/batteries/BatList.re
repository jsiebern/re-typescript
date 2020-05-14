include List;

let reduce = f =>
  fun
  | [] => invalid_arg("List.reduce: Empty List")
  | [h, ...t] => fold_left(f, h, t);

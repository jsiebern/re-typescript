module Int = Int32

module IntShift = {
  let \"||": (int, int) => int = Int.logor->Obj.magic
  let \"<<": (int, int) => int = Int.shift_left->Obj.magic
  let \">>": (int, int) => int = Int.shift_right->Obj.magic
  let \">>>": (int, int) => int = Int.shift_right_logical->Obj.magic
  let \"~~": int => int = Int.lognot->Obj.magic
  let \"&": (int, int) => int = Int.logand->Obj.magic
  let \"&&" = (v1, v2) => \"&"(v1, v2) > 0
}

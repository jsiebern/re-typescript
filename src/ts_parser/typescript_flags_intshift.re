module IntShift = {
  let (&): (int, int) => int = Int.logand;
  let (||): (int, int) => int = Int.logor;
  let (<<): (int, int) => int = Int.shift_left;
  let (>>): (int, int) => int = Int.shift_right;
  let (>>>): (int, int) => int = Int.shift_right_logical;
  let (~~): int => int = Int.lognot;
  let (&&) = (v1, v2) => (v1 & v2) > 0;
};

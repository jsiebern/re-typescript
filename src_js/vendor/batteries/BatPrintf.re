let fprintf = (v: ref(string), _, string) => {
  v := v^ ++ string;
  ();
};
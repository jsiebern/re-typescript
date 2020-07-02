type img = {default: string};
@bs.val external img: string => img = "require";
let logo_reason = img("./../../../../src_js/assets/reasonml.png").default;
let logo_ocaml = img("./../../../../src_js/assets/ocaml.png").default;
let logo_re_typescript =
  img("./../../../../src_js/assets/re_typescript.png").default;
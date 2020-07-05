type img = {default: string}
@bs.val external img: string => img = "require"
let logo_reason = img("./reasonml.png").default
let logo_ocaml = img("./ocaml.png").default
let logo_re_typescript = img("./re_typescript.png").default

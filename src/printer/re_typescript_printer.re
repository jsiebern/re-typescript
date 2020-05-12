open Migrate_parsetree;

let print_from_ts = (input: Re_typescript_base.Ts.toplevel) => {
  let ast = Re_typescript_generate_ast.generate(input);
  // let migration =
  //   Versions.migrate(Versions.ocaml_406, Versions.ocaml_current);

  Pprintast.string_of_structure(ast |> Obj.magic);
};
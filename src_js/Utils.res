open Config_t
open! Bridge_bs

let default_bucklescript_config = {
  string_variant_mode: #BsInline,
  mixed_variant_mode: #BsUnboxed,
  number_variant_mode: #BsInline(None),
  union_mode: #KeepLiterals,
}

let default_config: config = {
  suppress_warning_for_extended_records: false,
  omit_extended_unreferenced_records: false,
  output_type: Bucklescript,
  bucklescript_config: default_bucklescript_config,
  array_mode: Array,
  number_mode: Float,
  intersection_mode: {
    objects: Tuple,
    unions: Tuple,
    classes: Tuple,
    functions: Tuple,
    other: Tuple,
    tuple_members_optional: false,
  },
  generate_parser: false,
  generate_serializer: false,
}

let config = {
  ...default_config,
  number_mode: Int,
  output_type: Bucklescript,
  omit_extended_unreferenced_records: true,
}

let getFilesInExample = (example: example) => {
  let files = ref([])
  let rec walk = (~path=[], tree: example_tree) => {
    switch tree {
    | File(fileName, fileContent) =>
      files :=
        files.contents->Belt.Array.concat([
          (`${path->Js.Array.joinWith("/", _)}/${fileName}`, fileContent),
        ])
    | Folder(folderName, treeLst) =>
      let path = path->Belt.Array.concat([folderName])
      treeLst->Belt.List.forEach(tree => {
        walk(~path, tree)
      })
    }
  }
  example.files->Belt.List.forEach(tree => {
    walk(~path=[example.path], tree)
  })
  files.contents
}

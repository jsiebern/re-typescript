open Js_of_ocaml;

module Bridge = Re_typescript_config.Bridge;
type tree =
  | Folder(string, list(tree))
  | File(string, string);

let examples = [
  // -----------------------------------------------------------------------------
  Folder(
    "base_type_declarations",
    [
      File(
        "meta.json",
        Bridge.string_of_example_meta({
          name: "Base type declarations",
          description: "",
          entry: "base_type_declarations.d.ts",
        }),
      ),
      File(
        "base_type_declarations.d.ts",
        {|
// --- Base type declarations
type s = string;
type n = number; // re-typescript has different number resolution modes, default is "float"
type b = boolean;
type v = void;
  |},
      ),
    ],
  ),
  // -----------------------------------------------------------------------------
  Folder(
    "any_type",
    [
      File(
        "meta.json",
        Bridge.string_of_example_meta({
          name: "Any type",
          description: "",
          entry: "any_type.d.ts",
        }),
      ),
      File(
        "any_type.d.ts",
        {|
// --- Any: A separate any type will be created that is marked as unboxed
// --- Currently this would need to be decoded via %identity access
type whatever = any;
    |},
      ),
    ],
  ),
  // -----------------------------------------------------------------------------
  Folder(
    "null_undefined",
    [
      File(
        "meta.json",
        Bridge.string_of_example_meta({
          name: "Null / Undefined",
          description: "",
          entry: "null_undefined.d.ts",
        }),
      ),
      File(
        "null_undefined.d.ts",
        {|
// --- Unions that contain null / undefined are wrapped in the conversion
type s_null =     string | null;
type s_opt =      string | undefined;
type s_null_opt = string | null | undefined;
    |},
      ),
    ],
  ),
  // -----------------------------------------------------------------------------
  Folder(
    "interfaces",
    [
      File(
        "meta.json",
        Bridge.string_of_example_meta({
          name: "Interfaces",
          description: "",
          entry: "basic_interfaces.d.ts",
        }),
      ),
      File(
        "basic_interfaces.d.ts",
        {|
// --- Interfaces & basic interface inheritance
interface IOne {
  key_1: string;
  key_2: boolean;
}
interface ITwo extends IOne {
  key_3: number;
}

// --- Interfaces & obj types are generated equal as records
type obj_1 = { key: string };
    |},
      ),
      File(
        "interface_extension.d.ts",
        {|
// --- Interface extensions are resolved properly
// --- Created a configurable warning suppression ([@warning "-30"]) for overlapping fields in extensions
interface Extend_me<Fill, Default = boolean> {
  field_1: Fill;
  field_2: Default;
  field_4: string;
}
interface Extends_one extends Extend_me<string> {
  field_3: number
}
interface Extends_another<PassThrough> extends Extend_me<PassThrough> {
  field_3: PassThrough
}
    |},
      ),
    ],
  ),
  // -----------------------------------------------------------------------------
  Folder(
    "intersections",
    [
      File(
        "meta.json",
        Bridge.string_of_example_meta({
          name: "Intersections",
          description: "",
          entry: "intersections.d.ts",
        }),
      ),
      File(
        "intersections.d.ts",
        {|
// --- Simple intersection (default output mode for intersections is a tuple)
type inters_as_tpl = string & number;
interface ErrorHandling {
  success: boolean;
  error?: { message: string };
}
interface ArtworksData {
  artworks: { title: string }[];
}
interface ArtistsData {
  artists: { name: string }[];
}
type ArtworksResponse = ArtworksData & ErrorHandling;
type ArtistsResponse = ArtistsData & ErrorHandling;
    |},
      ),
    ],
  ),
  // -----------------------------------------------------------------------------
  Folder(
    "type_extraction",
    [
      File(
        "meta.json",
        Bridge.string_of_example_meta({
          name: "Type extraction",
          description: "",
          entry: "type_extraction.d.ts",
        }),
      ),
      File(
        "type_extraction.d.ts",
        {|
// --- Basic type extraction
type obj_1 = { key: string };
interface IExtraction {
  key_1: boolean;
  key_2: obj_1;
}
type extracted_bool = IExtraction['key_1'];
type extracted_string_nested = IExtraction['key_2']['key'];
    |},
      ),
    ],
  ),
  // -----------------------------------------------------------------------------
  Folder(
    "arrays",
    [
      File(
        "meta.json",
        Bridge.string_of_example_meta({
          name: "Arrays",
          description: "",
          entry: "arrays.d.ts",
        }),
      ),
      File(
        "arrays.d.ts",
        {|
// --- Array declarations
type arr_of_bool    = Array<boolean>;
type arr_of_string  = string[];
    |},
      ),
    ],
  ),
  // -----------------------------------------------------------------------------
  Folder(
    "inline_objects",
    [
      File(
        "meta.json",
        Bridge.string_of_example_meta({
          name: "Inline objects",
          description: "",
          entry: "inline_objects.d.ts",
        }),
      ),
      File(
        "inline_objects.d.ts",
        {|
type array_of_inline = Array<{ inline: boolean }>;
interface IInline {
  field_1: string;
  inline: {
    field: number;
    nested: {
      nested_field: boolean;
    }
  }
}
    |},
      ),
    ],
  ),
  // -----------------------------------------------------------------------------
  Folder(
    "tuples",
    [
      File(
        "meta.json",
        Bridge.string_of_example_meta({
          name: "Tuples",
          description: "",
          entry: "tuples.d.ts",
        }),
      ),
      File(
        "tuples.d.ts",
        {|
type tpl_1 = [string, string, number];
type tpl_2 = [boolean, { inline: boolean }];
    |},
      ),
    ],
  ),
  // -----------------------------------------------------------------------------
  Folder(
    "enums",
    [
      File(
        "meta.json",
        Bridge.string_of_example_meta({
          name: "Enums",
          description: "",
          entry: "enums.d.ts",
        }),
      ),
      File("enums.d.ts", {|
export enum enum_1 { Red, Green, Blue };
    |}),
      File(
        "complex_enums.d.ts",
        {|
// Complex enums (e.g. with default values) are not yet implemented
    |},
      ),
    ],
  ),
  // -----------------------------------------------------------------------------
  Folder(
    "literals",
    [
      File(
        "meta.json",
        Bridge.string_of_example_meta({
          name: "Literals",
          description: "",
          entry: "string_literals.d.ts",
        }),
      ),
      File(
        "string_literals.d.ts",
        {|
// --- String literals
// (String literals have 3 config types: Variant, PolyVariant, BsInline. These will be handled differently for generating the encoder / decoder functions.)
type string_literal = "double" | 'single' | '$_bad_string';
type string_literal_opt = 'one' | 'two' | undefined;

// --- Automatic Extension of union literals (keeps around the original)
type string_literal_ext = 'x' | 'y';
type string_literal_ext_base = 'z' | string_literal_ext;
    |},
      ),
      File(
        "numeric_literals.d.ts",
        {|
type num_literal = 2 | 4 | 8 | 12;
    |},
      ),
      File(
        "mixed_literals.d.ts",
        {|
type mixed_literal_1 = 2 | 'string' | 8 | 'other_string';
type mixed_literal_2 = false | 1;
    |},
      ),
    ],
  ),
  // -----------------------------------------------------------------------------
  Folder(
    "unions",
    [
      File(
        "meta.json",
        Bridge.string_of_example_meta({
          name: "Unions",
          description: "",
          entry: "unions.d.ts",
        }),
      ),
      File(
        "unions.d.ts",
        {|
// --- Type unions (Generates bs.unboxed by default)
// (Creating a variant alongside the unboxed value is planned. Still undecided on how to build the classify structure.)
type string_literal = "double" | 'single';

type union_1 = string | number | undefined;
type union_2 = string_literal | string;
    |},
      ),
      File(
        "recursive_unions.d.ts",
        {|
// --- Recursive unions
type union_3 = string | Array<union_3>;
type union_4 = string | { field: union_4 };
    |},
      ),
      File(
        "parameterized_unions.d.ts",
        {|
type ValueOrObj<T> = T | { obj_value: T };
type ValueOrArray<T> = T | Array<ValueOrArray<T>>;
    |},
      ),
    ],
  ),
  // -----------------------------------------------------------------------------
  Folder(
    "type_params",
    [
      File(
        "meta.json",
        Bridge.string_of_example_meta({
          name: "Type parameters",
          description: "",
          entry: "type_params.d.ts",
        }),
      ),
      File(
        "type_params.d.ts",
        {|
type with_param<A> = A;
type use_param = with_param<string>;

interface IParam<A, B, C = string> {
  a: A,
  b: B,
  c: C,
}
interface IParamExt<A, B = number, C = boolean> extends IParam<A,B,C> {}

type use_interface_param = IParamExt<string>; // Default params are lazy, they will be applied at the call site of the parameterized type

interface IParamDefaultInline<P = { is_default: boolean }> {
  key: string;
  p: P;
}
type use_default_inline = IParamDefaultInline;

// --- Type parameters are tracked across generated inlined types
interface IParamNested<A, B> {
  field1: A,
  field2: {
    field3: B
  }
}
    |},
      ),
    ],
  ),
  // -----------------------------------------------------------------------------
  Folder(
    "functions",
    [
      File(
        "meta.json",
        Bridge.string_of_example_meta({
          name: "Functions",
          description: "",
          entry: "functions.d.ts",
        }),
      ),
      File(
        "functions.d.ts",
        {|
// --- Function definitions
declare function some_function(): void;
declare function some_other_function(): { inline: string };
declare function some_function_args(a: string, b: number): string;
declare function some_function_opt_args(a: string, b?: number): string;
declare function some_function_inline_args(a: string | number, b?: { inline: number }): string;
declare function some_function_as_any();
declare function some_function_params<A,B,C>(a: A, b: B): C;
    |},
      ),
      File(
        "inline_functions.d.ts",
        {|
// --- Inline function definitions
interface i_inline_function {
    field: string;
    action: (a: string, b?: number) => void;
}
type i_inline_access = i_inline_function['action'];
    |},
      ),
      File(
        "method_signatures.d.ts",
        {|
// --- Method signature in interfaces
interface method_signature {
  func1: (x: number) => number;       // Function type literal
  func2(x: number): number;           // Method signature
  func3(x: string, y: boolean): void; // Method signature for extraction
}
type extracted_method_signature = method_signature['func3'];
    |},
      ),
    ],
  ),
  // -----------------------------------------------------------------------------
  Folder(
    "imports",
    [
      File(
        "meta.json",
        Bridge.string_of_example_meta({
          name: "Imports",
          description: "",
          entry: "imports.d.ts",
        }),
      ),
      File(
        "imports.d.ts",
        {|
// --- Simple imports
import IsOk from './import_source';
import { str } from './import_source';
import * as Others from './import_source';

type should_be_bool = IsOk;
type should_be_string = str;
type should_be_obj = Others.iImport;
    |},
      ),
      File(
        "import_source.d.ts",
        {|
export type str = string;
interface iImport {
    get: (a: number) => void;
}

type b = boolean;
export default b;
    |},
      ),
    ],
  ),
  // -----------------------------------------------------------------------------
];

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

let register_examples = () => {
  let rec walk = (~path=Fs.base_path, folders_or_files) => {
    folders_or_files
    |> CCList.iter(
         fun
         | Folder(name, children) =>
           walk(~path=Fp.append(path, name), children)
         | File(name, content) =>
           Js_of_ocaml.Sys_js.create_file(
             ~name=Fp.append(path, name) |> Fp.toString,
             ~content,
           ),
       );
  };
  walk(examples);
};

module File = CCIO.File;

let rec example_tree = (~exclude=[], path) => {
  Fs.list_of_dir(path)
  |> CCList.filter_map(sub_path => {
       let sub_path_str = sub_path |> Fp.toString;
       let base_name = Fp.baseName(sub_path) |> CCOpt.get_exn;
       if (exclude |> CCList.exists(CCEqual.string(base_name))) {
         None;
       } else {
         Some(
           File.is_directory(sub_path_str)
             ? Bridge.Folder((base_name, example_tree(sub_path)))
             : Bridge.File(base_name),
         );
       };
     });
};

let examples_list = () =>
  Bridge.string_of_example_list(
    try({
      let entries = Fs.list_of_dir(Fs.base_path);
      entries
      |> CCList.map(example_path => {
           let example_path_str = example_path |> Fp.toString;
           {
             Bridge.path: example_path_str,
             meta:
               Bridge.example_meta_of_string(
                 Sys_js.read_file(
                   ~name=Fp.append(example_path, "meta.json") |> Fp.toString,
                 ),
               ),
             files: example_tree(~exclude=["meta.json"], example_path),
           };
         });
    }) {
    | e => []
    },
  );

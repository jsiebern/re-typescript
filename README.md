# re-typescript

An opinionated attempt at finally solving typescript interop for `ReasonML` / `OCAML`.

## Why

I am maintaining a package of automatically generated bindings for `Material UI`. Generating these bindings is becoming more and more frustrating, as the complexity of the original package grows and tools that wish to cover everything begin to break down. this leads to more and more types missing on each cycle and precious time spent on recovering them by adjusting the type extraction process. Right now the generator uses a library which converts `typescript` into a `json-schema` which then get's analyzed & parsed into reason code. I believe that these tools want to cover too many use cases in order to be effective. So I'm setting out to simplify this process and maybe provide an effective way of generalizing type mapping between `reason` and `typescript`.

Quite ambitious. We'll see.

## Idea

`re-typescript` implements it's own lexer / parser to analyse typescript declaration files specifically. It purposefully does **not** want to get into tracking inferred types through `*.ts` files. It only extracts cleanly defined types as best as it can.

I am not sure about how to handle unparseable code yet. I believe a _best approximation_ appraoch will be best, where it will gracefully end the traversal by just injecting an abstract type of some sort, instead of failing or fully omitting a type.

`re-typescript` first derives a primitive AST from the tokenization process. It then decodes that AST by way of resolving references / dependencies. Finally it directly generates an ocaml AST, which allows for easy printing of both `reason` & `ocaml`. I'm composing the ast by using `ocaml-migrate-parsetree`, which will later allow for an easy use inside of a potential `PPX`.

This tool will be opinionated about a few things. Typescripts dynamic nature makes this unavoidable. I am not yet sure how the native mode will generate it's types exactly (esp. regarding unions / mixed enums). Bucklescript has great features for handling these things, like the `@unboxed` attribute, as well as `@bs.string` for externals and `bs.as` in records.

## Playground

I will update the playground from time to time. It's not great yet, I just wanted a visual representation for now. You can see the current progress here:
[re-typescript](https://jsiebern.github.io/re-typescript/)

## Todo

- [ ] Lexing / Parsing

  - [ ] Include lexing positions for better error messages
  - [ ] Use an identifier hash table to avoid crashing on large files
  - [ ] Basic
    - [x] Any
    - [x] Void
    - [x] Boolean
    - [x] Number
    - [x] String
    - [x] Null
    - [x] Undefined
    - [x] Reference
      - [ ] Special modifier keywords (`e.g. Required<{...}>`)
    - [ ] Type extraction (`e.g. x['field']`)
  - [x] Array
  - [ ] Tuple
  - [ ] Union
  - [ ] Enum
    - [ ] Keys
    - [ ] Default values
  - [ ] Interfaces / Objects
    - [x] Keyword
    - [x] Keys
    - [x] Semicolon / Comma
    - [x] Optional
    - [x] Readonly
    - [ ] Dynamic keys
    - [x] Extends keyword
  - [ ] Functions
    - [ ] By keyword
    - [ ] Arrow function
    - [ ] Arguments
    - [ ] Return type
  - [ ] Classes
    - [ ] Properties
    - [ ] Static
    - [ ] Public / Protected / Private
    - [ ] Methods
    - [ ] Inheritance
    - [ ] Implementations
    - [ ] Short fields initialization
  - [x] Imports
    - [x] Named
    - [x] Aliased
    - [x] Star aliased
    - [x] List of named
  - [ ] Exports
  - [ ] Generics
    - [ ] Type definition
    - [ ] Interfaces
    - [ ] Classes
    - [ ] Extends keyword

- [ ] Decoding
  - [ ] Create representation
    - [x] Basic types
    - [ ] Interfaces
    - [ ] Arrays / Lists
    - [ ] Enums
    - [ ] Unions
    - [ ] Functions
    - [ ] Classes
  - [x] Abstracted file loader to resolve import / exports (should work both on web / native)
  - [ ] Resolve extension refs for the current type (finalize all fields)
  - [ ] Flattened type tree
- [ ] Generating
  - [ ] Valid identifiers (Reserved keywords / uppercase variables)
  - [ ] Native (Generate types without bucklescript features)
    - [ ] Basic types
    - [ ] Interfaces
    - [ ] Arrays / Lists
    - [ ] Enums
    - [ ] Unions
    - [ ] Functions
    - [ ] Classes
  - [ ] Bucklescript (Make use of bucklescript features, but do not use externals)
    - [ ] Basic types
    - [ ] Interfaces
    - [ ] Arrays / Lists
    - [ ] Enums
    - [ ] Unions
    - [ ] Functions
    - [ ] Classes
  - [ ] Bucklescript Bindings (Full use of bucklescript features and sensible approach of providing externals)
    - [ ] Basic types
    - [ ] Interfaces
    - [ ] Arrays / Lists
    - [ ] Enums
    - [ ] Unions
    - [ ] Functions
    - [ ] Classes
    - [ ] Exports as externals
- [ ] Playground
  - [ ] Check if it's really necessary to pull half of the ocaml compiler in
  - [ ] Create a UI
  - [ ] Add syntax highlighting
  - [ ] Keep source in local storage
  - [ ] Add reason / ocaml switch
  - [ ] Download file button
- [ ] Config / Misc
  - [ ] Think of a way to inject overrides (e.g. a Hashmap of type name regexes and a replacement)
  - [ ] Provide better error messages on syntax error
  - [ ] `array_mode` (Array / List)
  - [ ] `number_mode` (Int / Float / Unboxed)
  - [ ] PPX (far future)
    - ```reason
      [%re_typescript "package/def.d.ts"]]
      ```
    - ```reason
      [%re_typescript "package/def.d.ts"(["a_type", "another_type"])]]
      ```
    - ```reason
      [%re_typescript {| interface { field: string; } |}]]
      ```

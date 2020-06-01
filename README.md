# re-typescript

An opinionated attempt at finally solving typescript interop for `ReasonML` / `OCaml`.

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

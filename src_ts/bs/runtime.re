open Ts_morph;

let project =
  Project.make(
    Some(
      Project.Config.make(
        ~useInMemoryFileSystem=true,
        ~compilerOptions={
          lib: [|"lib", "lib.es2015.collection.d.ts"|],
          noLib: false,
          importHelpers: true,
        },
        (),
      ),
    ),
  );

let typeChecker = project->Project.getTypeChecker->TypeChecker.compilerObject;

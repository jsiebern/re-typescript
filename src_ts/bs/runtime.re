open Ts_morph;

let project =
  Project.make(Some(Project.Config.make(~useInMemoryFileSystem=true, ())));

let typeChecker = project->Project.getTypeChecker->TypeChecker.compilerObject;

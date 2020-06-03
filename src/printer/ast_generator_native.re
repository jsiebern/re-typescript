open Migrate_parsetree;

let generate = (~ctx, _) =>
  raise(
    Tree_utils.Exceptions.Parser_unexpected(
      "Cannot generate native mode yet",
    ),
  );

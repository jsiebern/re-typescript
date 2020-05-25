%%

%public let separated_or_terminated_list(separator, X) :=
  | x=X; { [x] }
  | x=X; separator; { [x] }
  | x=X; separator; xs=separated_or_terminated_list(separator, X); { x :: xs }

%public let maybe_as_bool(X) :=
  v = X?; { match v with | None -> false | Some _ -> true }

%public let maybe_as_list(X) :=
    v = X?; { match v with | None -> [] | Some v -> v }
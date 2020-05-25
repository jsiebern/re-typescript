%%

%public let literal_string :=
  | s = STRING; { let (s,_,p) = s in `String(s, p) }

%public let literal_number :=
  | n = NUMBER; { `Number(n) }
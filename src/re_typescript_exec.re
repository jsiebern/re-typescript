let test = (
  "/test.d.ts",
  {|

type opt_in_arr = (string | undefined)[];
type null_in_arr = Array<boolean | null>;
type arr_null = string[] | null;
type arr_opt = Array<boolean> | undefined;

// ------------------------------------------------------

|},
);

switch (Re_typescript_lib.quick_parse([|test|])) {
| Error(e) => raise(Failure(e))
| Ok(parsed) => Console.log(parsed)
};

Console.log(
  "\n-------------------------------------------------------------------\n",
);

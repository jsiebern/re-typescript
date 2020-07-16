let test = (
  "/test.d.ts",
  {|

type f =  number;

namespace Package {
  type x = string;
  type y = x;
  type ff = f;

  type ssad<B> = {
    field: {
      second: string,
    },
    B:B,
    inner: A
  }
  type yyy = ssad<string>['field']['second'];
}


|},
);
switch (Re_typescript_lib.quick_parse([|test|])) {
| Error(e) => raise(Failure(e))
| Ok(parsed) => Console.log(parsed)
};

Console.log(
  "\n-------------------------------------------------------------------\n",
);

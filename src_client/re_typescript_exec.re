let test = (
  "/test.d.ts",
  {|

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

      // These interfaces are composed to have
      // consistent error handling, and their own data.

      type ArtworksResponse = ArtworksData & ErrorHandling;
      type ArtistsResponse = ArtistsData & ErrorHandling;

|},
);

switch (Re_typescript_lib.quick_parse([|test|])) {
| Error(e) => raise(Failure(e))
| Ok(parsed) => Console.log(parsed)
};

Console.log(
  "\n-------------------------------------------------------------------\n",
);

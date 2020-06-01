open TestFramework;

describe("intersection types", ({test, _}) => {
  test("can create simple intersection tuple", ({expect, _}) => {
    expect.string(print({|
      type tpl = string & number;
    |})).
      toMatchSnapshot()
  });
  test("can create simple intersection tuple with references", ({expect, _}) => {
    expect.string(
      print(
        {|
      type num_opt = number | undefined
      type tpl = string & num_opt;
    |},
      ),
    ).
      toMatchSnapshot()
  });
  test("can create more complex intersection tuple", ({expect, _}) => {
    expect.string(
      print(
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
      ),
    ).
      toMatchSnapshot()
  });
});

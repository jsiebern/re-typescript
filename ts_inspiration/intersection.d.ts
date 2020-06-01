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

export const handleArtistsResponse: (response: ArtistsResponse) => void;

// -----------------------------------

// interface A {
//   a: number;
// }
// interface B {
//   b: number;
// }

// var ab: A & B = { a: 1, b: 1 };
// var a: A = ab; // A & B assignable to A
// var b: B = ab; // A & B assignable to B

// interface X {
//   p: A;
// }
// interface Y {
//   p: B;
// }

// var xy: X & Y = { p: ab }; // X & Y has property p of type A & B

// type F1 = (a: string, b: string) => void;
// type F2 = (a: number, b: number) => void;

// type F3 = F1 & F2;
// var f: (a: string | number, b: string | number) => {};

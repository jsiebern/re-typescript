declare class Person {
  constructor(name: string);
}

interface Loggable {
  log(name: string): void;
}

declare class ConsoleLogger implements Loggable {
  log(name: string): void;
}

// Takes two objects and merges them together
declare function extend<First extends {}, Second extends {}>(
  first: First,
  second: Second
): First & Second;

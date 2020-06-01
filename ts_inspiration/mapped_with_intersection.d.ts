type PartialWithNewMember<T> = {
  [P in keyof T]?: T[P];
} & { newMember: boolean };

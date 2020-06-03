declare namespace React {
  export type ReactType<TProps> = TProps;
  export type ReactNode = string;
  export type ReactNodeArray = string[];
  export type ReactElement<T> = number;
}

export type SemanticShorthandItemFunc<TProps> = (
  component: React.ReactType<TProps>,
  props: TProps,
  children?: React.ReactNode | React.ReactNodeArray
) => React.ReactElement<any> | null;

export type SemanticShorthandCollection<TProps> = SemanticShorthandItem<
  TProps
>[];
export type SemanticShorthandContent = React.ReactNode;
export type SemanticShorthandItem<TProps> =
  | React.ReactNode
  | TProps
  | SemanticShorthandItemFunc<TProps>;

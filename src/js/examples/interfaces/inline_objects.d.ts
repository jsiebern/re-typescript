type array_of_inline = Array<{ inline: boolean }>;
interface IInline {
  field_1: string;
  inline: {
    field: number;
    nested: {
      nested_field: boolean;
    };
  };
}

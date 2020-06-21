type with_param<A> = A;
type use_param = with_param<string>;

interface IParam<A, B, C = string> {
  a: A;
  b: B;
  c: C;
}
interface IParamExt<A, B = number, C = boolean> extends IParam<A, B, C> {}

type use_interface_param = IParamExt<string>; // Default params are lazy, they will be applied at the call site of the parameterized type

interface IParamDefaultInline<P = { is_default: boolean }> {
  key: string;
  p: P;
}
type use_default_inline = IParamDefaultInline;

// --- Type parameters are tracked across generated inlined types
interface IParamNested<A, B> {
  field1: A;
  field2: {
    field3: B;
  };
}

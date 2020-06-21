// --- Interface extensions are resolved properly
// --- Created a configurable warning suppression ([@warning "-30"]) for overlapping fields in extensions
interface Extend_me<Fill, Default = boolean> {
  field_1: Fill;
  field_2: Default;
  field_4: string;
}
interface Extends_one extends Extend_me<string> {
  field_3: number;
}
interface Extends_another<PassThrough> extends Extend_me<PassThrough> {
  field_3: PassThrough;
}

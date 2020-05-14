let content = {|
import { Palette } from './createPalette';
import * as React from 'react';
import { CSSProperties } from './withStyles';

type x = string;
type y = number;
type someObj = {
    some: boolean,
    other: string,
    rec: someObj,
};
interface next extends React.CSSProperties<Required<{
  fontFamily: string;
  nested: React.SomeOther<{color: number},string,boolean>;
}>> {
    has: number;
    obj: somObj;
};


|};

module X = {
  [@react.component]
  let make = () => {
    let (v, setV) = React.useReducer((_, v) => v, content);

    <>
      <textarea
        value=v
        onChange={e => setV(e->ReactEvent.Form.target##value)}
      />
      <div className="display">
        <pre> {BsPrinter.print(v)->React.string} </pre>
      </div>
    </>;
  };
};

ReactDOMRe.renderToElementWithId(<X />, "app");

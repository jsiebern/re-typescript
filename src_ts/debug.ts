import { Node, Type, Symbol } from 'ts-morph';
type loggableTypes = Node | Type | Symbol;
let getId = (t: any) => (t['id'] != null ? t['id'] : -1);

export const enum LogLevel {
  None = 1 << 3,
  Info = 1 << 2,
  Debug = 1 << 1,
  Warning = 1 << 0,
  Error = 0,
}

const getLoggingFunc = (logLevel: LogLevel) => {
  switch (logLevel) {
    case LogLevel.Info:
      return console.info;
    case LogLevel.Debug:
      return console.debug;
    case LogLevel.Warning:
      return console.warn;
    case LogLevel.Error:
      return console.error;
    default:
      return (...a: any[]) => {};
  }
};

let currentLogLevel = LogLevel.Error;
export function setLogLevel(level: LogLevel) {
  currentLogLevel = level;
}

export function log(
  message: string | string[],
  type_or_level_or_array?:
    | loggableTypes
    | loggableTypes[]
    | LogLevel
    | undefined,
  maybe_level: LogLevel = LogLevel.Debug
): void {
  return;
  // let types: Array<loggableTypes> | undefined;
  // let logLevel: LogLevel = LogLevel.None;
  // if (Array.isArray(type_or_level_or_array)) {
  //   types = type_or_level_or_array;
  // } else if (type_or_level_or_array instanceof Object) {
  //   types = [type_or_level_or_array];
  // } else if (typeof type_or_level_or_array === 'undefined') {
  //   types = undefined;
  // } else {
  //   logLevel = type_or_level_or_array;
  // }
  // if (typeof maybe_level !== 'undefined') {
  //   logLevel = maybe_level;
  // }

  // if (logLevel <= currentLogLevel) {
  //   console.group(logLevel);
  //   const log = getLoggingFunc(logLevel);
  //   (!Array.isArray(message) ? [message] : message).forEach(log);
  //   if (Array.isArray(types)) {
  //     types.forEach((t) => {
  //       if (Node.isNode(t)) {
  //         log('NODE', getId(t.compilerNode), t.getKindName(), t.getText());
  //       } else if (t instanceof Symbol) {
  //         log(
  //           'SYMBOL',
  //           getId(t.compilerSymbol),
  //           t.getName(),
  //           t.getDeclaredType().getText()
  //         );
  //       } else if (t instanceof Type) {
  //         log('TYPE', getId(t.compilerType), t.getText());
  //       }
  //     });
  //     console.groupEnd();
  //     log('------------------------------------------------------------');
  //   }
  // }
}

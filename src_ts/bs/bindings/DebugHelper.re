let debugClean: (Js.Json.t, int) => string = [%raw
  {|
function doClean(v, level = 1) {
  if (level >= 3) {
    return "circular";
  }
  if (typeof v === "undefined") {
    return "undefined";
  }
  if (Array.isArray(v)) {
    return (v.map((v) => doClean(v, level + 1)));
  }
  if (typeof v === "object") {
    let o = {};
    Object.keys(v).reduce((o, k) => {
      o[k] = doClean(v[k], level + 1);
      return o;
    }, o);
    return JSON.stringify(o);
  }
  return JSON.stringify(v);
}
|}
];

Js_of_ocaml.Js.Unsafe.eval_string(
  {|
    if (typeof JSON.decycle !== "function") {
    JSON.decycle = function decycle(object, replacer) {
        "use strict";

// Make a deep copy of an object or array, assuring that there is at most
// one instance of each object or array in the resulting structure. The
// duplicate references (which might be forming cycles) are replaced with
// an object of the form

//      {"$ref": PATH}

// where the PATH is a JSONPath string that locates the first occurance.

// So,

//      var a = [];
//      a[0] = a;
//      return JSON.stringify(JSON.decycle(a));

// produces the string '[{"$ref":"$"}]'.

// If a replacer function is provided, then it will be called for each value.
// A replacer function receives a value and returns a replacement value.

// JSONPath is used to locate the unique object. $ indicates the top level of
// the object or array. [NUMBER] or [STRING] indicates a child element or
// property.

        var objects = new WeakMap();     // object to path mappings

        return (function derez(value, path) {

// The derez function recurses through the object, producing the deep copy.

            var old_path;   // The path of an earlier occurance of value
            var nu;         // The new object or array

// If a replacer function was provided, then call it to get a replacement value.

            if (replacer !== undefined) {
                value = replacer(value);
            }

// typeof null === "object", so go on if this value is really an object but not
// one of the weird builtin objects.

            if (
                typeof value === "object"
                && value !== null
                && !(value instanceof Boolean)
                && !(value instanceof Date)
                && !(value instanceof Number)
                && !(value instanceof RegExp)
                && !(value instanceof String)
            ) {

// If the value is an object or array, look to see if we have already
// encountered it. If so, return a {"$ref":PATH} object. This uses an
// ES6 WeakMap.

                old_path = objects.get(value);
                if (old_path !== undefined) {
                    return {$ref: old_path};
                }

// Otherwise, accumulate the unique value and its path.

                objects.set(value, path);

// If it is an array, replicate the array.

                if (Array.isArray(value)) {
                    nu = [];
                    value.forEach(function (element, i) {
                        nu[i] = derez(element, path + "[" + i + "]");
                    });
                } else {

// If it is an object, replicate the object.

                    nu = {};
                    Object.keys(value).forEach(function (name) {
                        nu[name] = derez(
                            value[name],
                            path + "[" + JSON.stringify(name) + "]"
                        );
                    });
                }
                return nu;
            }
            return value;
        }(object, "$"));
    };
}


if (typeof JSON.retrocycle !== "function") {
    JSON.retrocycle = function retrocycle($) {
        "use strict";

// Restore an object that was reduced by decycle. Members whose values are
// objects of the form
//      {$ref: PATH}
// are replaced with references to the value found by the PATH. This will
// restore cycles. The object will be mutated.

// The eval function is used to locate the values described by a PATH. The
// root object is kept in a $ variable. A regular expression is used to
// assure that the PATH is extremely well formed. The regexp contains nested
// * quantifiers. That has been known to have extremely bad performance
// problems on some browsers for very long strings. A PATH is expected to be
// reasonably short. A PATH is allowed to belong to a very restricted subset of
// Goessner's JSONPath.

// So,
//      var s = '[{"$ref":"$"}]';
//      return JSON.retrocycle(JSON.parse(s));
// produces an array containing a single element which is the array itself.

        var px = /^\$(?:\[(?:\d+|"(?:[^\\"\u0000-\u001f]|\\(?:[\\"\/bfnrt]|u[0-9a-zA-Z]{4}))*")\])*$/;

        (function rez(value) {

// The rez function walks recursively through the object looking for $ref
// properties. When it finds one that has a value that is a path, then it
// replaces the $ref object with a reference to the value that is found by
// the path.

            if (value && typeof value === "object") {
                if (Array.isArray(value)) {
                    value.forEach(function (element, i) {
                        if (typeof element === "object" && element !== null) {
                            var path = element.$ref;
                            if (typeof path === "string" && px.test(path)) {
                                value[i] = eval(path);
                            } else {
                                rez(element);
                            }
                        }
                    });
                } else {
                    Object.keys(value).forEach(function (name) {
                        var item = value[name];
                        if (typeof item === "object" && item !== null) {
                            var path = item.$ref;
                            if (typeof path === "string" && px.test(path)) {
                                value[name] = eval(path);
                            } else {
                                rez(item);
                            }
                        }
                    });
                }
            }
        }($));
        return $;
    };
}
    |},
)
|> ignore;

let type_to_json = (t: Ts_nodes.Type.t) => {
  Console.log(
    Js_of_ocaml.Js.(
      Unsafe.fun_call(
        Unsafe.js_expr("JSON.stringify"),
        [|
          Unsafe.fun_call(
            Unsafe.js_expr("JSON.decycle"),
            [|Unsafe.inject(t |> Ts_nodes.Type.t_to_js)|],
          ),
          Unsafe.inject(null),
          Unsafe.inject(2),
        |],
      )
    ),
  );
};

let node_to_json = (node: Ts_nodes.Generic.t) => {
  Console.log(
    Js_of_ocaml.Js.(
      Unsafe.fun_call(
        Unsafe.js_expr("JSON.stringify"),
        [|
          Unsafe.fun_call(
            Unsafe.js_expr("JSON.decycle"),
            [|Unsafe.inject(node |> Ts_nodes.Generic.t_to_js)|],
          ),
          Unsafe.inject(null),
          Unsafe.inject(2),
        |],
      )
    ),
  );
};
let nodeKind_to_json = (node: Ts_nodes.nodeKind) =>
  node_to_json(Ts_nodes_util.unwrap_identified(node));

let obj_keys = v =>
  Js_of_ocaml.Js.(
    Unsafe.fun_call(
      Unsafe.js_expr("JSON.stringify"),
      [|
        Unsafe.fun_call(
          Unsafe.js_expr("Object.keys"),
          [|Unsafe.inject(v)|],
        ),
      |],
    )
  );

let obj_prop_names = v =>
  Js_of_ocaml.Js.(
    Unsafe.fun_call(
      Unsafe.js_expr("JSON.stringify"),
      [|
        Unsafe.fun_call(
          Unsafe.js_expr("Object.getOwnPropertyNames"),
          [|
            Unsafe.inject(
              Unsafe.fun_call(
                Unsafe.js_expr("Object.getPrototypeOf"),
                [|Unsafe.inject(v)|],
              ),
            ),
          |],
        ),
        Unsafe.inject(null),
        Unsafe.inject(2),
      |],
    )
  );

let log = v =>
  Console.log(
    Js_of_ocaml.Js.(
      Unsafe.fun_call(
        Unsafe.js_expr("JSON.stringify"),
        [|
          Unsafe.fun_call(
            Unsafe.js_expr("JSON.decycle"),
            [|Unsafe.inject(v)|],
          ),
          Unsafe.inject(null),
          Unsafe.inject(2),
        |],
      )
    ),
  );

const fs = require("fs");
const path = require("path");

var walk = function (dir, done) {
  var results = [];
  fs.readdir(dir, function (err, list) {
    if (err) return done(err);
    var pending = list.length;
    if (!pending) return done(null, results);
    list.forEach(function (file) {
      file = path.resolve(dir, file);
      fs.stat(file, function (err, stat) {
        if (stat && stat.isDirectory()) {
          walk(file, function (err, res) {
            results = results.concat(res);
            if (!--pending) done(null, results);
          });
        } else {
          results.push(file);
          if (!--pending) done(null, results);
        }
      });
    });
  });
};

async function get_example_files(path) {
  return new Promise((resolve, reject) => {
    walk(path, (_, b) => {
      resolve(b);
    });
  });
}

async function app() {
  const base_path = `${__dirname}/examples`;
  const examples = fs.readdirSync(base_path);

  let examples_rendered = await Promise.all(
    examples.map(async (example_name) => {
      const example_path = path.join(base_path, example_name);
      const subs = await get_example_files(example_path);
      return [
        example_name,
        await Promise.all(
          subs.map((sub) => [
            sub.replace(`${example_path}/`, ""),
            fs.readFileSync(sub),
          ])
        ),
      ];
    })
  );
  examples_rendered = examples_rendered
    .map(
      ([name, files]) => `
    Folder(
      "${name}",
      [
        ${files
          .map(
            ([name, content]) => `
        File(
          "${name}",
          {|${content}|},
        )
        `
          )
          .join(",\n")}
      ]
    )
    `
    )
    .join(",\n");

  const src = `
  type tree =
    | Folder(string, list(tree))
    | File(string, string);
  let examples = [
    ${examples_rendered}
  ];
  `;

  fs.writeFileSync("examples.re", src);
}

app();

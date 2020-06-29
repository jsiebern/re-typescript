const fs = require("fs");
const path = require("path");
const { argv } = require("process");

function copyFile(source, destination) {
  return new Promise((resolve, reject) => {
    fs.exists(source, (exists) => {
      if (!exists) {
        reject(`Path to ${source} not resolved successfully`);
      } else {
        fs.copyFile(source, destination, (res) => {
          resolve(res);
        });
      }
    });
  });
}

async function app() {
  const base_path = path.resolve(
    __dirname.split("re-typescript/")[0] + "re-typescript/",
    "src_ts",
    "bs",
    "types"
  );
  const copyPaths = [
    [
      path.join(base_path, "typescript.atd"),
      path.join(__dirname, "typescript.atd"),
    ],
    [
      path.join(base_path, "typescript_flags.re"),
      path.join(__dirname, "typescript_flags.re"),
    ],
  ];
  await Promise.all(
    copyPaths.map(([source, destination]) => copyFile(source, destination))
  );
}

app();

include Rely.Make({
  let config =
    Rely.TestFrameworkConfig.initialize({
      snapshotDir: "tests/native/__snapshots__",
      projectDir: "src",
    });
});

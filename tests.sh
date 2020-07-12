#!/bin/bash
set -e
esy tests
node ./_build/default/tests/native/RunTests.bc.js
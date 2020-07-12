#!/bin/bash

set -e
if [ "$1" ==  "build" ]
then
    esy tests --profile release-static
else
    esy tests
    $(fnm exec -- which node) ./_build/default/tests/native/RunTests.bc.js 
fi

#!/bin/bash

set -e
if [ "$1" ==  "build" ]
then
    esy tests --profile release-static
else
    esy tests
    U=""
    F="$@"
    if [ "$1" ==  "u" ]
    then
        U=" -u "
        F="${@:2}"
    fi
    $(fnm exec -- which node) ./_build/default/tests/RunTests.bc.js --filter "$(echo $F)" $U
fi

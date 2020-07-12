#!/bin/bash

set -e
if [ "$1" ==  "build" ]
then
    esy dev --profile release-static
else
    if [ "$1" ==  "watch" ]
    then
        esy dev-watch
    else
        esy dev
        $(fnm exec -- which node) ./_build/default/src_client/re_typescript_exec.bc.js 
    fi
fi
#!/bin/bash

set -e
if [ "$1" ==  "build" ]
then
    esy dev --profile release-static
else
    if [ "$1" ==  "watch" ]
    then
        npx redemon --paths=src --extensions=re,atd,mll,mly ./dev.sh
    else
        esy dev
        $(fnm exec -- which node) ./_build/default/src/re_typescript_exec.bc.js 
    fi
fi
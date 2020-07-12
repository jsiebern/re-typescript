#!/bin/bash
set -e
esy client
node ./_build/default/src_client/re_typescript_exec.bc.js
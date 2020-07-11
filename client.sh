#!/bin/bash
set -e
esy client
node ./_build/default/src_client/src_client.bc.js | prettyjson
#!/bin/bash
esy js-release
./types.sh
yarn fpack
yarn fpack-worker
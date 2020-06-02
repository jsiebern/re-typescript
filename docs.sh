#!/bin/bash
esy js-release
yarn types
yarn fpack
yarn fpack-worker
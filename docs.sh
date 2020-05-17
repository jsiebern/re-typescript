#!/bin/bash
esy js-release
yarn bs:build
yarn fpack
yarn fpack-worker
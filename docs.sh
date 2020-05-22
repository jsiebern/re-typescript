#!/bin/bash
esy js
yarn bs:build
yarn fpack
yarn fpack-worker
#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
NAPKIN="$DIR/napkinscript.exe"

TARGET_FILE="$(cd "$(dirname "$1")"; pwd -P)/$(basename "$1")";
TEMP_FILE="${TARGET_FILE}_temp";

$NAPKIN -parse ns -print ns $TARGET_FILE > $TEMP_FILE
rm $TARGET_FILE
mv $TEMP_FILE $TARGET_FILE
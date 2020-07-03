#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
NAPKIN="$DIR/napkinscript.exe"

TARGET_FILE="$(cd "$(dirname "$1")"; pwd -P)/$(basename "$1")";
TEMP_FILE="$TARGET_FILE_temp";

$($NAPKIN) --help
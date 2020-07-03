#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
FORMAT="$DIR/format.sh"

for file in $(find $1 -type f -name "*.res")
do
  $FORMAT $file
done 

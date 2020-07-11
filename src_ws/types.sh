#!/bin/bash
atdg=`esy which atdgen`

$atdg types/ws_messages.atd -t
$atdg types/ws_messages.atd -bs
$atdg types/typescript.atd -t
$atdg types/typescript.atd -bs

cp types/ws_messages.atd ./../src/ws_types
cp types/typescript.atd ./../src/ws_types
cp types/typescript.atd ./../src/ts_parser
cp types/typescript_flags.re ./../src/ts_parser

yarn bs:build
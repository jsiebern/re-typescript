#!/bin/bash
atdg=`esy which atdgen`
rm src_js/types/*.ml*
rm src_js/types/*.atd
cp src/config/*.atd src_js/types
$atdg src_js/types/config.atd -t
$atdg src_js/types/config.atd -bs
$atdg src_js/types/bridge.atd -t
$atdg src_js/types/bridge.atd -bs
$atdg src_js/types/parse_info.atd -t
$atdg src_js/types/parse_info.atd -bs
yarn bs:build
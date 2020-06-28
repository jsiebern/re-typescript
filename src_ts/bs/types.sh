#!/bin/bash
atdg=`esy which atdgen`
$atdg types/typescript.atd -t
$atdg types/typescript.atd -bs
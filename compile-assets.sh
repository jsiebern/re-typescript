#!/bin/bash
pwd=$(pwd)

cd src/base
ocamllex lexer.mll
menhir parser.mly

mv *.ml $pwd/src_js/vendor
mv *.mli $pwd/src_js/vendor
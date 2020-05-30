#!/bin/bash

menhir \
    parser.mly \
    parser_tokens.mly \
    --base parser \
    --dump --explain --unused-tokens
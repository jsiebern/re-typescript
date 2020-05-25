#!/bin/bash

menhir \
    parser_new.mly \
    parser_utils.mly \
    parser_tokens.mly \
    parser_enums.mly \
    parser_functions.mly \
    parser_identifiers.mly \
    parser_interfaces.mly \
    parser_literals.mly \
    parser_namespaces.mly \
    parser_types.mly \
    --base parser_new \
    --dump --explain --unused-tokens
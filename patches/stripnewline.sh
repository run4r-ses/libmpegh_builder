#!/bin/bash
CLEAN_CMD=$(echo "$@" | tr -d '\r' | tr '\n' ' ' | sed 's/\\/ /g' | sed 's/  */ /g')
echo "$CLEAN_CMD"
eval "$CLEAN_CMD"

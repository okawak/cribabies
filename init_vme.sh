#!/bin/bash

current=$(
  cd "$(dirname "$0")" || exit 1
  pwd
)

if [ -f "$current/modulelist.yaml" ]; then
  poetry run python "$current/init_modules/src/init_modules.py" "$current/modulelist.yaml"
else
  echo 'Please confirm "modulelist.yaml" is in the current path.'
fi

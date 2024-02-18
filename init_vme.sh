#!/bin/bash

current=$(
  cd "$(dirname "$0")" || exit 1
  pwd
)

if [ -f "$current/modulelist.yaml" ]; then
  cd "$current/init_modules" || exit 1
  poetry run python "src/init_modules.py" "$current/modulelist.yaml"
  cd - || exit 1
else
  echo "Please confirm \"modulelist.yaml\" is in the cribabies directory."
fi

#!/bin/bash

current=$(cd $(dirname $0);pwd)

if [ -f "$current/../modulelist.yml" ] ;then
  source $current/../.venv/bin/activate
  python3 $current/../init_module/main.py $current/../init_module/modulelist.yml
else
  echo 'Please confirm "modulelist.yml" is in the current path.'
fi

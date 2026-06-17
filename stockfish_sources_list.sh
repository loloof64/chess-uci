#!/usr/bin/env bash

echo '"sources": ['
echo '  "native/cpp/addon.cpp",'
echo '  "native/cpp/engine_wrapper.cpp",'

find native/cpp/stockfish/src \
  -type f \
  -name '*.cpp' \
  ! -name 'main.cpp' \
  | sort \
  | while read -r file; do
      echo "  \"$file\","
    done

echo ']'
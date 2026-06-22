#!/usr/bin/env bash

echo '"sources": ['
echo '  "native/cpp/addon.cpp",'
echo '  "native/cpp/engine_wrapper.cpp",'
echo '  "native/cpp/stockfish_init.cpp",'
echo '  "native/cpp/stockfish_runner.cpp",'

find native/cpp/stockfish/src \
  -type f \
  -name '*.cpp' \
  ! -name 'main.cpp' \
  | sort \
  | while read -r file; do
      echo "  \"$file\","
    done

echo ']'
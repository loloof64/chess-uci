#!/usr/bin/env bash

set -e

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"

VARIANT=${1:-generic}

echo "Building Stockfish variant: $VARIANT"

# Select gyp file
cp \
  "$ROOT_DIR/binding.$VARIANT.gyp" \
  "$ROOT_DIR/binding.gyp"

cd "$ROOT_DIR"


# Clean previous build
rm -rf build


# Configure
node-gyp configure


# Build
node-gyp build


# Store result
mkdir -p \
  "$ROOT_DIR/native/linux-$VARIANT"


cp \
  "$ROOT_DIR/build/Release/stockfish.node" \
  "$ROOT_DIR/native/linux-$VARIANT/stockfish.node"


echo "Done:"
echo "native/linux-$VARIANT/stockfish.node"
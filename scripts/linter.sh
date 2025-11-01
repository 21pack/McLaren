#!/bin/sh -e

BASEDIR=$(realpath "$(dirname "$0")")
ROOTDIR=$(realpath "$BASEDIR/..")

find "$ROOTDIR/engine"  \( -name '*.cpp' -o -name '*.h' \) -print0 \
    | xargs -0 clang-tidy -p build/
find "$ROOTDIR/game" \( -name '*.cpp' -o -name '*.h' \) -print0 \
    | xargs -0 clang-tidy -p build/

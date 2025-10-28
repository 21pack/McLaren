#!/bin/sh -e

BASEDIR=$(realpath "$(dirname "$0")")
ROOTDIR=$(realpath "$BASEDIR/..")

cmake -S . -B "$ROOTDIR/build" -DBUILD_TESTING=ON
cmake --build "$ROOTDIR/build"

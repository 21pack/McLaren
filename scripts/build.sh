#!/bin/sh -e

BASEDIR=$(realpath "$(dirname "$0")")
ROOTDIR=$(realpath "$BASEDIR/..")

RELEASE=false
WITH_TESTS=true
BUILD_DIR="$ROOTDIR/build"
CMAKE_BUILD_TYPE=""
CMAKE_OPTIONS=""

for arg in "$@"; do
    case $arg in
        --release)
            RELEASE=true
            ;;
        --without-tests)
            WITH_TESTS=false
            ;;
        *)
            echo "Unknown argument: $arg"
            ;;
    esac
done

if [ "$RELEASE" = true ]; then
    CMAKE_BUILD_TYPE="-DCMAKE_BUILD_TYPE=Release"
    CMAKE_OPTIONS="$CMAKE_OPTIONS -DBUILD_TESTING=OFF"
    echo "Release build"
else
    CMAKE_BUILD_TYPE="-DCMAKE_BUILD_TYPE=Debug"
    echo "Debug build"
    if [ "$WITH_TESTS" = true ]; then
        CMAKE_OPTIONS="$CMAKE_OPTIONS -DBUILD_TESTING=ON"
    else
        CMAKE_OPTIONS="$CMAKE_OPTIONS -DBUILD_TESTING=OFF"
    fi
fi

cmake -S "$ROOTDIR" -B "$BUILD_DIR" $CMAKE_BUILD_TYPE $CMAKE_OPTIONS
cmake --build "$BUILD_DIR" --config $( [ "$RELEASE" = true ] && echo "Release" || echo "Debug" )

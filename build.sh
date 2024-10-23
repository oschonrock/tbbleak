#!/bin/bash

set -o errexit
set -o nounset
USAGE="Usage: $(basename $0) [-v | --verbose] [ gcc | clang ] [ reset | clean | debug | release | relwithdebinfo]"

CMAKE=cmake
BUILD=./build
TYPE=relwithdebinfo
COMPILER=gcc
CLEAN=
RESET=
VERBOSE=

for arg; do
  case "$arg" in
    --help|-h)    echo $USAGE; exit 0;;
    -v|--verbose) VERBOSE='VERBOSE=1'  ;;
    debug)        TYPE=debug ;;
    release)      TYPE=release ;;
    relwithdebinfo)      TYPE=relwithdebinfo ;;
    clang)        COMPILER=clang ;;
    gcc)          COMPILER=gcc ;;
    clean)        CLEAN=1 ;;
    reset)        RESET=1 ;;
    *)            echo -e "unknown option $arg\n$USAGE" >&2;  exit 1 ;;
  esac
done

cd "$(realpath $(dirname $0))"

BUILD_DIR=$BUILD/$COMPILER/$TYPE

if [[ "$COMPILER" == "clang" ]]
then
    C_COMPILER=clang 
    CXX_COMPILER=clang++
else
    C_COMPILER=gcc 
    CXX_COMPILER=g++
fi
COMPILER_OPTIONS="-DCMAKE_C_COMPILER=$C_COMPILER -DCMAKE_CXX_COMPILER=$CXX_COMPILER"

[[ -n $RESET && -d $BUILD_DIR ]] && rm -rf $BUILD_DIR
    
$CMAKE -S . -B $BUILD_DIR $COMPILER_OPTIONS -DCMAKE_BUILD_TYPE=$TYPE

[[ -n $CLEAN ]] && $CMAKE --build $BUILD_DIR --target clean

$CMAKE --build $BUILD_DIR -- -j4 $VERBOSE

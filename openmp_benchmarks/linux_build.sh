#!/bin/bash
rm -rf build

if [[ "$1" = "icc" ]] ; then
    export CC=icc
    export CXX=icpc
elif [[ "$1" = "clang" ]] ; then
    export CC=clang
    export CXX=clang++
else 
    export CC=gcc
    export CXX=g++
fi

mkdir -p build
cd build
cmake -GNinja ..
ninja
cd ..

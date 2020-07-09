#!/bin/bash
mkdir -p build
cd build
cmake -GNinja ..
ninja
cd ..
ln -s build/OpenMPBench.exe OpenMpBench.exe

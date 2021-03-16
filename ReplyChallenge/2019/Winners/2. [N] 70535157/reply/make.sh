#!/bin/bash -E

mkdir -p build
cd build
cmake ..
make -j 2 $@

#!/bin/sh

mkdir build
cmake -S . -B build && cd build && make
echo 'executable successfully built in `build`'

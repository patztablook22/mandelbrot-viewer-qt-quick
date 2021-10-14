#!/bin/sh
rm build/mandelbrot
mkdir build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cd build && make
echo 'executable successfully built in `build`'
./mandelbrot

# mandelbrot-viewer-qt-quick
Mandelbrot set viewer in C++ QtQuick

## Compiling from source

```sh
git clone https://github.com/patztablook22/mandelbrot-viewer-qt-quick
cd mandelbrot-viewer-qt-quick

./configure.sh
./build.sh
```
will create executable `mandelbrot-viewer-qt-quick/build/src/mandelbrot`, to execute it, run:
```sh
./run.sh
```

## Controls

| Event  | Action                           |
|--------|----------------------------------|
|`MOUSE1`| center and zoom in 1.3x          |
|`MOUSE2`| center and zoom out 1.3x         |
|`MOUSE3`| center                           |
|`SCROLL`| zoom in/out a little bit         |
|`CTRL+E`| export currently rendered image  |
|`CTRL+P`| palette selection menu           |
|`CTRL+R`| rendering menu                   |
|`CTRL+Q`| quit                             |

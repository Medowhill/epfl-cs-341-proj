# CS-341 Project
## Rendering 3-D Fractal Images via Raytracing
### Instructions
* Install `make`, `cmake`, `g++`, and [opencv](https://docs.opencv.org/master/df/d65/tutorial_table_of_content_introduction.html).
```shell
$ mkdir build out
$ cd build
$ cmake ..
$ make
$ cd ..

$ build/raytracer -i examples/julia.json
$ build/raytracer -i examples/julia_video.json

# If you are using macOS:
$ open out/julia.png
$ open out/julia_video.avi
```
### Options
* `-a` : enable ambient occlusion
* `-d` : debug mode
* `-h` : print help message
* `-i [name]` : specify the input file path
* `-o [name]` : specify the output file path
* `-s` : enable shadows
* `-S` : enable soft shadows
### Documentation
* [Project GitHub Page](https://medowhill.github.io/epfl-cs-341-proj/)

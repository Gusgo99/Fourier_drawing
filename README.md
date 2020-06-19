# Fourier_drawing
A program that uses a complex Fourier series to draw things.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

To compile the project, the only external dependency is wxWidgets. It can be installed by following the [tutorial](https://wiki.wxwidgets.org/Install) for your specific platform.

### Installing

In order to compile it the first time, run the following commands on the project directory:

```
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
```

This will generate the environment necessary to compile the project. If the generator you're using uses a makefile, you can just run make and it will generate the executable.

It's also possible to build the program with optimizations:

```
mkdir release
cd release
cmake .. -DCMAKE_BUILD_TYPE=Release
```

It can be compiled the same way as the debug version

## Built With

* [Mingw-W64](http://mingw-w64.org/doku.php) - Compiler - version: 10.1.0
* [wxWidgets](https://www.wxwidgets.org/) - Graphical library - version: 3.1.3
* [KissFFT](https://github.com/mborgerding/kissfft) - FFT library

## Authors

* **Gustavo Pacola Gonçalves** - [Gusgo99](https://github.com/Gusgo99)

## License

This project is licensed under the MIT License - see the LICENSE file for details.

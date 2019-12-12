# Fourier_drawing
A program that uses the Fourier series to draw things

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

These instructions are supposed to be followed on windows. If you wish to use another OS, use the instructions from SDL2 and modify the makefile accordingly.

The only external dependency of this project is the SDL2 library. In order to get it to compile, download the [SDL2 developmento library](http://www.libsdl.org/download-2.0.php). The file downloaded should match your target compiler.

The downloaded compressed file has a INSTALL.txt file which should have instructions about which directory has the version for the system you're running. After selecting the correct directory, copy the lib and include directories to C:\libraries\. Other directories can also be used, but the makefile has to be altered.

The bin directory is also important, as it contains SDL2.dll, which should be copied to the project root directory.

### Installing

In order to compile it the first time, run the following command on the project directory:

```
make setup
```

In order to compile, just use:

```
make
```

And to execute:

```
make exec
```

It's also possible to build the program with optimizations:

```
make optim
```

And to execute the optimized version:

```
make exec_optim
```

## Built With

* [Mingw-W64](http://mingw-w64.org/doku.php) - Compiler used - version: (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0
* [SDL2](http://www.libsdl.org/download-2.0.php) - Graphical library used - version: 2.0.9

## Authors

* **Gustavo Pacola Gonçalves** - [Gusgo99](https://github.com/Gusgo99)

## License

This project is licensed under the GNU General Public License 3 - see the LICENSE file for details.

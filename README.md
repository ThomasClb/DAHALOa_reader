# DAHALOa_reader
The continuous CR3BP abacus reader based on Differential Algebra.

## Requirements
To use the DAHALOa_reader code, you need a C++11 compiler.

The code was mainly tested on Unix distributions but could be adapted with very low effort.

The only dependency on this code is with the DACE, which can be found at: https://github.com/ThomasClb/dace.
This DACE version was originally forked from the original DACE repository: https://github.com/dacelib/dace.
It uses AlgebraicMatrix<T> class without making modifications to the CMakeLists.txt file and solves an I/O bug of the DACE, required to read DA files.

## Setting up DAHALOa_reader
We recommend using CMake to use DAHALOa_reader and build the examples.
To use DAHALOa_reader library, simply clone this repository:
```
git clone https://github.com/ThomasClb/DAHALOa_reader.git
```
Then create a build directory and run cmake, and then make to compile the examples:
```
cd DAHALOa_reader
mkdir build
cd build
cmake ../script/example/
make
```
You might need to locate the DACE for cmake if it was not installed using:
```
sudo make install
```

## Running the examples
Two examples are given to understand how DAHALOa_reader works and how to use it, as well as a test file mapping the Lyapunov orbits at L2 in the Earth-Moon system.

You can download more solution files to read with DAHALOa_reader on https://doi.org/10.5281/zenodo.6778146.

In the DAHALOa_reader folder run:
```
./build/example <example_index: 1 or 2> <parameters> 
```
Leaving the parameters field empty will return an error message that will inform you on what are the available options.

Reading the source code of the two examples while reading their outputs is highly recommended to understand how they work and how to use DAHALOa_reader.

# Sweep-Decoder-Boundaries

C++ implementation of the sweep decoder for topological quantum codes with boundaries.
See https://arxiv.org/abs/1809.10145 and .

## Build instructions

Use [CMake](https://cmake.org/) to build. During the build process CMake will automatically download [googletest](https://github.com/google/googletest) and [pcg-cpp](https://github.com/imneme/pcg-cpp). Tested on Linux (Ubuntu 16.04 & 18.04) and macOS.

### Step-by-step instructions

- `mkdir build && cd build`
- `cmake -DCMAKE_BUILD_TYPE=Release ../`
- `make`

### To run the tests

- `mkdir build && cd build`
- `cmake -DCMAKE_BUILD_TYPE=Debug ../`
- `make`
- `make test`

## Usage

- The python script `data_generator.py` is the entry_point
- Run `python data_generator.py --help` for information
- See `example_script.py` for an example of a bigger run

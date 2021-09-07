# DQMCpp

Here you can find a C++ library to work with CERN CMS DQM and some tests used in CMS PFG group. Fill free to use these tests as examples to develop your essential things!

Read online docs -- [https://thegriglat.github.io/dqmcpp](https://thegriglat.github.io/dqmcpp/)

## How to build

DQMCpp only depends on `libcurl` and `libcurl-dev`. CMake is used as build system.
```bash
mkdir build
cd build
cmake ../src
make -j `nproc`
```

To get Doxygen docs about DQMCpp run `make docs` or `make docs-pdf` from root of the repository. I try to get docs up-to-date.
```bash
make docs
make docs-pdf
```

## How to contribute:
 * Make changes
 * run `make` in source -- it will run `clang-format` to make code looks better and unified
 * Submit merge/pull request or contact via thegriglat@gmail.com
 


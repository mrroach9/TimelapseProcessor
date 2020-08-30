This folder contains a precompiled library of
[googletest](https://github.com/google/googletest) v1.10.0, including headers
and static libs. Currently only supporting Windows X86_64.

## Prerequisites
* CMake
* For Windows:
  * Visual Studio Build Tools, VC16 (VS 2019) or above
  * PowerShell
* For linux:
  * TODO

## Build command
First run the following commands:

    git clone https://github.com/google/googletest.git
    mkdir build && cd build
    mkdir out
    cmake ../ -DCMAKE_INSTALL_PREFIX="./out"

Then if you're building on...
### Windows
* Open build/googletest-distribution.sln in Visual Studio, and build project "INSTALL" with 
"Release" mode.
* Once build succeeds, simply copy over everything in build/out folder to your code base.

### Linux
TODO

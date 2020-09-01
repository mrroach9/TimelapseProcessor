This folder contains a precompiled library of
[rapidjson](https://rapidjson.org/) v1.1.0. It's a header-only lib and supports
all platforms.

## Prerequisites
* CMake
* For Windows:
  * Visual Studio Build Tools, VC16 (VS 2019) or above
  * PowerShell
* For linux:
  * TODO

## Build command
First run the following commands:

    git clone https://github.com/Tencent/rapidjson.git
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

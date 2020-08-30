This folder contains a precompiled library of
[opencv](https://github.com/opencv/opencv) v4.4.0, including headers
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
    cmake ../ -DBUILD_WITH_STATIC_CRT=OFF -DBUILD_SHARED_LIBS=OFF

Then if you're building on...
### Windows
* Open build/OpenCV.sln in Visual Studio, and build project "INSTALL" with 
"Release" mode.
* Once build succeeds, copy over everything in build/install folder to your code base.
You can choose to skip the executables at build\install\x64\vc16\bin and feature
detection data at build/install/etc folder.

### Linux
TODO

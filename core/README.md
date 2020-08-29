# Timelapse Processor Core Library

[![Build Status](https://travis-ci.org/mrroach9/TimelapseProcessor.svg?branch=master)](https://travis-ci.org/mrroach9/TimelapseProcessor)

This folder contains the C++ backend of the timelapse preprocessor, with all
the core algorithm on rendering, image processing, video exporting. This library
(shall) interact with frontend app via NAPI.

Currently we have only tested and supported building under Windows x86_64.

## Prerequisites
* [Bazel v3.3.1](https://www.bazel.build/) or above
* For Windows:
  * Visual Studio Build Tools, VC16 (VS 2019) or above
  * PowerShell
* For linux:
  * TODO

## Build command
For now this project is still a sketch, you can verify the environment and code
completeness by running:

    bazel test ...


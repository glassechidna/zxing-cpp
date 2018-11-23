# ZXing C++ Port


# Scantrust usage

## Build for ios
You have to first set two environment variables

1) `IOS_LIBS_OUTPUT` the destination of the compile library
2) `IOS_TOOLCHAIN` the toolchain that cmake should use to compile the project for ios platform. The toolchain can be found here (https://github.com/leetal/ios-cmake).

Then run the script
```
./build-ios.sh
```

It will build and install zxing library in `$IOS_LIBS_OUTPUT`. The library is build for every iOS
platform (e.g. armv7, arm64, ...) and the binaries are then packed together inside the same static
archive (libzxing.a).

## Android

For android, you should use this cmake project as a native dependency from your main project.

> Note: You might want to check this (https://github.com/ScanTrust/android-libs/blob/feature/use-jni-sdk/android-sdk/build.gradle)

**TODO** Explain the android build process!! 

## Desktop

To compile the library on your machine just run the following commands

```
# cd to project root
mkdir build # if not existing
cd build
cmake ..
cmake install
```

[![Build Status](https://travis-ci.org/glassechidna/zxing-cpp.svg?branch=master)](https://travis-ci.org/glassechidna/zxing-cpp)

[ZXing](https://github.com/zxing/zxing) is/was a Java library.

At some point a complete C++ port/rewrite was created and maintained in the official [ZXing](https://github.com/zxing/zxing) repo. However, at the time of writing the C++ port is no longer maintained and has been removed from the official ZXing repo.

This project was forked from the [last ZXing commit](https://github.com/zxing/zxing/commit/00f6340) to contain the C++ project, with the following exceptions

 * scons (Python) build system has been deleted.
 * Deleted black box tests, because they refer to a large test data in ZXing repo.
 * Added appropriate copyright/licensing details (based on those in the ZXing repo).
 * Updated README.md

Removal of build systems was done to minimise maintenance burden.

If tests and XCode projects (other than those produced automatically be CMake) are desired, then another repo should be created and this repo referenced as a submodule. 

# Building using CMake

CMake is a tool, that generates native makefiles and workspaces. It integrates well with a number of IDEs including Qt Creator and Visual Studio.

Usage with CLion or Qt Creator:

  1. Simply open `CMakeLists.txt` as a new project
  2. Additional command line arguments can be specified (see below)

Usage with Makefiles, Visual Studio, etc. (see `cmake --help` for a complete list of generators):

  1. `mkdir build`
  2. `cd` to `build`
  3. Unix: run `cmake -G "Unix Makefiles" ..`
  3. Windows: run `cmake -G "Visual Studio 10" ..`
  
You can switch between build modes by specifying:

  - `-DCMAKE_BUILD_TYPE=Debug` or
  - `-DCMAKE_BUILD_TYPE=Release`

# OpenCV integration

When build on a system where opencv is installed the open cv bridge classes and executable will be built too.

# Development tips

To profile the code (very useful to optimize the code):

  1. Install Valgrind
  2. Run `valgrind --tool=callgrind build/zxing - path/to/test/data/*.jpg > report.html`
  3. Analyze output using KCachegrind

#! /bin/sh

continue = 1

if [[ -z "$IOS_LIBS_OUTPUT" ]]; then
    echo "Please set IOS_LIBS_OUTPUT to use this script."
    echo "   IOS_LIBS_OUTPUT: the path where the ios libraries are saved.";
    continue = 0
fi

if [[ -z "$IOS_TOOLCHAIN" ]]; then
    echo "Please set IOS_LIBS_OUTPUT to use this script."
    echo "   IOS_TOOLCHAIN: the path to the cmake iod toolchain.";
    echo "                  you can get it at https://github.com/leetal/ios-cmake"
    continue = 0
fi

if [[ $continue -ne 1 ]]; then
    echo "Configuration issue."
    exit 1
fi

if [[ -d build ]]; then
    echo "Cleaning build dir"
    rm -r build
fi
mkdir build
cd build

if [[ ${IOS_LIBS_OUTPUT} != /* ]]; then
    IOS_LIBS_OUTPUT=../${IOS_LIBS_OUTPUT}
    echo ${IOS_LIBS_OUTPUT}
fi

cmake .. -DCMAKE_TOOLCHAIN_FILE=${IOS_TOOLCHAIN} -DCMAKE_INSTALL_PREFIX=${IOS_LIBS_OUTPUT} -DCMAKE_PREFIX_PATH=${IOS_LIBS_OUTPUT} -DCMAKE_FIND_NO_INSTALL_PREFIX=True -DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=ONLY --debug-output
make
make install

cmake_minimum_required(VERSION 3.0)

project(zxing)

option(BUILD_TESTING "Enable generation of test targets" OFF)
option(BUILD_RUNNABLE "Enable generation of runnable targets" OFF)

set(CMAKE_LIBRARY_PATH /opt/local/lib ${CMAKE_LIBRARY_PATH})

# Check for polluted source tree.
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/CMakeCache.txt OR
    EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/CMakeFiles)
    message(FATAL_ERROR
        "Source directory is polluted:"
        "\n  * remove CMakeCache.txt"
        "\n  * remove CMakeFiles directory")
endif()

# Suppress in-source builds.
if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
    message(FATAL_ERROR
        "CMake generation is not allowed within the source directory:"
        "\n  * mkdir build"
        "\n  * cd build"
        "\n  * Unix-like: cmake -G \"Unix Makefiles\" .."
        "\n  * Windows: cmake -G \"Visual Studio 10\" ..")
endif()

# Adjust CMake's module path.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/FindModules)

# Suppress MSVC CRT warnings.
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(/Za)
    add_definitions(/MP) # multi-core builds
endif()

include(source_files.cmake)

if(WIN32)
    include_directories(core/lib/win32)
    set(CMAKE_DEBUG_POSTFIX -debug)
endif()

# OpenCV classes
find_package(OpenCV)
if(OpenCV_FOUND)
    list(APPEND LIBZXING_FILES
        opencv/src/zxing/MatSource.cpp
        opencv/src/zxing/MatSource.h
    )

    include_directories(${OpenCV_INCLUDE_DIRS})
    include_directories(opencv/src)
endif()


include_directories(core/src)
add_library(libzxing ${LIBZXING_FILES})
set_target_properties(libzxing PROPERTIES OUTPUT_NAME zxing)

set(targets libzxing)


if(WITHOUT_ICONV)
    message(STATUS "Skip iconv detection")
else()
    find_package(Iconv)
    if(ICONV_FOUND)
        include_directories(${ICONV_INCLUDE_DIR})
        target_link_libraries(libzxing ${ICONV_LIBRARIES})
    else()
        add_definitions(-DNO_ICONV=1)
    endif()
endif()

if(NOT ANDROID)

    if(BUILD_RUNNABLE)
        # Add cli executable.
        file(
            GLOB_RECURSE ZXING_FILES
            "./cli/src/*.cpp"
            "./cli/src/*.h"
        )

        add_executable(zxing ${ZXING_FILES})
        target_link_libraries(zxing libzxing)
        set(targets ${targets} zxing)

        # Add OpenCV cli executable
        if(OpenCV_FOUND)
            file(GLOB_RECURSE OPENCV_ZXING_FILES
                    "./opencv-cli/src/*.cpp"
                    "./opencv-cli/src/*.h"
                    )
            add_executable(zxing-cv ${OPENCV_ZXING_FILES})
            target_link_libraries(zxing-cv libzxing ${OpenCV_LIBRARIES})
        endif()
    endif()

    if(BUILD_TESTING)
        # Add testrunner executable.
        find_package(CPPUNIT)
        if(CPPUNIT_FOUND)
            file(GLOB_RECURSE TESTRUNNER_FILES
                "./core/tests/src/*.cpp"
                "./core/tests/src/*.h"
            )
            add_executable(testrunner ${TESTRUNNER_FILES})
            include_directories(${CPPUNIT_INCLUDE_DIR})
            target_link_libraries(testrunner libzxing  ${CPPUNIT_LIBRARIES})
        endif()
    endif()
endif()

install(TARGETS ${targets} EXPORT zxing-targets
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        INCLUDES DESTINATION include
        )

install(EXPORT zxing-targets DESTINATION lib/zxing/cmake NAMESPACE zxing::)

install(
        DIRECTORY core/src/zxing/
        DESTINATION include/zxing
        FILES_MATCHING PATTERN "*.h"
)

configure_file(cmake/zxing-config.cmake.in zxing-config.cmake @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/zxing-config.cmake DESTINATION lib/zxing/cmake)
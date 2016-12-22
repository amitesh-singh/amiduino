#=====================================================================================
# ubirch #1 cmake
#=====================================================================================
cmake_minimum_required(VERSION 3.0)

#this is the place where you can set configuration options
set(CMAKE_TOOLCHAIN_FILE cmake/avr-arduino-toolchain.cmake)
include(config.cmake)

# the toolchain file (see https://github.com/ubirch/avr-arduino-cmake)

# project name and code types we use
project(shresth C CXX ASM)

# adding our libraries, sources
add_libraries(lib)
add_sources(src)

# add the sketches directory (contains sketch directories and
# a "libraries" dir where dependent libs are extracted
add_sketches(sketches)

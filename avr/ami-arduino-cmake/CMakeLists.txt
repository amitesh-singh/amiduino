cmake_minimum_required(VERSION 3.0)

#this is the place where you can set configuration options
include(config.cmake)

# the toolchain file (see https://github.com/ubirch/avr-arduino-cmake)
set(CMAKE_TOOLCHAIN_FILE cmake/avr-arduino-toolchain.cmake)

# project name and code types we use
project(ami-arduino C CXX ASM)

# adding our libraries, sources
add_libraries(lib)
# we don't need it now - ami
#add_sources(src)

# add the sketches directory (contains sketch directories and
# a "libraries" dir where dependent libs are extracted
add_sketches(sketches)

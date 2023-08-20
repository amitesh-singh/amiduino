# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(blinky)
set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS “-fmodules-ts”)
add_compile_options(-std=c++20) #-fmodules-ts -fext-numeric-literals)



target_sources(app PRIVATE src/main.cpp)

#!/bin/bash -x

# this files helps bundling the build commands

mkdir -p build && cd build && cmake .. && make

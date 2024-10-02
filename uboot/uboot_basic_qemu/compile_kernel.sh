#!/bin/bash

clang -fPIE -target aarch64-unknown-none -c start.S -o start.o
lld -flavor ld -maarch64elf --pie --static --nostdlib --script=kernel.lds start.o -o kernel.elf

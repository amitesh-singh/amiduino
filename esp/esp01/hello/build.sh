#!/bin/bash
#
arduino-cli compile -v --fqbn esp8266:esp8266:generic:eesz=1M64 --build-property "compiler.optimization_flags=-Os"


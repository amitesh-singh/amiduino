#!/bin/bash
#
# NodeMCU v3 (LoLin, ESP-12E) is "NodeMCU 1.0 (ESP-12E Module)" in the esp8266 core
arduino-cli compile -v --fqbn esp8266:esp8266:nodemcuv2 \
  --build-property "compiler.optimization_flags=-Os" \
  "$(dirname "$0")"


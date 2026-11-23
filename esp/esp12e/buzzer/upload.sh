#!/bin/bash

arduino-cli upload \
  -p /dev/ttyUSB0 \
  --fqbn esp8266:esp8266:nodemcuv2 \
  .



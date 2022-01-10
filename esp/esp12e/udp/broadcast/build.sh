#arduino-builder -compile -warnings=default -verbose -logger=humantags -build-options-file=$(pwd)/build.options.json -build-path=$(pwd)/build -build-cache=$(pwd)/.cache $(pwd)/udpclient.ino
arduino-cli compile --fqbn esp8266:esp8266:generic $(pwd)/


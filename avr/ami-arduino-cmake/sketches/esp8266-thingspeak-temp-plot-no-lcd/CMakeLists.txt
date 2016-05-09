# add a target and tell it what it's source files are
add_executable_avr(thingspeak_temp thingspeak.cpp)
# tell the target, that it should use the arduino core libraries
target_link_libraries(thingspeak_temp arduino-core)


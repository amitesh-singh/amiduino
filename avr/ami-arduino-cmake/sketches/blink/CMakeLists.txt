# add a target and tell it what it's source files are
add_executable_avr(blink blink.cpp)
# tell the target, that it should use the arduino core libraries
target_link_libraries(blink arduino-core)

# add a target and tell it what it's source files are
add_executable_avr(irsend irsendsamsung.cpp)
# tell the target, that it should use the arduino core libraries
target_link_libraries(irsend arduino-core)
target_sketch_libraryoffline(irsend Arduino-IRremote)
target_sketch_libraryoffline(irsend samsungtvremote)
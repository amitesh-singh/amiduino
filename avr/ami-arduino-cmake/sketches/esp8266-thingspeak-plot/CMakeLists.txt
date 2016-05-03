# add a target and tell it what it's source files are
add_executable_avr(thingspeak thingspeak.cpp)
# tell the target, that it should use the arduino core libraries
target_include_directories(thingspeak PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/LiquidCrystal/
                           ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/SoftwareSerial/)
target_link_libraries(thingspeak -L${CMAKE_CURRENT_SOURCE_DIR}/../../lib/LiquidCrystal/ -lliquidcrystal arduino-core)


# add a target and tell it what it's source files are
add_executable_avr(wind_speed wind_speed.cpp)
# tell the target, that it should use the arduino core libraries
target_include_directories(wind_speed PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/LiquidCrystal/)
target_link_libraries(wind_speed -L${CMAKE_CURRENT_SOURCE_DIR}/../../lib/LiquidCrystal/ -lliquidcrystal arduino-core)


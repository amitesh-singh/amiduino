# add a target and tell it what it's source files are
add_executable_avr(temp temp.cpp)
# tell the target, that it should use the arduino core libraries
target_include_directories(temp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/LiquidCrystal/)
target_link_libraries(temp -L${CMAKE_CURRENT_SOURCE_DIR}/../../lib/LiquidCrystal/ -lliquidcrystal arduino-core)


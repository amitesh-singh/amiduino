add_executable_avr(kitchen_timer kitchen-timer.cpp)
message(STATUS ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/7segLed/)
target_include_directories(kitchen_timer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/7segLed/)
target_link_libraries(kitchen_timer -L${CMAKE_CURRENT_SOURCE_DIR}/../../lib/7segLed/ -lsegdisplay arduino-core)

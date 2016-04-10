# add a target and tell it what it's source files are
#find_package(SEGDISPLAY REQUIRED)
add_executable_avr(display display.cpp)
#target_include_directories(display PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${ARDUINO_CORES_PATH} ${ARDUINO_VARIANTS_PATH} ${ARDUINO_VARIANTS_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/lib/7segLed )
# tell the target, that it should use the arduino core libraries
#target_link_libraries(display )
#target_link_libraries(display segdisplay)
message(STATUS ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/7segLed/)
target_include_directories(display PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/7segLed/)
target_link_libraries(display -L${CMAKE_CURRENT_SOURCE_DIR}/../../lib/7segLed/ -lsegdisplay arduino-core)

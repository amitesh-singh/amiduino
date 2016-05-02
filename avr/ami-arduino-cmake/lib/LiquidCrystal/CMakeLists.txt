add_library(liquidcrystal  LiquidCrystal.h LiquidCrystal.cpp)
#target_include_directories(segdisplay PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${ARDUINO_CORES_PATH} ${ARDUINO_VARIANTS_PATH} ${ARDUINO_VARIANTS_PATH})
target_link_libraries(liquidcrystal arduino-core)


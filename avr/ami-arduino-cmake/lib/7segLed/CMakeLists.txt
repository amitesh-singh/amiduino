add_library(segdisplay  Seg7Led.h Seg7Led.cpp)
target_include_directories(segdisplay PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
${ARDUINO_CORES_PATH} ${ARDUINO_VARIANTS_PATH} ${ARDUINO_VARIANTS_PATH})
target_link_libraries(segdisplay -larduino-core)


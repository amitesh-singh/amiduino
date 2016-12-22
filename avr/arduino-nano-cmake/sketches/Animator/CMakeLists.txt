# add a target and tell it what it's source files are
add_executable_avr(anim animator.cpp main.cpp)
# tell the target, that it should use the arduino core libraries
target_link_libraries(anim arduino-core)
target_link_libraries(anim SPI)
target_link_libraries(anim UTFT)
target_link_libraries(anim URTouch)

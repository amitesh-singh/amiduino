cmake_minimum_required(VERSION 3.12)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(my_project)

# initialize the Pico SDK
pico_sdk_init()

# rest of your project

add_executable(hello_world main.c)
target_link_libraries(hello_world pico_stdlib)
pico_add_extra_outputs(hello_world)

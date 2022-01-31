project(basic)

add_executable(logging_server async_udp_server.cpp)
add_executable(client client.cpp)

target_link_libraries(logging_server PRIVATE pthread)
target_link_libraries(client PRIVATE pthread)

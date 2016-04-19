include_directories(${CMAKE_SOURCE_DIR}/inc)
add_executable(server address.cpp client.cpp list_processor.cpp main.cpp packet.cpp socket.cpp server_socket.cpp log_manager.cpp u16basic.cpp)

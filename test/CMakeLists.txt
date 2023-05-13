cmake_minimum_required(VERSION 3.22)

project(Test)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(client client.cpp)

target_include_directories(client
    PRIVATE
        ${CMAKE_SOURCE_DIR}/network_communication
        ${CMAKE_SOURCE_DIR}/vendor/asio/asio/include
)

target_link_libraries(client
    PRIVATE
        ${CMAKE_BINARY_DIR}/network_communication/libnetwork_communication.a
    PUBLIC
        nlohmann_json::nlohmann_json
)


add_executable(server server.cpp)

target_include_directories(server
    PRIVATE
        ${CMAKE_SOURCE_DIR}/network_communication
        ${CMAKE_SOURCE_DIR}/vendor/asio/asio/include
)

target_link_libraries(server
    PRIVATE
        ${CMAKE_BINARY_DIR}/network_communication/libnetwork_communication.a
    PUBLIC
        nlohmann_json::nlohmann_json
)
cmake_minimum_required(VERSION 3.22)

project(network_communication
    DESCRIPTION "network communication"
    LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FILE(GLOB SOURCE_FILES *.cpp)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_SOURCE_DIR}/network_communication/network_communication
        ${CMAKE_SOURCE_DIR}/vendor/asio/asio/include
        ${CMAKE_SOURCE_DIR}/vendor/json/include
)

target_link_libraries(${PROJECT_NAME} PUBLIC nlohmann_json::nlohmann_json)
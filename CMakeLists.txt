cmake_minimum_required(VERSION 3.22)

project(network_communication
    DESCRIPTION "network communication"
    LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(vendor/json)
add_subdirectory(network_communication)
add_subdirectory(test) # for testing

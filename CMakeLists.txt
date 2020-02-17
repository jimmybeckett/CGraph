cmake_minimum_required(VERSION 3.10)

project(CGraph)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(BUILD_SHARED_LIBS OFF)

set(CMAKE_C_STANDARD 11)

set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(src)

enable_testing()

add_subdirectory(test)

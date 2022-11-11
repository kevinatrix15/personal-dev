cmake_minimum_required(VERSION 3.10)

project(SaveBB8)

set(BUILD_DOC
    NO
    CACHE BOOL "Build documentation")

enable_testing()

# Set the CMAKE source directory
set(CONFIG_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/CMake
    CACHE FILEPATH "Directory containing project configuration settings")

add_subdirectory(src)

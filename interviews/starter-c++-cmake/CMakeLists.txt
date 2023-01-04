cmake_minimum_required(VERSION 3.10)

project(CodingQuiz)

set(BUILD_DOC
    NO
    CACHE BOOL "Build documentation")

if(DEFINED UNITTEST AND NOT UNITTEST EQUAL 0)
  enable_testing()
endif()

# Set the CMAKE source directory
set(CONFIG_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/CMake
    CACHE FILEPATH "Directory containing project configuration settings")

add_subdirectory(src)
add_subdirectory(googletest)
cmake_minimum_required(VERSION 3.0.0)
project(hiring-quiz VERSION 0.1.0)

include(CTest)
enable_testing()

add_subdirectory(googletest)

include_directories(googletest/include)

add_executable(hiring-quiz main.cpp)

target_link_libraries(hiring-quiz PRIVATE gtest)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# for gtest setup: https://stackoverflow.com/questions/62910867/how-to-run-tests-and-debug-google-test-project-in-vs-code

# templates: https://mjmorse.com/blog/cmake-template/

set(this_target starter.Test)

# Select all c++ files
file(GLOB CPP "*.cpp")
# Select all header files
file(GLOB H "*.h")

# Set binary for tests creation
add_executable(${this_target} ${CPP} ${H})
target_include_directories(
  ${this_target}
  PRIVATE
    "${CMAKE_SOURCE_DIR}/googletest/googletest/include"
    "${CMAKE_SOURCE_DIR}/googletest/googletest/include/gtest"
    "${CMAKE_SOURCE_DIR}/googletest/googlemock/include"
    "${CMAKE_SOURCE_DIR}/googletest/googlemock/include/gmock")

source_group("Header
    Files" FILES ${H})
source_group("Source
    Files" FILES ${CPP})

set(LIBS
    starter
    gtest
    gtest_main
    gmock)

# Set libraries needed by the test binary
target_link_libraries(${this_target} ${LIBS})

# Set projects this test depends on
add_dependencies(${this_target} starter gtest gtest_main gmock
                 gmock_main)

# Set the test binary to run
add_test(NAME ${this_target} COMMAND ${this_target})

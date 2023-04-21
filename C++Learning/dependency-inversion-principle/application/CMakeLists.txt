set(TARGET_NAME my-lib)

################################################################################
# Add Libraries
#-------------------------------------------------------------------------------
add_library(${TARGET_NAME}
  ${SHARED_OR_STATIC}
)

# TODO: set this for entire project settings
if(MSVC)
  target_compile_options(${TARGET_NAME} PRIVATE /W3 /WX)
else()
  target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

target_include_directories(${TARGET_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_compile_features(${TARGET_NAME} INTERFACE cxx_std_17)

################################################################################
# Install Rules
################################################################################
if(NOT CMAKE_SKIP_INSTALL_RULES)
    include(GNUInstallDirs)
    install(TARGETS ${TARGET_NAME})
    install(DIRECTORY include TYPE INCLUDE)
endif()


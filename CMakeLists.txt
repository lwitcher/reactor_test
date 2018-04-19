cmake_minimum_required (VERSION 2.6)
add_definitions(-std=c++11 -pthread)
include_directories(./)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/)

aux_source_directory(./ SRC_FILES)

add_executable(test ${SRC_FILES})

TARGET_LINK_LIBRARIES(
test
pthread
)
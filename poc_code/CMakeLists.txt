cmake_minimum_required (VERSION 2.6)
add_definitions(-O0 -g3 -std=c++11 -pthread)
include_directories(./)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/)

set(TEST_READ test_read.cpp)
set(TEST_EPOLL test_epoll.cpp)

add_executable(test_epoll ${TEST_EPOLL})
add_executable(test_read ${TEST_READ})

TARGET_LINK_LIBRARIES(
test_read
pthread
)
TARGET_LINK_LIBRARIES(
test_epoll
pthread
)

cmake_minimum_required(VERSION 2.8)
project(requirements)

add_definitions("--std=c++11")

include_directories("${CMAKE_CURRENT_LIST_DIR}/include")

FILE(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")

FIND_LIBRARY(LIB_BOOST_FILESYSTEM "boost_filesystem")
FIND_LIBRARY(LIB_BOOST_SYSTEM "boost_system")

add_executable(main ${SOURCES})

target_link_libraries(main ${LIB_BOOST_FILESYSTEM} ${LIB_BOOST_SYSTEM})
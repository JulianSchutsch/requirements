cmake_minimum_required(VERSION 2.8)
project(requirements)

add_definitions("--std=c++14 -Wextra -pedantic -Wall -O3 -g")

include_directories("${CMAKE_CURRENT_LIST_DIR}/include")

FILE(GLOB_RECURSE REQUIREMENTS_SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/requirements/*.cpp")
FILE(GLOB_RECURSE UTIL_SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/util/*.cpp")
FILE(GLOB_RECURSE REQ_SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/req/*.cpp")

FIND_LIBRARY(LIB_BOOST_FILESYSTEM "boost_filesystem")
FIND_LIBRARY(LIB_BOOST_SYSTEM "boost_system")

add_library(requirements SHARED ${REQUIREMENTS_SOURCES} ${UTIL_SOURCES})

target_link_libraries(requirements ${LIB_BOOST_FILESYSTEM} ${LIB_BOOST_SYSTEM})

add_executable(main "${CMAKE_CURRENT_LIST_DIR}/src/main.cpp" ${REQ_SOURCES})

target_link_libraries(main "requirements")

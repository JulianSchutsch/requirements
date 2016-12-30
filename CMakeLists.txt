cmake_minimum_required(VERSION 2.8)
project(requirements C CXX)

find_package(Threads REQUIRED)

add_definitions("--std=c++14 -Wextra -pedantic -Wall -O3 -g -DBOOST_NO_CXX11_SCOPED_ENUMS")

#The gtkmm stuff
find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)
link_directories(${GTKMM_LIBRARY_DIRS})

include_directories("${CMAKE_CURRENT_LIST_DIR}/include" ${GTKMM_INCLUDE_DIRS})
include_directories("${CMAKE_CURRENT_LIST_DIR}/test/include")

FILE(GLOB_RECURSE REQUIREMENTS_SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/requirements/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/src/generators/*.cpp")
FILE(GLOB_RECURSE UTIL_SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/util/*.cpp")
FILE(GLOB_RECURSE REQ_SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/req/*.cpp")
FILE(GLOB_RECURSE CONSOLE_SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/console/*.cpp")
FILE(GLOB_RECURSE GREQ_SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/greq/*.cpp")

FILE(GLOB_RECURSE REQUIREMENTS_TEST "${CMAKE_CURRENT_LIST_DIR}/test/requirements/*.cpp")
FILE(GLOB_RECURSE TEST_TEST "${CMAKE_CURRENT_LIST_DIR}/test/test/*.cpp")

find_library(LIB_BOOST_FILESYSTEM "boost_filesystem")
find_library(LIB_BOOST_SYSTEM "boost_system")
find_library(LIB_TINYXML "tinyxml")

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_library(requirements SHARED ${REQUIREMENTS_SOURCES} ${UTIL_SOURCES})

target_link_libraries(requirements ${LIB_BOOST_FILESYSTEM} ${LIB_BOOST_SYSTEM})

add_executable(req ${REQ_SOURCES} ${CONSOLE_SOURCES})

target_link_libraries(req "requirements" ${LIB_TINYXML})

add_executable(greq ${GREQ_SOURCES})

target_link_libraries(greq "requirements" ${LIB_TINYXML} ${GTKMM_LIBRARIES})

add_executable(t ${REQUIREMENTS_TEST} ${TEST_TEST})

target_link_libraries(t "requirements" ${CMAKE_THREAD_LIBS_INIT} ${GTEST_BOTH_LIBRARIES})

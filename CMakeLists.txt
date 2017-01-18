cmake_minimum_required(VERSION 2.8)
project(requirements CXX C)

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE DEBUG)
  message("No build type set, set to default")
endif()
message("Build type now ${CMAKE_BUILD_TYPE}")

set(COMMON_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++14 -Wextra -pedantic -Wall -DBOOST_NO_CXX11_SCOPED_ENUMS")
set(COMMON_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")

set(CMAKE_CXX_FLAGS_DEBUG "${COMMON_CXX_FLAGS} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${COMMON_CXX_FLAGS} -O3")
set(CMAKE_CXX_FLAGS_GCOV "${COMMON_CXX_FLAGS} -g -O0 --coverage -fprofile-arcs -ftest-coverage")

set(CMAKE_EXE_LINKER_FLAGS_GCOV "${COMMON_EXE_LINKER_FLAGS} -lgcov")

find_package(Threads REQUIRED)

function (add_sources varName baseFolder)
    foreach(folder IN LISTS ARGN)
        file(GLOB_RECURSE SOURCES "${baseFolder}/${folder}/*.cpp")
        list(APPEND RESULT "${SOURCES}")
    endforeach()
    set(${varName} ${RESULT} PARENT_SCOPE)
endfunction()

#The gtkmm stuff
find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)
link_directories(${GTKMM_LIBRARY_DIRS})

include_directories("${CMAKE_CURRENT_LIST_DIR}/include" ${GTKMM_INCLUDE_DIRS})
include_directories("${CMAKE_CURRENT_LIST_DIR}/test/include")

add_sources(REQUIREMENTS_SOURCES "${CMAKE_CURRENT_LIST_DIR}/src" "requirements" "generators" "annotations" "util" "commands")
add_sources(REQ_SOURCES "${CMAKE_CURRENT_LIST_DIR}/src" "req" "console")
add_sources(GREQ_SOURCES "${CMAKE_CURRENT_LIST_DIR}/src" "greq")
add_sources(TEST_SOURCES "${CMAKE_CURRENT_LIST_DIR}/test" "requirements" "util" "test" "annotations" "commands")

find_library(LIB_BOOST_FILESYSTEM "boost_filesystem")
find_library(LIB_BOOST_SYSTEM "boost_system")
find_library(LIB_TINYXML "tinyxml")

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_library(requirements SHARED ${REQUIREMENTS_SOURCES})
target_link_libraries(requirements ${LIB_BOOST_FILESYSTEM} ${LIB_BOOST_SYSTEM})

add_executable(req ${REQ_SOURCES} ${CONSOLE_SOURCES})
target_link_libraries(req "requirements" ${LIB_TINYXML})

add_executable(greq ${GREQ_SOURCES})
target_link_libraries(greq "requirements" ${LIB_TINYXML} ${GTKMM_LIBRARIES})

add_executable(t ${TEST_SOURCES})
target_link_libraries(t "requirements" ${LIB_TINYXML} ${CMAKE_THREAD_LIBS_INIT} ${GTEST_BOTH_LIBRARIES})

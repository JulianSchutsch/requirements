cmake_minimum_required(VERSION 2.8)
project(requirements CXX C)

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE DEBUG)
  message("No build type set, set to default")
endif()
message("Build type now ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD 14)
set(COMMON_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++14 -Wextra -pedantic -Wall -Weffc++ -DBOOST_NO_CXX11_SCOPED_ENUMS")
set(COMMON_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")

set(CMAKE_CXX_FLAGS_DEBUG "${COMMON_CXX_FLAGS} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${COMMON_CXX_FLAGS} -O3")
set(CMAKE_CXX_FLAGS_GCOV "${COMMON_CXX_FLAGS} -g -O0 --coverage -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS_GCOV "${COMMON_EXE_LINKER_FLAGS} -lgcov")

find_package(Threads REQUIRED)

function (add_sources varName filePattern baseFolder)
    foreach(folder IN LISTS ARGN)
        file(GLOB_RECURSE SOURCES "${baseFolder}/${folder}/${filePattern}")
        list(APPEND RESULT "${SOURCES}")
    endforeach()
    set(${varName} ${RESULT} PARENT_SCOPE)
endfunction()

include_directories("${CMAKE_CURRENT_LIST_DIR}/include" ${GTKMM_INCLUDE_DIRS})
include_directories("${CMAKE_CURRENT_LIST_DIR}/test/include")

find_library(LIB_BOOST_FILESYSTEM "boost_filesystem")
find_library(LIB_BOOST_SYSTEM "boost_system")
find_library(LIB_TINYXML "tinyxml")

add_sources(REQUIREMENTS_SOURCES "*.cpp" "${CMAKE_CURRENT_LIST_DIR}/src" "requirements" "util")

add_library(requirements SHARED ${REQUIREMENTS_SOURCES})
target_link_libraries(requirements ${LIB_BOOST_FILESYSTEM} ${LIB_BOOST_SYSTEM})

if("${DISABLE_REQ}" STREQUAL "")
  message("Configuring REQ")
  add_sources(REQ_SOURCES "*.cpp" "${CMAKE_CURRENT_LIST_DIR}/src" "req" "console")
  add_executable(req ${REQ_SOURCES} ${CONSOLE_SOURCES})
  target_link_libraries(req "requirements" ${LIB_TINYXML})
endif()

if("${DISABLE_QREQ}" STREQUAL "")
  message("Configuring QREQ")
  find_package(Qt5Widgets REQUIRED)
  find_package(Qt5Gui REQUIRED)
  set(CMAKE_AUTOMOC ON)
  set(CMAKE_INCLUDE_CURRENT_DIR ON)
  add_sources(QREQ_SOURCES "*.cpp" "${CMAKE_CURRENT_LIST_DIR}/src" "qreq")
  add_sources(QREQ_HEADERS "*.hpp" "${CMAKE_CURRENT_LIST_DIR}/include" "qreq")
  add_executable(qreq ${QREQ_SOURCES} ${QREQ_HEADERS})
  target_link_libraries (qreq "requirements" Qt5::Widgets Qt5::Gui ${LIB_TINYXML})
endif()

if("${DISABLE_GTEST}" STREQUAL "")
  message("Configuring GTEST")
  find_package(GTest REQUIRED)
  include_directories(${GTEST_INCLUDE_DIRS})
  add_sources(TEST_SOURCES "*.cpp" "${CMAKE_CURRENT_LIST_DIR}/test" "requirements" "util" "test" "annotations" "commands")
  add_executable(t ${TEST_SOURCES})
  target_link_libraries(t "requirements" ${LIB_TINYXML} ${CMAKE_THREAD_LIBS_INIT} ${GTEST_BOTH_LIBRARIES})
endif()

install(TARGETS req qreq requirements RUNTIME DESTINATION bin LIBRARY DESTINATION lib)
install(DIRECTORY include/requirements DESTINATION include/requirements)

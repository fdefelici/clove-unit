cmake_minimum_required(VERSION 3.18)
project(CloveCppExample LANGUAGES CXX)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(CloveCppExample ${SOURCES} ${HEADERS} ../../clove-unit.h)
target_include_directories(CloveCppExample PRIVATE ../../)

set_property(TARGET CloveCppExample PROPERTY CXX_STANDARD 11)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive-")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic-errors")
endif()
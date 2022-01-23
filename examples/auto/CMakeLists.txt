cmake_minimum_required(VERSION 3.18)
project(CloveAutoExample LANGUAGES C)

file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE HEADERS ${CMAKE_SOURCE_DIR}/src/*.h)

add_executable(AutoExample ${SOURCES} ${HEADERS} ../../clove.h)
target_include_directories(AutoExample PRIVATE ../../)
set_property(TARGET AutoExample PROPERTY C_STANDARD 11)

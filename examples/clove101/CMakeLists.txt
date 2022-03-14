cmake_minimum_required(VERSION 3.18)
project(Clove101Example LANGUAGES C)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

add_executable(Clove101Example ${SOURCES} ${HEADERS} ../../clove-unit.h)
target_include_directories(Clove101Example PRIVATE ../../)
set_property(TARGET Clove101Example PROPERTY C_STANDARD 11)

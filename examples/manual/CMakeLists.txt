cmake_minimum_required(VERSION 3.18)
project(CloveManualExample LANGUAGES C)

file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE HEADERS ${CMAKE_SOURCE_DIR}/src/*.h)

add_executable(ManualExample ${SOURCES} ${HEADERS} ../../clove.h)
target_include_directories(ManualExample PRIVATE ../../)
set_property(TARGET ManualExample PROPERTY C_STANDARD 11)

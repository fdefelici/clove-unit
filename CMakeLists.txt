cmake_minimum_required(VERSION 3.18)
project(CloveExample LANGUAGES C)

file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/example/*.c)
file(GLOB_RECURSE HEADERS ${CMAKE_SOURCE_DIR}/example/*.h)

add_executable(CloveExample ${SOURCES} ${HEADERS} clove.h)
target_include_directories(CloveExample PRIVATE .)
set_property(TARGET CloveExample PROPERTY C_STANDARD 11)

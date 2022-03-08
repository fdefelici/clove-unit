#cmake_minimum_required(VERSION 3.18)
#project(CLoveUnit LANGUAGES C)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
#file(GLOB_RECURSE HEADERS ${CMAKE_SOURCE_DIR}/src/*.h)

add_executable(CLoveUnitTest ${SOURCES} ${HEADERS} ../clove-unit.h)
target_include_directories(CLoveUnitTest PRIVATE ../)
set_property(TARGET CLoveUnitTest PROPERTY C_STANDARD 11)

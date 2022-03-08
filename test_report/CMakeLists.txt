#cmake_minimum_required(VERSION 3.18)
#project(CLoveUnit LANGUAGES C)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

add_executable(CLoveReportTest ${SOURCES} ../clove-unit.h)
target_include_directories(CLoveReportTest PRIVATE ../)
set_property(TARGET CLoveReportTest PROPERTY C_STANDARD 11)

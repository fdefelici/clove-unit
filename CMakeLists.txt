cmake_minimum_required(VERSION 3.18)
project(CLoveUnit LANGUAGES C)
include(CTest)

add_subdirectory(tests/functs)
add_subdirectory(tests/stricts/clove-c)
add_subdirectory(tests/stricts/clove-cpp)
add_subdirectory(examples/clove101)
add_subdirectory(examples/clovepp)

if (NOT DEFINED CI_TRIGGERED)
    add_subdirectory(tests/perfs)
endif()

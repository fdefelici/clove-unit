cmake_minimum_required(VERSION 3.18)
project(CLoveUnit LANGUAGES C)
include(CTest)

#[[
add_subdirectory(tests/functs)
if (DEFINED CLOVE_CMAKE__CI_TRIGGERED)
    add_subdirectory(tests/stricts/clove-c)
    add_subdirectory(tests/stricts/clove-cpp)
    add_subdirectory(examples/clove101)
    add_subdirectory(examples/clovepp)
endif()

if (DEFINED CLOVE_CMAKE__ENABLE_PERFS)
add_subdirectory(tests/perfs)
endif()
]]

if (CLOVE_CMAKE__UC_BUILD)
    add_subdirectory(tests/functs)
    add_subdirectory(tests/stricts/clove-c)
    add_subdirectory(tests/stricts/clove-cpp)
    add_subdirectory(examples/clove101)
    add_subdirectory(examples/clovepp)
endif()

if (CLOVE_CMAKE__UC_SANITY)
    add_subdirectory(tests/stricts/clove-sanity)
endif()

if (CLOVE_CMAKE__UC_PERFS)
    add_subdirectory(tests/perfs)
endif()

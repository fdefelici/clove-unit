cmake_minimum_required(VERSION 3.18)
project(CLoveUnit LANGUAGES C)
include(CTest)

add_library(clove-unit INTERFACE)
target_include_directories(clove-unit INTERFACE "${CMAKE_CURRENT_LIST_DIR}")

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

#[[ 
  In case this is the root project add dev targets.
  To avoid targets pollution when using FetchContent were only the target library is required
]] 
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)

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

endif()
cmake_minimum_required(VERSION 3.18)
project(StrictCloveC LANGUAGES C)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

add_executable(StrictCloveC ${SOURCES} ${HEADERS} ../../../clove-unit.h)
target_include_directories(StrictCloveC PRIVATE ../../../ ./src/)
set_property(TARGET StrictCloveC PROPERTY C_STANDARD 11)
set_property(TARGET StrictCloveC PROPERTY COMPILE_WARNING_AS_ERROR ON)

# Compiler Options
if (MSVC)
    target_compile_options(StrictCloveC PRIVATE /W4)
else() # GCC and CLANG
    target_compile_options(StrictCloveC PRIVATE -Wall -Wextra -Wpedantic)
endif()

cmake_minimum_required(VERSION 3.18)
project(StrictCloveSanity LANGUAGES C)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

add_executable(StrictCloveSanity ${SOURCES} ${HEADERS} ../../../clove-unit.h)
target_include_directories(StrictCloveSanity PRIVATE ../../../ ./src/)
set_property(TARGET StrictCloveSanity PROPERTY C_STANDARD 11)

# Compiler Options
if (MSVC)
    target_compile_options(StrictCloveSanity PRIVATE /fsanitize=address)
    #target_link_options(StrictCloveSanity PRIVATE /fsanitize=address)
else() # GCC and CLANG
    target_compile_options(StrictCloveSanity PRIVATE -fsanitize=address,leak,undefined -g)
    target_link_options(StrictCloveSanity PRIVATE -fsanitize=address,leak,undefined)
endif()

add_test(NAME StrictCloveSanity COMMAND StrictCloveSanity)
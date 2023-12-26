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

add_test(NAME Sanity_uc_default COMMAND StrictCloveSanity)
add_test(NAME Sanity_uc_help COMMAND StrictCloveSanity -h)
add_test(NAME Sanity_uc_version COMMAND StrictCloveSanity -v)
add_test(NAME Sanity_uc_include COMMAND StrictCloveSanity -i *.SimpleTest01)
add_test(NAME Sanity_uc_exclude COMMAND StrictCloveSanity -e *.SimpleTest01)
add_test(NAME Sanity_uc_pretty COMMAND StrictCloveSanity -r pretty)
add_test(NAME Sanity_uc_csv COMMAND StrictCloveSanity -r csv)
add_test(NAME Sanity_uc_json COMMAND StrictCloveSanity -r json)
add_test(NAME Sanity_uc_list COMMAND StrictCloveSanity -l)
add_test(NAME Sanity_uc_list_pretty COMMAND StrictCloveSanity -l -r pretty)
add_test(NAME Sanity_uc_list_csv COMMAND StrictCloveSanity -l -r csv)
add_test(NAME Sanity_uc_list_json COMMAND StrictCloveSanity -l -r json)
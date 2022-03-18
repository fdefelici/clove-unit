cmake_minimum_required(VERSION 3.18)
project(CLoveUnit LANGUAGES C)
include(CTest)

add_subdirectory(tests)
add_subdirectory(perfs)
add_subdirectory(examples/clove101)


enable_testing()
add_test(NAME CLoveUnitTest COMMAND CLoveUnitTest)
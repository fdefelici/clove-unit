cmake_minimum_required(VERSION 3.18)
project(CloveExample LANGUAGES C)

add_executable(CloveExample example/main.c example/test_suite1.h example/test_suite2.h example/test_suite3.h)
target_include_directories(CloveExample PRIVATE .)
set_property(TARGET CloveExample PROPERTY C_STANDARD 11)

cmake_minimum_required(VERSION 3.18)
project(CloveExample LANGUAGES C)

add_executable(CloveExample example/main.c example/test1.h example/test2.h)
target_include_directories(CloveExample PRIVATE .)
set_property(TARGET CloveExample PROPERTY C_STANDARD 11)

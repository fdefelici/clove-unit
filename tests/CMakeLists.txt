#cmake_minimum_required(VERSION 3.18)
#project(CLoveUnit LANGUAGES C)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

add_executable(CLoveUnitTest ${SOURCES} ${HEADERS} ../clove-unit.h)
target_include_directories(CLoveUnitTest PRIVATE ../ ./src/)
set_property(TARGET CLoveUnitTest PROPERTY C_STANDARD 11)
set_property(TARGET CLoveUnitTest PROPERTY COMPILE_WARNING_AS_ERROR ON)

add_subdirectory(resources/prj01)

# Resource project configuration
if(MSVC)
    #set(RES_PRJ01_EXEC_PATH ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE_INIT}/CLoveUnitResourcePrj01.exe)
    set(RES_PRJ01_EXEC_PATH \"${CMAKE_CURRENT_BINARY_DIR}/resources/prj01/${CMAKE_BUILD_TYPE_INIT}/CLoveUnitResourcePrj01.exe\")
else()
    set(RES_PRJ01_EXEC_PATH \"${CMAKE_CURRENT_BINARY_DIR}/resources/prj01/CLoveUnitResourcePrj01\")
endif()
add_compile_definitions(RES_PRJ01_EXEC_PATH=${RES_PRJ01_EXEC_PATH})

# Compiler Options
if (MSVC)
    target_compile_options(CLoveUnitTest PRIVATE /W4)
else() # GCC and CLANG
    target_compile_options(CLoveUnitTest PRIVATE -Wall -Wextra -Wpedantic)
endif()

#Automatically added by include CTest
#enable_testing()
#clove-unit -x option make the process end with error in case of test failure
add_test(NAME CLoveUnitTest COMMAND CLoveUnitTest -x)
#cmake_minimum_required(VERSION 3.18)
project(CLoveUnitTest LANGUAGES C)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

add_executable(CLoveUnitTest ${SOURCES} ${HEADERS} ../../clove-unit.h)
target_include_directories(CLoveUnitTest PRIVATE ../../ ./src/)
set_property(TARGET CLoveUnitTest PROPERTY C_STANDARD 11)
#set_property(TARGET CLoveUnitTest PROPERTY COMPILE_WARNING_AS_ERROR ON)

add_subdirectory(resources/prj01)

# Resource project configuration
if(MSVC)
    macro(to_win_path_escaped path_in_out)
        string(REGEX REPLACE "/" "\\\\\\\\" ${path_in_out} "${${path_in_out}}")
    endmacro()

    set(RES_PRJ01_EXEC_PATH \"${CMAKE_CURRENT_BINARY_DIR}/resources/prj01/${CMAKE_BUILD_TYPE_INIT}/CLoveUnitResourcePrj01.exe\")
    set(RES_PRJ01_TEST_PATH \"${CMAKE_CURRENT_SOURCE_DIR}/resources/prj01\")
    set(RES_PRJ01_TEST_PATH_UNIX ${RES_PRJ01_TEST_PATH})
    to_win_path_escaped(RES_PRJ01_EXEC_PATH)
    to_win_path_escaped(RES_PRJ01_TEST_PATH)
    #cmake_path(SET RES_PRJ01_TEST_PATH \"${CMAKE_CURRENT_SOURCE_DIR}/resources/prj01\")
    #cmake_path(NATIVE_PATH RES_PRJ01_TEST_PATH RES_PRJ01_TEST_PATH)
    #message(STATUS "RES_PRJ01_TEST_PATH=${RES_PRJ01_TEST_PATH}") 
else()
    set(RES_PRJ01_EXEC_PATH \"${CMAKE_CURRENT_BINARY_DIR}/resources/prj01/CLoveUnitResourcePrj01\")
    set(RES_PRJ01_TEST_PATH \"${CMAKE_CURRENT_SOURCE_DIR}/resources/prj01\")
    set(RES_PRJ01_TEST_PATH_UNIX ${RES_PRJ01_TEST_PATH})
endif()
add_compile_definitions(RES_PRJ01_EXEC_PATH=${RES_PRJ01_EXEC_PATH})
add_compile_definitions(RES_PRJ01_TEST_PATH=${RES_PRJ01_TEST_PATH})
add_compile_definitions(RES_PRJ01_TEST_PATH_UNIX=${RES_PRJ01_TEST_PATH_UNIX})

# Compiler Options
#[[
if (MSVC)
    target_compile_options(CLoveUnitTest PRIVATE /W4)
else() # GCC and CLANG
    target_compile_options(CLoveUnitTest PRIVATE -Wall -Wextra -Wpedantic)
endif()
]]

#Automatically added by include CTest
#enable_testing()
#clove-unit -x option make the process end with error in case of test failure
add_test(NAME CLoveUnitTest COMMAND CLoveUnitTest -x)
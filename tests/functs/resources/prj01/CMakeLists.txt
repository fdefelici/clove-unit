#cmake_minimum_required(VERSION 3.18)
#project(CLoveUnit LANGUAGES C)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
#file(GLOB_RECURSE HEADERS ${CMAKE_SOURCE_DIR}/src/*.h)

add_executable(CLoveUnitResourcePrj01 ${SOURCES} ../../../../clove-unit.h)
target_include_directories(CLoveUnitResourcePrj01 PRIVATE ../../../../)
set_property(TARGET CLoveUnitResourcePrj01 PROPERTY C_STANDARD 11)

#[[
get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()
]]




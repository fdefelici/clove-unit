file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

add_executable(CLoveUnitResourcePrj03 ${SOURCES} ../../../clove-unit.h)
target_include_directories(CLoveUnitResourcePrj03 PRIVATE ../../../)
set_property(TARGET CLoveUnitResourcePrj03 PROPERTY C_STANDARD 11)

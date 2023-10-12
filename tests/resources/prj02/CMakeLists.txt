file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

add_executable(CLoveUnitResourcePrj02 ${SOURCES} ../../../clove-unit.h)
target_include_directories(CLoveUnitResourcePrj02 PRIVATE ../../../)
set_property(TARGET CLoveUnitResourcePrj02 PROPERTY C_STANDARD 11)

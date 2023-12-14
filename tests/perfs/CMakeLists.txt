file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
add_executable(CLovePerfTest ${SOURCES} ../../clove-unit.h)
target_include_directories(CLovePerfTest PRIVATE ../../)
set_property(TARGET CLovePerfTest PROPERTY C_STANDARD 11)


file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/utils/*.c)
add_executable(CLovePerfGenerator ${SOURCES})
target_include_directories(CLovePerfGenerator PRIVATE ../../)
set_property(TARGET CLovePerfGenerator PROPERTY C_STANDARD 11)
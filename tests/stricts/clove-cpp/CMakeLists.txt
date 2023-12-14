file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(StrictCloveCpp ${SOURCES} ${HEADERS} ../../../clove-unit.h)
target_include_directories(StrictCloveCpp PRIVATE ../../../)

set_property(TARGET StrictCloveCpp PROPERTY CXX_STANDARD 11)
set_property(TARGET StrictCloveCpp PROPERTY COMPILE_WARNING_AS_ERROR ON)

# Compiler Options
if (MSVC)
    target_compile_options(StrictCloveCpp PRIVATE /W4)
else() # GCC and CLANG
    target_compile_options(StrictCloveCpp PRIVATE -Wall -Wextra -Wpedantic)
endif()

cmake_minimum_required(VERSION 3.18)

set(CLOVE_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})
list(INSERT CMAKE_MODULE_PATH 0 ${CLOVE_SOURCE_DIR}/cmake/modules)

# Extract CLOVE_VERSION from header file
set(CLOVE_VERSION_REGEX "#define __CLOVE_VERSION_.*[ \t]+(.+)")
file(STRINGS "${CLOVE_SOURCE_DIR}/clove-unit.h" CLOVE_VERSION REGEX ${CLOVE_VERSION_REGEX})
list(TRANSFORM CLOVE_VERSION REPLACE ${CLOVE_VERSION_REGEX} "\\1")
string(JOIN "." CLOVE_VERSION ${CLOVE_VERSION})

# Configure Project
project(
  CLoveUnit 
  VERSION ${CLOVE_VERSION}
  DESCRIPTION "Single-header Unit Testing framework for C (interoperable with C++) with test autodiscovery feature"
  HOMEPAGE_URL "https://github.com/fdefelici/clove-unit"
  LANGUAGES C
)

include(JoinPaths)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Eventually CMAKE_INSTALL_PREFIX can be overridden by the user to change base installation path for the package
join_paths(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}" clove-unit ${PROJECT_VERSION})

# Library Target definition and configuration
add_library(clove-unit INTERFACE)
add_library(clove-unit::clove-unit ALIAS clove-unit)

target_include_directories(
  clove-unit INTERFACE 
  $<BUILD_INTERFACE:${CLOVE_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(clove-unit INTERFACE c_std_11)

# Install command stuffs for enabling find_package usage
install(
    TARGETS clove-unit 
    EXPORT clove-unit-targets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

write_basic_package_version_file(
  clove-unit-config-version.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    ${CLOVE_SOURCE_DIR}/cmake/in/clove-unit-config.cmake.in
    clove-unit-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake
)

install(
    FILES ${CLOVE_SOURCE_DIR}/clove-unit.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    FILES
        ${PROJECT_BINARY_DIR}/clove-unit-config.cmake
        ${PROJECT_BINARY_DIR}/clove-unit-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake
)

install(
    EXPORT clove-unit-targets
    #NAMESPACE clove-unit::
    DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake
)

export(PACKAGE clove-unit)


#[[ 
  In case this is the root project add dev targets (Development mode).
  To avoid targets pollution when using FetchContent were only the target library is required
  Note: PROJECT_IS_TOP_LEVEL cmake variable exists in version 3.21+
]] 
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  include(CTest)

  option(CLOVE_CMAKE__UC_BUILD                  "enable build use case" OFF)
  option(CLOVE_CMAKE__UC_SANITY                 "enable sanity check use case" OFF)
  option(CLOVE_CMAKE__UC_PERFS                  "enable performance test use case" OFF)
  option(CLOVE_CMAKE__CPP_STRICT_WARN_AS_ERROR  "threat compilation warning as error" OFF)

  if (CLOVE_CMAKE__UC_BUILD)
    add_subdirectory(tests/functs)
    add_subdirectory(tests/stricts/clove-c)
    add_subdirectory(tests/stricts/clove-cpp)
    add_subdirectory(examples/clove101)
    add_subdirectory(examples/clovepp)
  endif()

  if (CLOVE_CMAKE__UC_SANITY)
    add_subdirectory(tests/stricts/clove-sanity)
  endif()

  if (CLOVE_CMAKE__UC_PERFS)
    add_subdirectory(tests/perfs)
  endif()

endif()

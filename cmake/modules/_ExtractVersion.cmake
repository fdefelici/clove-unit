# Extract clove version from clove-unit.h file
function(clove_get_version out_version)
    set(CLOVE_VERSION_REGEX "#define __CLOVE_VERSION_.*[ \t]+(.+)")
    file(STRINGS "${CMAKE_CURRENT_LIST_DIR}/clove-unit.h" CLOVE_VERSION REGEX ${CLOVE_VERSION_REGEX})
    list(TRANSFORM CLOVE_VERSION REPLACE ${CLOVE_VERSION_REGEX} "\\1")
    string(JOIN "." CLOVE_VERSION ${CLOVE_VERSION})
    set(${out_version} "${CLOVE_VERSION}" PARENT_SCOPE)
endfunction()


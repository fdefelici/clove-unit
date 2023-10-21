#define CLOVE_SUITE_NAME UNIT_PathTest
#include "clove-unit.h"

CLOVE_TEST(PathConcatenation) {
    char* result = __clove_path_concat('/', "path/to/first", "second");
    CLOVE_ULLONG_EQ(20, __clove_string_length(result));
    CLOVE_STRING_EQ("path/to/first/second", result);
}   

CLOVE_TEST(PathConcatConvertingSeparator) {
    char* result = __clove_path_concat('\\', "path/to/first", "second");
    CLOVE_ULLONG_EQ(20, __clove_string_length(result));
    CLOVE_STRING_EQ("path\\to\\first\\second", result);
} 

CLOVE_TEST(AbsolutePathFromExecBasePath) {
    char* result = __clove_path_rel_to_abs_exec_path("path/to/append");
    CLOVE_NOT_NULL(result);
}   
#include <string.h>
#include <stdlib.h>

CLOVE_TEST(BasePathForJustExecutable) {
    char expected[3] = { '.', __CLOVE_PATH_SEPARATOR, '\0' };
    char* result = __clove_path_basepath("file.exe");
    CLOVE_STRING_EQ(expected, result);
}

CLOVE_TEST(BasePathForFullPath) {
    char expected[] = "\\path\\to\\directory";
    __clove_path_to_os (expected);

    char* result = __clove_path_basepath("/path/to/directory/file.txt");
    CLOVE_STRING_EQ(expected, result);
    free(result);
}

CLOVE_TEST(BasePathForDirectory) {
    char expected[] = "\\path\\to\\directory";
    __clove_path_to_os (expected);

    char* result = __clove_path_basepath("/path/to/directory/");
    CLOVE_STRING_EQ(expected, result);
    free(result);
}

CLOVE_TEST(PathToOs) {
#ifdef _WIN32
    const char* expected = "\\path\\to\\directory";
    char provided[] = "/path/to/directory";
#else
    const char* expected = "/path/to/directory";
    char provided[] = "\\path\\to\\directory";
#endif
    __clove_path_to_os(provided);
    CLOVE_STRING_EQ(expected, provided);
}

CLOVE_TEST(PathToOsMixed) {
    char provided[] = "/path\\to/directory";
#ifdef _WIN32
    const char* expected = "\\path\\to\\directory";
#else
    const char* expected = "/path/to/directory";
#endif
    __clove_path_to_os(provided);
    CLOVE_STRING_EQ(expected, provided);
}

#define CLOVE_SUITE_NAME PathTest
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

CLOVE_TEST(BasePathForJustExecutable) {
    char expected[3] = { '.', __CLOVE_PATH_SEPARATOR, '\0' };

    char* result = __clove_path_basepath("file.exe");
    CLOVE_STRING_EQ(expected, result);
}  
#include "domain_utils.h"

__clove_suite_t create_suite(const char* name, size_t test_count) {
    __clove_suite_t suite;
    suite.name = (char*)name; //remove const just to avoid build warnings
    suite.test_count = test_count;
    return suite;
}

__clove_test_t create_test(const char* name) {
    __clove_test_t test;
    test.name = (char*)name; //remove const just to avoid build warnings
    test.file_name = "abs"__CLOVE_PATH_SEPARATOR_STR"test_file.c";
    test.result = __CLOVE_TEST_RESULT_PASSED;
    test.duration.seconds = 0;
    test.duration.nanos_after_seconds = 100;
    return test;
}

__clove_test_t create_test_fail(const char* name) {
    __clove_test_t test;
    test.name = (char*)name; //remove const just to avoid build warnings
    test.file_name = "abs"__CLOVE_PATH_SEPARATOR_STR"test_file.c";
    test.result = __CLOVE_TEST_RESULT_FAILED;
    test.duration.seconds = 0;
    test.duration.nanos_after_seconds = 100;
    test.issue.line = 8;
    test.issue.assert = __CLOVE_ASSERT_EQ;
    test.issue.data_type = __CLOVE_GENERIC_BOOL;
    test.issue.expected._bool = false;
    test.issue.actual._bool = true;
    return test;
}

__clove_test_t create_test_skip(const char* name) {
    __clove_test_t test;
    test.name = (char*)name; //remove const just to avoid build warnings
    test.file_name = "abs"__CLOVE_PATH_SEPARATOR_STR"test_file.c";
    test.result = __CLOVE_TEST_RESULT_SKIPPED;
    return test;
}
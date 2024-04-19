#include "domain_utils.h"

__clove_suite_t create_suite(const char* name) {
    __clove_suite_t suite;
    suite.name = (char*)name; //remove const just to avoid build warnings
    suite.test_count = 0;
    __CLOVE_VECTOR_INIT(&suite.tests, __clove_test_t);
    suite.issue.passed_count = 0;
    suite.issue.failed_count = 0;
    suite.issue.skipped_count = 0;
    return suite;
}

void suite_add_test(__clove_suite_t* suite, __clove_test_t* test) 
{
    suite->test_count++;
    __CLOVE_VECTOR_ADD(&suite->tests, __clove_test_t, *test);

    if (test->result == __CLOVE_TEST_RESULT_PASSED) suite->issue.passed_count++;
    else if (test->result == __CLOVE_TEST_RESULT_FAILED) suite->issue.failed_count++;
    else if (test->result == __CLOVE_TEST_RESULT_SKIPPED) suite->issue.skipped_count++;
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
    test.duration.nanos_after_seconds = 50;
    test.duration.seconds = 0;
    test.funct_line = 4;
    return test;
}